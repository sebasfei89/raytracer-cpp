#pragma once

#include <algorithm>

constexpr float EPSILON = 0.00001f;

bool Equals(float a, float b)
{
    return std::abs(a - b) <= EPSILON;
}