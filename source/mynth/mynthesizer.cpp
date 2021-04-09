#include "mynthesizer.h"
#include <stdio.h>
#include <string.h>
// Recalculate the ADSR envelope whenever it is needed
void mynth::calculate_adsr(VoiceDataSource* pb) {
        ADSRState& state = pb->adsr_state;
        ADSRSettings& adsr = pb->adsr;
        if (state.stage == ADSRStage::Idle) return;				// We're already idle!
        float	target = adsr.level[int(state.stage) + 1] * pb->velocity;
int		length = int(adsr.time[int(state.stage)] * pb->pSampleRate);
        double	current = pb->amplitude;
        // Find coefficient
state.multiplier = 1.0 + (log(fmax(MINIMUM_AMPLITUDE, target)) - log(fmax(MINIMUM_AMPLITUDE, current))) / length;
        state.samples_left = length;							// Next time the ADSR will change state
}

inline void mynth::process_adsr(VoiceDataSource* pb) {
        ADSRState& state = pb->adsr_state;
        state.samples_left--;
        if (!state.samples_left) {
                state.stage = (ADSRStage)((int)state.stage + 1);	// Evil increase of enum
                if (state.stage == ADSRStage::Idle) return;			// We're already idle!
                calculate_adsr(pb);									// State changed, reculculate the ADSR
        }
        return;
}

/*
        Synth PCM generation
*/

// The audio & magic is created here
// Callback for ma_engine that produces audio
ma_result mynth::voice_read_pcm(ma_data_source* pDataSource, void* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead) {
        VoiceDataSource* pb = (VoiceDataSource*)pDataSource;
        double dt = 1.f / pb->pSampleRate;
        // Copy out the command (and clear single shot things)
        SynthCommand cmd;
        pb->command_lock.lock();
        if (pb->command.active) {
                cmd = pb->command;
                pb->command.active = false;
                pb->command.gate = false;
        }
        pb->command_lock.unlock();
        // Threading done, check if we're idle
        if ((pb->adsr_state.stage == ADSRStage::Idle) && (!cmd.active)) {
                // Early out, generate only silence
                // TODO: Update registers (pb_period, lfo's etc)
                memset(pFramesOut, 0, (size_t)frameCount * 2 * sizeof(float));
                *pFramesRead = frameCount;
                return MA_SUCCESS;
        }
        // Process command
        if (cmd.active) {
                // Parameters
                if (cmd.adsr_set) pb->adsr = cmd.adsr;
                if (cmd.wavemixer_set) {
                        // pb->wavemixer = Wavemixer{ cmd.wavemixer }; // Why isn't this possible still in 2021?!
                        memcpy(pb->wavemixer, cmd.wavemixer, sizeof(Wavemixer));
                }
                if (cmd.vibrato_set) pb->vibrato_target = cmd.vibrato;
                if (cmd.velocity_set) pb->velocity = cmd.velocity;
                if (cmd.slide_rate_set) pb->slide_rate_target = cmd.slide_rate;
                // Handle trigging
                if (cmd.gate_set) {
//                        bool down = (!pb->gate) && (cmd.gate);
                        bool down = cmd.gate;
                        bool release = (pb->gate) && (!cmd.gate);
                        pb->gate = cmd.gate;
                        if (down) pb->adsr_state.stage = ADSRStage::Attack;				// If key is pressed, attack
                        if (release) pb->adsr_state.stage = ADSRStage::Release;			// If key is released, release
                        if (down | release) calculate_adsr(pb);							// Update amp ADSR
                }
                // Note handling
                if (cmd.freq) {
                        pb->freq_target = cmd.freq;								// Set frequency
                        // (TODO: SlideRate == 0)
                        if (!pb->freq) pb->freq = cmd.freq;					// First note ever. Set it without ever sliding
                }
        }
        // Update params that don't need to be interpolated across the audiobuffer
        double full_dt = dt * frameCount;
        pb->slide_rate = approach(pb->slide_rate, pb->slide_rate_target, APPROACH_SPEED_SLIDE_RATE * full_dt);
        // Generate and update oscillators
        float* buf = (float*)pFramesOut;
        size_t processed_frames = 0;
        for (int n = 0; n < frameCount; n++) {
                // Exit if faded out
                if (pb->adsr_state.stage == ADSRStage::Idle) {
                        break;
                }
                // Param interpolation
                pb->freq = approach(pb->freq, pb->freq_target, pb->slide_rate * dt);
                pb->vibrato.depth = approach(pb->vibrato.depth, pb->vibrato_target.depth, float(APPROACH_SPEED_VIBRATO_DEPTH * dt));
                pb->vibrato.rate = approach(pb->vibrato.rate, pb->vibrato_target.rate, float(APPROACH_SPEED_VIBRATO_RATE * dt));
                // Run oscillators
                double t = pb->period;
                double a = pb->amplitude;
                float f_sin = pb->wavemixer[0] ? sine_f32(t, a) : 0;
                float f_sqr = pb->wavemixer[1] ? square_f32(t, a) : 0;
                float f_saw = pb->wavemixer[2] ? sawtooth_f32(t, a) : 0;
                float f_tri = pb->wavemixer[3] ? triangle_f32(t, a) : 0;
                // Mix (TODO: smooth the mixer, TODODO: check if this is optimized or not)
                float f = f_sin * pb->wavemixer[0] +
                        f_sqr * pb->wavemixer[1] +
                        f_saw * pb->wavemixer[2] +
                        f_tri * pb->wavemixer[3];
                // Write
                buf[n * 2] = f; // left
                buf[n * 2 + 1] = f; // right
                // Update
                double dp = pb->freq * (1 + pb->vibrato.depth * sin(pb->vibrato_phase * PI * 2));		// Vibrato
                pb->period += dp / pb->pSampleRate;															// Advance the period
                pb->vibrato_phase += pb->vibrato.rate / pb->pSampleRate;
    pb->amplitude = fmin(1, fmax(MINIMUM_AMPLITUDE, pb->amplitude * pb->adsr_state.multiplier));	// Adjust the amplitude
                process_adsr(pb);																			// Check the ADSR
                processed_frames++;
        }
        // Zero the rest of the buffer (if there are any frame left to generate)
        size_t frames_left = frameCount - processed_frames;
        if (frames_left) memset(buf + processed_frames * 2, 0, frames_left * 2 * sizeof(float)); // Zero out the rest
        // DONE
        pb->t += frameCount; // This timer is here just for vibrato and should be gone
        *pFramesRead = frameCount;
        return MA_SUCCESS;
}

/*
mini_audio interface
*/

// Callback for miniaudio to know our format
ma_result mynth::voice_get_data_format(ma_data_source* pDataSource, ma_format* pFormat, ma_uint32* pChannels, ma_uint32* pSampleRate) {
        VoiceDataSource* pb = (VoiceDataSource*)pDataSource;
        *pFormat = ma_format_f32;			// Float is fast
        *pChannels = 2;						// Stereo
        *pSampleRate = pb->pSampleRate;
        return MA_SUCCESS;
}

// Setup callbacks for the voice
void mynth::VoiceDataSource_init(ma_engine* audio_engine, VoiceDataSource* voice) {
        voice->ds.onRead = voice_read_pcm;
        voice->ds.onGetDataFormat = voice_get_data_format;
        voice->pSampleRate = ma_engine_get_sample_rate(audio_engine);
}

/*
        Voice management
*/


// Submit command to synth.
void mynth::voice_command(Voice& voc, SynthCommand& cmd) {
        cmd.active = true;
        voc.src.command_lock.lock();
        voc.src.command = cmd;
        voc.src.command_lock.unlock();
}

// Initialize the voice into miniaudio-engine
void mynth::voice_init(ma_engine* audio_engine, Voice& voc, ma_sound_group* group) {
        ma_uint32 flags = MA_SOUND_FLAG_ASYNC;
        VoiceDataSource_init(audio_engine, &voc.src);
        ma_sound_init_from_data_source(audio_engine, (ma_data_source*)&voc.src, flags, group, &voc.sound);
}

// Cleanup
void mynth::voice_shutdown(Voice& voc) {
        ma_sound_stop(&voc.sound);
        ma_sound_uninit(&voc.sound);
}

/*
        Helpers
*/

float mynth::TET12(int midi_note) {
        return 440.f * pow(2.f, (midi_note - 69.f) / 12.f);
}

void mynth::note_on(Voice& voice, SynthCommand& cmd, const float freq, const float velocity) {
        cmd.gate_set = true;
        cmd.gate = true;
        cmd.freq = freq;
        if (velocity > 1) {
                cmd.velocity_set = true;
                cmd.velocity = velocity;
        }
}

void mynth::note_off(Voice& voice, SynthCommand& cmd) {
        cmd.gate_set = true;
        cmd.gate = false;
}

void mynth::set_adsr(Voice& voice, SynthCommand& cmd, const ADSRSettings& adsr) {
        cmd.adsr_set = true;
        cmd.adsr = adsr;
}


