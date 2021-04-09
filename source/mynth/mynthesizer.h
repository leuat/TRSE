#pragma once

#include <mutex>
#include <stdint.h>
#include "ma_context.h"
#include "mynth_osc.h"
#include <algorithm>
#include <math.h>

class mynth {
	// maths
public:
#define E_V 2.71828182845904523536
#define PI 3.14159265
/*float lerp(float a, float b, float f)
{
	return (a * (1.0 - f)) + (b * f);
}

float lerp01(float a, float b, float f)
{
    return fmin(1, fmax(0, (a * (1.0 - f)) + (b * f)));
}*/

	enum class ADSRStage {
		Attack = 0,
		Decay = 1,
		Sustain = 2,
		Release = 3,
		Idle = 4
	};

	/*	ADSR configuration
		Time and level is indexed by ADSRStage.
		Time is the time it takes to complete each stage (in seconds)
		Level is the amplification for each stage (between 0..1)

		For ASR og AR configuration, set the correspending time[stage] to 0.
		Example for simple beeper:
		adsr.time = {0.005, 0, 10000.f, 0.005 };	// Sharp rise and release, no decay, very long sustain
        adsr.level = {0, 1, 1 };					// Decay == Sustain, fmax level

	*/
	struct ADSRSettings {
		float time[5] = { 0.005f, 0.02f, 1000.f, 2.f };	// time[4] is ignored
		float level[5] = { 0, 1, 0.5f };				// level[0], [3] and [4] must be zero (or you'll have clicks, hung notes or worse)
	};

	typedef float Wavemixer[4];

	struct LFO {
		float depth = 0;	// Amplitude factor between 0..1
		float rate = 0;		// Rate in seconds
	};

	// Command sent across the woven threads
	struct SynthCommand {
		bool active = false;		// Should maybe be an atomic so that the necessity to lock is reduced
		// Gate (note on)
		bool gate_set = false;
		bool gate = false;
		// Note
		float gain = 1.0f;
		float freq = 0;				// Set to zero for no new note
		// Slide / Portamento
		bool slide_rate_set = false;
		float slide_rate = 20000.;
		// Velocity
		bool velocity_set = false;
		float velocity = 1.0f;		// Between 0..1
		// ADSR
		bool adsr_set = false;
		ADSRSettings adsr;			// See ADSRSettings
		// Oscillators
		bool wavemixer_set = false;
		Wavemixer wavemixer = { 0, 1, 0, 0 };	// Amplitude for Sine, Square, Saw and Triangle. Between 0..1
		// Vibrato
		bool vibrato_set = false;
		LFO vibrato;							// See LFO
	};

	// Internal state for voice
	struct ADSRState {
		ADSRStage stage = ADSRStage::Idle;
		int samples_left = 0;
		float target = 0;
		double multiplier = 0;
	};

    // miniaudio datasource.
	struct VoiceDataSource {
        ma_data_source_callbacks ds = {0}; // must be first!
		ma_uint32 pSampleRate;
		float t = 0.f;
		double period = 0.f;
		double amplitude = 0.f;
		bool gate = false;
		// parameters
		float velocity = 1.0f;
		double freq_target = 0.0f;
		double slide_rate_target = 20000.0;
		ADSRSettings adsr;
		Wavemixer wavemixer = { 0, 1, 0, 0 };
		LFO vibrato_target;
		// parameter state
		double freq = 0.f;
		double slide_rate = 20000.0;
		ADSRState adsr_state;
		LFO vibrato;
		float vibrato_phase = 0;
		// public
		std::mutex command_lock;
		SynthCommand command;
	};
    // A single channel voice
    struct Voice {
            VoiceDataSource src;
            ma_sound sound;
    };

	/*
		ADSR	Envelopes
	*/
    static constexpr const double MINIMUM_AMPLITUDE = 0.001;	// For ADSR to avoid mathematical mayhem


    static void calculate_adsr(VoiceDataSource* pb);
    static inline void process_adsr(VoiceDataSource* pb);

    /*
            Synth PCM generation
    */


    #define sign(x) ((x > 0) - (x < 0))
    // Simple function to aid interpolating parameters
    template <typename T>
    static inline T approach(T current, T target, T speed) {
            return current + sign(target - current) * (speed);
    }

    // Speed at which to interpolate values, in seconds
    #define APPROACH_SPEED_SLIDE_RATE 20000.
    #define APPROACH_SPEED_VIBRATO_DEPTH 1000.
    #define APPROACH_SPEED_VIBRATO_RATE 1000.

    // The audio & magic is created here
    // Callback for ma_engine that produces audio
    static ma_result voice_read_pcm(ma_data_source* pDataSource, void* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead);
    // Callback for miniaudio to know our format
    static ma_result voice_get_data_format(ma_data_source* pDataSource, ma_format* pFormat, ma_uint32* pChannels, ma_uint32* pSampleRate);
    // Setup callbacks for the voice
    static void VoiceDataSource_init(ma_engine* audio_engine, VoiceDataSource* voice);

    // Submit command to synth.
    static void voice_command(Voice& voc, SynthCommand& cmd);
    // Initialize the voice into miniaudio-engine
    static void voice_init(ma_engine* audio_engine, Voice& voc, ma_sound_group* group=nullptr);
    static void voice_shutdown(Voice& voc);

    static float TET12(int midi_note);

    static void note_on(Voice& voice, SynthCommand& cmd, const float freq, const float velocity = -1);

    static void note_off(Voice& voice, SynthCommand& cmd);
    static void set_adsr(Voice& voice, SynthCommand& cmd, const ADSRSettings& adsr);

};
