#pragma once

#include "raytracer_export.h"

#include "Intersection.h"
#include "Matrix.h"
#include "Tuple.h"
#include "World.h"

#include <vector>

class RAYTRACER_EXPORT Ray
{
public:
    Ray(Tuple const& origin, Tuple const& direction);

    Tuple const& Origin() const { return m_origin; }
    Tuple const& Direction() const { return m_direction; }

    Tuple Position(float t) const { return m_origin + (m_direction * t); }

    std::vector<Intersection> Intersect(Sphere const& sphere) const;
    std::vector<Intersection> Intersect(World const& world) const;

    IntersectionData Precompute(Intersection const& i) const;

private:
    Tuple m_origin;
    Tuple m_direction;
};

RAYTRACER_EXPORT Ray operator*(Mat44 const& m, Ray const& r);