#pragma once

#include "raytracer_export.h"

#include <type_traits>
#include <ostream>

class RAYTRACER_EXPORT Tuple
{
public:
    Tuple(float x, float y, float z, float w);

    float operator[](size_t index) const;
    float& operator[](size_t index);

    bool operator==(Tuple const& other) const;
    bool operator!=(Tuple const& other) const;

    bool IsPoint() const;
    bool IsVector() const;

    float Length() const;

    void Normalize();

    float Dot(Tuple const& other) const;
    Tuple Cross(Tuple const& other) const;

private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

Tuple RAYTRACER_EXPORT Point(float x, float y, float z);
Tuple RAYTRACER_EXPORT Vector(float x, float y, float z);
Tuple RAYTRACER_EXPORT operator+(Tuple const& a, Tuple const& b);
Tuple RAYTRACER_EXPORT operator-(Tuple const& a, Tuple const& b);
Tuple RAYTRACER_EXPORT operator-(Tuple const& t);
Tuple RAYTRACER_EXPORT operator*(Tuple const& t, float s);
Tuple RAYTRACER_EXPORT operator/(Tuple const& t, float s);

std::ostream& operator<<(std::ostream& os, Tuple const& t)
{
    os << "[" << t[0];
    for (int i = 1; i < 4; i++)
    {
        os << ", " << t[i];
    }
    os << "]";

    return os;
}
