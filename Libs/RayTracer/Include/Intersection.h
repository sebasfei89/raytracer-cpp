#pragma once

#include "raytracer_export.h"
#include "Matrix.h"
#include "Tuple.h"

#include <ostream>
#include <vector>

class Sphere
{
public:
    Sphere() : m_transform(Mat44::Identity()) {}

    Tuple Center() const { return Point(0.f, 0.f, 0.f); }

    void SetTransform(Mat44 const& t) { m_transform = t; }
    Mat44 const& Transform() const { return m_transform; }

private:
    Mat44 m_transform;
};

class Intersection
{
public:
    RAYTRACER_EXPORT Intersection(float distance, Sphere const* s);

    float Distance() const { return m_distance; }
    Sphere const* Object() const { return m_object; }

    RAYTRACER_EXPORT bool operator==(Intersection const& other) const;

private:
    float m_distance;
    Sphere const* m_object;
};

RAYTRACER_EXPORT Intersection Hit(std::vector<Intersection> const& xs);

template<typename ...Args>
std::vector<Intersection> Intersections(Args const&... args)
{
    return { args... };
}

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, Intersection const& i);
