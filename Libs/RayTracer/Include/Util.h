#pragma once

#include <algorithm>

constexpr float EPSILON = 0.0001f;

inline bool Equals(float a, float b)
{
    return std::abs(a - b) <= EPSILON;
}

inline int Clamp(int value, int min, int max)
{
    return std::max(min, std::min(max, value));
}
