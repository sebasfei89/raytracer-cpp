#pragma once

#include "Cylinder.h"
#include "raytracer_export.h"

class Cone : public Cylinder
{
public:
    RAYTRACER_EXPORT Cone();
    RAYTRACER_EXPORT Cone(float min, float max, bool closed = false);

protected:
    float A(Ray const& ray) const override;
    float B(Ray const& ray) const override;
    float C(Ray const& ray) const override;

    float RadiusAt(float y) const override { return std::abs(y); }
    float CalculateNormalY(float y, float d) const override;
    void EarlyTest(Ray const& ray, std::vector<Intersection>& xs) const override;
};
