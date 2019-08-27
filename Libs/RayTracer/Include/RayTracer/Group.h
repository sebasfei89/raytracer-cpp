#pragma once

#include "Shape.h"
#include "raytracer_export.h"

#include <vector>

class Group : public Shape
{
public:
    RAYTRACER_EXPORT void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override;
    RAYTRACER_EXPORT Tuple NormalAtLocal(Tuple const& point) const override;

    bool IsEmpty() const { return m_children.empty(); }

    RAYTRACER_EXPORT void AddChild(ShapePtr child);
    RAYTRACER_EXPORT bool Includes(ShapePtr child) const;

private:
    std::vector<ShapePtr> m_children;
};
