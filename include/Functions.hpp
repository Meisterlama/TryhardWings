#pragma once
#include "raymath.h"

inline float sinMapped(float x, bool descending)
{
    float value = 0.5f * sin(x * PI - PI/2.0f) + 0.5f;
    return (descending) ? value : -value;
}

inline float tanhMapped(float x, bool descending)
{
    float expo = expf(-6.f * (2.f * x - 1.f));
    float value = (((1.f - expo)/(1.f + expo)) + 1.f)/2.f;
    return (descending) ? value : -value;
}
inline float elliptic(float x)
{
    return (x < sqrtf(2.0f)/2.0f) ? sqrtf(1.0f-x*x)
                                 : sqrtf(2.0f) - sqrtf(1.0f - (x-sqrtf(2.0f)) * (x-sqrtf(2.0f)));
}

inline float ellipticMapped(float x, bool descending)
{
    float value = -elliptic(x * sqrtf(2.0f)) / (2.0f-sqrtf(2.0f)) + sqrtf(2.0f)/2.0f + 1.0f;
    return (descending) ? value : -value;
}

inline float polynomialMapped(float x,bool descending)
{
    float value = (-2.f * x * x * x + 3 * x * x);
    return (descending) ? value : -value;
}