#pragma once

#include "source/LeLib/miniaudio.h"
#include <math.h>
#ifndef MA_TAU_D
#define MA_TAU_D   6.28318530717958647693
#endif


static MA_INLINE double ma_sin(double x)
{
    /* TODO: Implement custom sin(x). */
    return sin(x);
}

#define ma_abs(x)                   (((x) > 0) ? (x) : -(x))

static float sine_f32(double time, double amplitude)
{
    return (float)(ma_sin(MA_TAU_D * time) * amplitude);
}


static float square_f32(double time, double amplitude)
{
    double f = time - (ma_int64)time;
    double r;

    if (f < 0.5) {
        r = amplitude;
    }
    else {
        r = -amplitude;
    }

    return (float)r;
}


static float triangle_f32(double time, double amplitude)
{
    double f = time - (ma_int64)time;
    double r;

    r = 2 * ma_abs(2 * (f - 0.5)) - 1;

    return (float)(r * amplitude);
}

static float sawtooth_f32(double time, double amplitude)
{
    double f = time - (ma_int64)time;
    double r;

    r = 2 * (f - 0.5);

    return (float)(r * amplitude);
}
