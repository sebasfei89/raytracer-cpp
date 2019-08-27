#pragma once

#include "raytracer_export.h"
#include "Shape.h"

class Cube : public Shape
{
public:
    RAYTRACER_EXPORT void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override;
    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;
};
