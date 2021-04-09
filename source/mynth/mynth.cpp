/*
stuff i wished C had
enum ADSRStage : struct { size_t, mask8_t, char* } {
	Attack = {0, _, "Attack},
	...
};

float envelope[ADSRStage]

*/

#include "macros_etc.h"
#include "sg_imgui_ext.h"
#include "skeleton/includes.h"
#include "skeleton/application.h"
#include "majdi/piano_keymap.h"
#include "majdi/ma_context.h"
#include "majdi/mynthesizer.h"

using namespace mynth;

struct Mynthesizer {
	Voice voices[2];
} mynthesizer;

void mynthesizer_init() {
	for (auto& voc : mynthesizer.voices) {
		voice_init(&audio_engine, voc);
		ma_sound_start(&voc.sound);
	}
}

void mynthesizer_shutdown() {
	for (auto& voc : mynthesizer.voices) {
		voice_shutdown(voc);
	}
}


struct JamContext {
	JamMode mode = JamMode::Mono;
	float mono_last_frequency = -1;
	int mono_last_note = -1;
	int mono_channel = 0;
	ADSRSettings adsr;
	float wavemixer[4] = { 0, 1, 0, 0 };
	LFO vibrato;
} jam_context;

void send_keyboard_mynth_note(int midi_note, int velocity) {
	for (int n = 0; n < 1; n++)
	switch (jam_context.mode) {
	case JamMode::Mono: {
		if ((velocity == 0) && (jam_context.mono_last_frequency > -1)) {				
			if (midi_note != jam_context.mono_last_note) return;
			SynthCommand cmd;
			note_off(mynthesizer.voices[jam_context.mono_channel + n], cmd);
			voice_command(mynthesizer.voices[jam_context.mono_channel+n], cmd);
			break;
		}
		SynthCommand cmd;										// New command
		cmd.active = true;										// Tells the synth this is a new command
		cmd.gate_set = true;
		cmd.gate = velocity > 0;								// Gate: Sound is on
		cmd.freq = TET12(midi_note);							// 12-TET = translate midi note to frequency
		cmd.velocity = velocity / 128.f;						// MIDI velocity, 0-128. Not used
		// adsr
		cmd.adsr_set = true;									// Let synth know we're supplying a new ADSR
		cmd.adsr = jam_context.adsr;							// Copy ADSR
		// wave mixer
		cmd.wavemixer_set = true;								
		memcpy(cmd.wavemixer,									// Copy oscillator mixer
			   jam_context.wavemixer, sizeof(Wavemixer));		
		// vibrato
		cmd.vibrato_set = true;
		cmd.vibrato = jam_context.vibrato;						// Copy vibrato
		// internal jam_context
		jam_context.mono_last_frequency = cmd.freq;
		jam_context.mono_last_note = midi_note;
		// Submit command to synth
		voice_command(mynthesizer.voices[jam_context.mono_channel+n], cmd);
	}
	}
}

void adsr_widget(ADSRSettings& adsr) {
	ImGui::SliderFloat2("Level", &adsr.level[1], 0.01f, 1.0f, "%.4f");
	ImGui::SliderFloat4("Time", adsr.time, 0.01f, 15, "%.4fs", 3);
}

void keyboard_jamming() {
	// Handle keyboard
	ImGuiIO& io = ImGui::GetIO();
	static int octave = 3;
	if (io.KeysDown[GLFW_KEY_4] && io.KeysDownDuration[GLFW_KEY_4] == 0) {
		octave = MIN(octave + 1, 9);
	}
	if (io.KeysDown[GLFW_KEY_8] && io.KeysDownDuration[GLFW_KEY_8] == 0) {
		octave = MAX(octave - 1, 0);
	}
	for (int n = 0; n < MAX_PIANO_KEYMAP_ENTRIES; n++) {
		int keycode = piano_keymap[n].keycode;
		int note_value = piano_keymap[n].note_value + octave * 12;
		if (!io.KeysDown[keycode]) {
			if (io.KeysDownDurationPrev[keycode] > 0) {
				send_keyboard_mynth_note(note_value, 0);
			}
			continue;
		}
		if (io.KeysDownDuration[keycode] == 0) {
			send_keyboard_mynth_note(note_value, 100);
		}
	}
}

// If zeroed out you have an empty patterns
struct TrackerStep {
	bool note_off : 1;
	uint8_t note : 7;
	
};

#include <vector>

typedef std::vector<TrackerStep> ColumnData;

struct TrackerColumn {
	ColumnData steps;
};

struct TrackerTranport {
	bool playing = true;
	float position = 0;
	float max_length = 16;
} tracker_transport;

TrackerColumn test_tracker_column;

void tracker_ui(TrackerColumn& pattern) {
	ImGui::BeginChild("Tracker");
	int n = 0;
	for (auto &row : pattern.steps) {
		if (row.note_off)
			ImGui::Text("===", row.note);
		else if (row.note) 
			ImGui::Text("%d", row.note);
		else 
			ImGui::Text("---", row.note);
		if (n == int(tracker_transport.position)) {
			ImGui::SameLine(); ImGui::TextUnformatted("<");
		}
		n++;
	}
	static bool is_playing = false;
	ImGui::Checkbox("Playing", &is_playing);
	ImGui::End();
	if (is_playing) {
		TrackerStep& step = test_tracker_column.steps[int(tracker_transport.position)];
		if (step.note) {
			SynthCommand cmd;												// New command
			note_on(mynthesizer.voices[0], cmd, TET12(step.note), 1.0);
			set_adsr(mynthesizer.voices[0], cmd,
				ADSRSettings{
				{ 0.001, 0, 1000, 0.001, 0 },
				{ 0, 1.0, 1.0, 0 }
				});
			voice_command(mynthesizer.voices[0], cmd);
		}
		if (step.note_off) {
			SynthCommand cmd;												// New command
			note_off(mynthesizer.voices[0], cmd);
			voice_command(mynthesizer.voices[0], cmd);
		}
		{
			tracker_transport.position += float(global_render_context.dt) * 2 * (170.f / 60.f);
			tracker_transport.position = fmod(tracker_transport.position, tracker_transport.max_length);
		}
	}
}

void mynth_ui() {
	keyboard_jamming();
	ImGui::Begin("Mynth");
	adsr_widget(jam_context.adsr);
	ImGui::SliderFloat4("Mixer", jam_context.wavemixer, 0.f, 1.f, "%.3fx", 2);
	ImGui::SliderFloat("Vibrato Depth", &jam_context.vibrato.depth, 0.f, 1.f, "%.3fx", 2);
	ImGui::SliderFloat("Vibrato Rate", &jam_context.vibrato.rate, 0.f, 100.f, "%.3fHz", 2);
//	if (_DEBUG) {
		ImGui::Text("Size of tracker column: %d bytes", sizeof(TrackerStep));
//	}
	tracker_ui(test_tracker_column);
	ImGui::End();
}

void mynth_init() {
	audio_init();
	mynthesizer_init();
	{
		ColumnData &col = test_tracker_column.steps;
		col.resize(16);
		col[0].note = 40;
		col[1].note_off = true;
		col[2].note = 47;
		col[5].note_off = true;
		col[6].note = 43;
		col[8].note = 40;
		col[9].note = 41;
		col[10].note_off = true;
		col[11].note = 47;
		col[12].note = 43;
	}
}

void mynth_shutdown() {
	audio_shutdown();
	mynthesizer_shutdown();
}

int main(int argc, char** argv) {
	render_context_init();
	init_application();
	RenderContext &render_context = global_render_context;
	create_window(render_context);
	mynth_init();
	while (!glfwWindowShouldClose(render_context.window)) {
		new_frame(render_context);
		clear(render_context);
		application_input_handling(render_context);

		mynth_ui();

		present_application(render_context);
	}
	mynth_shutdown();
	render_context_shutdown(render_context);
	return EXIT_SUCCESS;
}
