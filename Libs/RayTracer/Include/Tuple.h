#pragma once

#include "raytracer_export.h"

#include <ostream>

class RAYTRACER_EXPORT Tuple
{
public:
    Tuple();
    Tuple(float x, float y, float z, float w);

    float operator[](size_t index) const;
    float& operator[](size_t index);

    bool operator==(Tuple const& other) const;
    bool operator!=(Tuple const& other) const;

    bool IsPoint() const;
    bool IsVector() const;

    float Length() const;

    void Normalize();
    Tuple Normalized() const;

    float Dot(Tuple const& other) const;
    Tuple Cross(Tuple const& other) const;

    Tuple Reflect(Tuple const& other) const;

private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

RAYTRACER_EXPORT Tuple Point(float x, float y, float z);
RAYTRACER_EXPORT Tuple Vector(float x, float y, float z);

RAYTRACER_EXPORT Tuple operator+(Tuple const& a, Tuple const& b);
RAYTRACER_EXPORT Tuple operator-(Tuple const& a, Tuple const& b);
RAYTRACER_EXPORT Tuple operator*(Tuple const& a, Tuple const& b);
RAYTRACER_EXPORT Tuple operator*(Tuple const& t, float s);
RAYTRACER_EXPORT Tuple operator/(Tuple const& t, float s);
RAYTRACER_EXPORT Tuple operator-(Tuple const& t);

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, Tuple const& t);
