#pragma once

#include "Material.h"
#include "Matrix.h"
#include "raytracer_export.h"

class Sphere
{
public:
    RAYTRACER_EXPORT Sphere();

    Tuple Center() const { return Point(0.f, 0.f, 0.f); }
    RAYTRACER_EXPORT Tuple NormalAt(Tuple const& point) const;

    void SetTransform(Mat44 const& t) { m_transform = t; }
    Mat44 const& Transform() const { return m_transform; }

    Material& ModifyMaterial() { return m_material; }
    Material const& GetMaterial() const { return m_material; }
    void SetMaterial(Material const& material) { m_material = material; }

    RAYTRACER_EXPORT bool operator==(Sphere const& other) const;

private:
    Mat44 m_transform;
    Material m_material;
};
