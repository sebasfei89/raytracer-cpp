#pragma once

#include "Material.h"
#include "Matrix.h"
#include "Intersection.h"
#include "raytracer_export.h"

#include <memory>
#include <vector>

class Ray;

class Shape : public std::enable_shared_from_this<Shape>
{
public:
    RAYTRACER_EXPORT Shape();

    void SetTransform(Mat44 const& t) { m_transform = t; }
    Mat44 const& Transform() const { return m_transform; }

    Material& ModifyMaterial() { return m_material; }
    Material const& GetMaterial() const { return m_material; }
    void SetMaterial(Material const& material) { m_material = material; }

    RAYTRACER_EXPORT Tuple NormalAt(Tuple const& point) const;

    // point is in shape's local space
    virtual Tuple NormalAtLocal(Tuple const& point) const = 0;

    // ray is in shape's local space
    virtual std::vector<Intersection> Intersect(Ray const& ray) const = 0;

    RAYTRACER_EXPORT virtual bool operator==(Shape const& other) const;

private:
    Mat44 m_transform;
    Material m_material;
};
