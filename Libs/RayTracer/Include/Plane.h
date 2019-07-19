#pragma once

#include "raytracer_export.h"
#include "Shape.h"

class Plane : public Shape
{
public:
    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;

    RAYTRACER_EXPORT std::vector<Intersection> Intersect(Ray const& ray) const override;
};
