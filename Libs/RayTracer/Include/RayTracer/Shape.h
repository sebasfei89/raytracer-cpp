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

    void SetTransform(Mat44 const& t) { m_transform = t; m_invTransform = t.Inverse(); }
    Mat44 const& Transform() const { return m_transform; }
    Mat44 const& InvTransform() const { return m_invTransform; }

    Material& ModifyMaterial() { return m_material; }
    Material const& GetMaterial() const { return m_material; }
    void SetMaterial(Material const& material) { m_material = material; }

    void CastShadows(bool castShadows) { m_castShadows = castShadows; }
    bool CastShadows() const { return m_castShadows; }

    RAYTRACER_EXPORT Tuple NormalAt(Tuple const& point) const;

    // point is in shape's local space
    virtual Tuple NormalAtLocal(Tuple const& point) const = 0;

    // ray is in shape's local space
    virtual void Intersect(Ray const& ray, std::vector<Intersection>& xs) const = 0;

    // ray is in shape's local space
    virtual bool IntersectsBefore(Ray const& ray, float distance) const = 0;

    RAYTRACER_EXPORT virtual bool operator==(Shape const& other) const;

private:
    Mat44 m_transform;
    Mat44 m_invTransform;
    Material m_material;
    bool m_castShadows;
};
