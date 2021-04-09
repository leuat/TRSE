#define MINIAUDIO_IMPLEMENTATION
#include "ma_context.h"

/*
	Some potential improvements:
	-	Select audio out, etc

*/

ma_engine audio_engine; // Global instance. We only need one audio engine. Ever.

void audio_shutdown() {
	ma_engine_uninit(&audio_engine);
}

bool audio_init() {
	ma_result result;
	result = ma_engine_init(NULL, &audio_engine);
	if (result != MA_SUCCESS) {
		printf("Failed to initialize audio engine.");
		return false;
	}
	return true;
}