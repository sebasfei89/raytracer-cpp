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

    void Intersect(ShapePtr const& shape, std::vector<Intersection>& xs) const;
    void Intersect(World const& world, std::vector<Intersection>& xs) const;

    bool HasIntersectionNearThan(World const& world, float distance) const;
    bool IntersectsBefore(ShapePtr const& shape, float distance) const;

    IntersectionData Precompute(Intersection const& i, std::vector<Intersection> const& xs = {}) const;

private:
    Tuple m_origin;
    Tuple m_direction;
};

RAYTRACER_EXPORT Ray operator*(Mat44 const& m, Ray const& r);