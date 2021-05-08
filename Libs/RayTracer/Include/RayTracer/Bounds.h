#pragma once

#include "Matrix.h"
#include "raytracer_export.h"
#include "Tuple.h"

class Ray;

// This class represents an Axis Aligned Bounding Box (AABB)
class Bounds
{
public:
    RAYTRACER_EXPORT Bounds();
    RAYTRACER_EXPORT Bounds(Tuple const& min, Tuple const& max);

    Tuple Min() const { return m_min; }
    Tuple Max() const { return m_max; }

    void Min(Tuple const& point) { m_min = point; }
    void Max(Tuple const& point) { m_max = point; }

    RAYTRACER_EXPORT bool operator==(Bounds const& other) const;

    RAYTRACER_EXPORT void Merge(Bounds const& other);

    RAYTRACER_EXPORT bool Intersects(Ray const& ray, float& t1, float& t2) const;
    bool Intersects(Ray const& ray) const { float t1, t2; return Intersects(ray, t1, t2); }

protected:
    std::tuple<float, float> CheckAxis(float origin, float direction, float min, float max) const;

private:
    Tuple m_min;
    Tuple m_max;
};

RAYTRACER_EXPORT Bounds operator*(Mat44 const& m, Bounds const& b);

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, Bounds const& b);
