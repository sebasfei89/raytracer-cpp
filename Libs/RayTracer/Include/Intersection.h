#pragma once

#include "raytracer_export.h"
#include "Matrix.h"
#include "Tuple.h"
#include "Types.h"

#include <ostream>
#include <vector>

struct IntersectionData
{
    Tuple m_point;
    Tuple m_overPoint;
    Tuple m_underPoint;
    Tuple m_eyev;
    Tuple m_normalv;
    Tuple m_reflectv;
    float m_n1;
    float m_n2;
    float m_distance;
    bool m_inside;
    ShapeConstPtr m_object;
};

class Intersection
{
public:
    RAYTRACER_EXPORT Intersection(float distance, ShapeConstPtr const& s);

    float Distance() const { return m_distance; }
    ShapeConstPtr Object() const { return m_shape; }

    RAYTRACER_EXPORT bool operator==(Intersection const& other) const;
    RAYTRACER_EXPORT bool operator<(Intersection const& other) const;

private:
    float m_distance;
    ShapeConstPtr m_shape;
};

RAYTRACER_EXPORT Intersection Hit(std::vector<Intersection> const& xs);

template<typename ...Args>
std::vector<Intersection> Intersections(Args const&... args)
{
    std::vector<Intersection> xs{ args... };
    std::sort(xs.begin(), xs.end());
    return xs;
}

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, Intersection const& i);
