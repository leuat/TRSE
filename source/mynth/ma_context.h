#pragma once

#include "source/LeLib/miniaudio.h"
#include "source/LeLib/miniaudio_engine.h"

extern ma_engine audio_engine;

void audio_shutdown();
bool audio_init();
