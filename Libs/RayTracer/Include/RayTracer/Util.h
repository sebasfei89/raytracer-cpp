#pragma once

#include <algorithm>
#include <cmath>

constexpr float PI = 3.14159265359f;
constexpr float PIOVR2 = PI / 2.f;
constexpr float PIOVR3 = PI / 3.f;
constexpr float PIOVR4 = PI / 4.f;
constexpr float PIOVR6 = PI / 6.f;

constexpr float EPSILON = 0.0001f;

constexpr float SQRT2 = 1.414213562373f; // std::sqrtf(2.f);
constexpr float SQRT2OVR2 = SQRT2 / 2.f; // std::sqrtf(2.f) / 2.f;

constexpr float INF = std::numeric_limits<float>::infinity();

inline bool Equals(float a, float b)
{
    if (std::isinf(a) || std::isinf(b))
    {
        return a == b;
    }
    return std::abs(a - b) < EPSILON;
}

template<typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, T>
Clamp(T value, T min, T max)
{
    return std::max(min, std::min(max, value));
}

inline bool SolveQuadratic(float a, float b, float c, float& r1, float& r2)
{
    auto discriminant = (b * b) - (4.f * a * c);
    if (std::abs(discriminant) < EPSILON)
    {
        // Discriminant is 0, then we have 2 equal roots
        r1 = r2 = -b / (2.f * a);
        return true;
    }

    // Test for complex roots
    if (discriminant < 0.f) return false;

    // Discriminant is positive, then we have two real and different roots
    auto const sqrtD = std::sqrtf(discriminant);
    auto const den = 1.f / (2.f * a);
    r1 = (-b - sqrtD) * den;
    r2 = (-b + sqrtD) * den;
    if (r1 > r2) std::swap(r1, r2);
    return true;
}
