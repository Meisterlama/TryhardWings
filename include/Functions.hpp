#pragma once
#include "raymath.h"

inline float sinMapped(float x, bool descending, bool derivative)
{
    float value = 0;
    if (derivative)
    {
        value = 0.5f * PI * cos(x*PI - PI/2.0f);
    }
    else
    {
        value = 0.5f * sin(x * PI - PI / 2.0f) + 0.5f;
    }
    return (descending) ? value : -value;
}

inline float tanhMapped(float x, bool descending, bool derivative)
{
    float expo = expf(-6.f * (2.f * x - 1.f));
    float value = 0;
    if (derivative)
    {
        value = 12.0f * expo / (2*expo + expo*expo + 1);
    }
    else
    {
        value = 0.5f * ((1.f - expo)/(1.f + expo)) + 0.5f;
    }
    return (descending) ? value : -value;
}
inline float elliptic(float x, bool derivative)
{
    float value = 0;
    if (derivative)
    {
        value = (x < sqrtf(2.0f)/2.0f) ? -x / sqrt(-x*x + 1.0f)
                                       : (x - sqrt(2.0f))/(sqrt(-(x-sqrt(2.0f))*(x-sqrt(2.0f)) + 1.0f));
    }
    else
    {
        value = (x < sqrtf(2.0f)/2.0f) ? sqrtf(1.0f-x*x)
                                       : sqrtf(2.0f) - sqrtf(1.0f - (x-sqrtf(2.0f)) * (x-sqrtf(2.0f)));
    }
    return value;
}

inline float ellipticMapped(float x, bool descending, bool derivative)
{
    float value = 1.f -elliptic(x * sqrtf(2.0f), derivative) / (2.0f-sqrtf(2.0f)) + sqrtf(2.0f)/2.0f;
    return (descending) ? value : -value;
}

inline float polynomialMapped(float x,bool descending, bool derivative)
{
    float value = 0;
    if (derivative)
    {
        value = (-6.f * x * x + 6.f * x);
    }
    else
    {
        value = (-2.f * x * x * x + 3 * x * x);
    }
    return (descending) ? value : -value;
}