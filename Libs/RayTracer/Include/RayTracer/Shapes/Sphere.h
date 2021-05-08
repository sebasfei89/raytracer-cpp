#pragma once

#include "Shape.h"

#include "raytracer_export.h"

class Sphere : public Shape
{
public:
    RAYTRACER_EXPORT bool operator==(Shape const& other) const override;

    RAYTRACER_EXPORT void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override;
    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;
};
