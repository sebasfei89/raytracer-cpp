#pragma once

#include <algorithm>

constexpr float EPSILON = 0.0001f;

constexpr float SQRT2 = 1.414213562373f; //std::sqrtf(2.f);
constexpr float SQRT2OVR2 = SQRT2 * .5f;

inline bool Equals(float a, float b)
{
    return std::abs(a - b) <= EPSILON;
}

inline int Clamp(int value, int min, int max)
{
    return std::max(min, std::min(max, value));
}
