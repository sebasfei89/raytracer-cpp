#pragma once

#include "raytracer_export.h"
#include "Shape.h"

class Plane : public Shape
{
public:
    RAYTRACER_EXPORT Plane();

    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;
    RAYTRACER_EXPORT void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override;
};
