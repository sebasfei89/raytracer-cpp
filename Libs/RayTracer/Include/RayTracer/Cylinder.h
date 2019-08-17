#pragma once

#include "raytracer_export.h"
#include "Shape.h"

#include <numeric>

class Cylinder : public Shape
{
public:
    RAYTRACER_EXPORT Cylinder();
    RAYTRACER_EXPORT Cylinder(float min, float max, bool closed = false);

    RAYTRACER_EXPORT void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override;
    RAYTRACER_EXPORT bool IntersectsBefore(Ray const& ray, float distance) const override;

    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;

    float Minimum() const { return m_min; }
    float Maximum() const { return m_max; }

    bool Closed() const { return m_closed; }

protected:
    void IntersectCaps(Ray const& ray, std::vector<Intersection>& xs) const;
    bool IntersectsCapsBefore(Ray const& ray, float distance) const;

    virtual float A(Ray const& ray) const;
    virtual float B(Ray const& ray) const;
    virtual float C(Ray const& ray) const;
    virtual float RadiusAt(float y) const { return 1.f; }
    virtual float CalculateNormalY(float y, float d) const { return 0.f; }

    virtual void EarlyTest(Ray const& ray, float a, std::vector<Intersection>& xs) const {}
    virtual bool EarlyTestBefore(Ray const& ray, float a, float distance) const { return false; }

private:
    float m_min;
    float m_max;
    bool m_closed;
};
