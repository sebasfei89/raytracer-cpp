#pragma once

#include "raytracer_export.h"
#include "Shape.h"

class Sphere : public Shape
{
public:
    Tuple Center() const { return Point(0.f, 0.f, 0.f); }

    RAYTRACER_EXPORT bool operator==(Shape const& other) const override;

    RAYTRACER_EXPORT std::vector<Intersection> Intersect(Ray const& ray) const override;

    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;
};
