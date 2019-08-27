#pragma once

#include "Bounds.h"
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

    Bounds const& GetBounds() const { return m_bounds; }

    RAYTRACER_EXPORT void SetTransform(Mat44 const& t);
    Mat44 const& Transform() const { return m_transform; }
    Mat44 const& InvTransform() const { return m_invTransform; }

    ShapePtr Parent() { return m_parent; }
    void SetParent(ShapePtr parent) { m_parent = parent; }

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
    RAYTRACER_EXPORT bool IntersectsBefore(Ray const& ray, float distance) const;

    RAYTRACER_EXPORT virtual bool operator==(Shape const& other) const;

    RAYTRACER_EXPORT Tuple WorldToLocal(Tuple const& point) const;
    RAYTRACER_EXPORT Tuple NormalToWorld(Tuple const& normal) const;

protected:
    Bounds& ModifyBounds() { return m_bounds; }
    RAYTRACER_EXPORT virtual void UpdateBounds();

private:
    Bounds m_bounds;
    Mat44 m_transform;
    Mat44 m_invTransform;
    Material m_material;
    bool m_castShadows;
    std::shared_ptr<Shape> m_parent;
};
