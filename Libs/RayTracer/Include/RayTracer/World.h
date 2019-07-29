#pragma once

#include "raytracer_export.h"

#include "Color.h"
#include "Intersection.h"
#include "Lighting.h"
#include "Shape.h"
#include "Transformations.h"

class Ray;

class World
{
public:
    std::vector<ShapePtr> const& Objects() const { return m_objects; }
    std::vector<ShapePtr>& ModifyObjects() { return m_objects; }

    std::vector<PointLight> const& Lights() const { return m_lights; }
    std::vector<PointLight>& ModifyLights() { return m_lights; }

    void Add(ShapePtr const& s) { m_objects.push_back(s); }
    void Add(PointLight const& s) { m_lights.push_back(s); }

    RAYTRACER_EXPORT Color ShadeHit(IntersectionData const& data, uint8_t remaining = 4u) const;
    RAYTRACER_EXPORT Color ReflectedColor(IntersectionData const& data, uint8_t maxRecursion = 4u) const;
    RAYTRACER_EXPORT Color RefractedColor(IntersectionData const& data, uint8_t maxRecursion = 4u) const;
    RAYTRACER_EXPORT Color ColorAt(Ray const& r, uint8_t remaining = 4u) const;

    RAYTRACER_EXPORT bool IsShadowed(Tuple const& point, PointLight const& light) const;

private:
    std::vector<ShapePtr> m_objects;
    std::vector<PointLight> m_lights;
};
