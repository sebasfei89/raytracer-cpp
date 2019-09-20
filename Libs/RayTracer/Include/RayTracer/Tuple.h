#pragma once

#include "raytracer_export.h"

#include <nlohmann/json.hpp>

#include <ostream>

using nlohmann::json;

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

    float X() const { return m_x; }
    float Y() const { return m_y; }
    float Z() const { return m_z; }
    float W() const { return m_w; }

    float& X() { return m_x; }
    float& Y() { return m_y; }
    float& Z() { return m_z; }
    float& W() { return m_w; }

private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

RAYTRACER_EXPORT Tuple Point(float x, float y, float z);
RAYTRACER_EXPORT Tuple Vector(float x, float y, float z);

#define APPLY_OPERATOR(a, b, op) { a.X() op b.X(), a.Y() op b.Y(), a.Z() op b.Z(), a.W() op b.W() }
inline Tuple operator+(Tuple const& a, Tuple const& b) { return APPLY_OPERATOR(a, b, +); }
inline Tuple operator-(Tuple const& a, Tuple const& b) { return APPLY_OPERATOR(a, b, -); }
inline Tuple operator*(Tuple const& a, Tuple const& b) { return APPLY_OPERATOR(a, b, *); }
#undef APPLY_OPERATOR

inline Tuple operator*(Tuple const& t, float s) { return { t.X() * s, t.Y() * s, t.Z() * s, t.W() * s }; }
inline Tuple operator/(Tuple const& t, float s) { return t * (1.f / s); }
inline Tuple operator-(Tuple const& t) { return { -t.X(), -t.Y(), -t.Z(), -t.W() }; }

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, Tuple const& t);

RAYTRACER_EXPORT void to_json(json& j, Tuple const& t);
RAYTRACER_EXPORT void from_json(json const& j, Tuple& t);
