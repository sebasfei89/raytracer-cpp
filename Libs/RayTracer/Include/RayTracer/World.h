#pragma once

#include "raytracer_export.h"

#include "Color.h"
#include "Intersection.h"
#include "Lighting.h"
#include "Shapes/Shape.h"
#include "Transformations.h"

#include <istream>
#include <map>
#include <vector>

class Ray;

class World
{
public:
    RAYTRACER_EXPORT bool Load(std::istream& is);

    std::vector<ShapePtr> const& Objects() const { return m_objects; }
    std::vector<ShapePtr>& ModifyObjects() { return m_objects; }

    std::vector<PointLight> const& Lights() const { return m_lights; }
    std::vector<PointLight>& ModifyLights() { return m_lights; }

    ArchetypeMap const& Archetypes() const { return m_archetypes; }

    RAYTRACER_EXPORT void Add(ArchetypePtr const& a);
    void Add(ShapePtr const& s) { m_objects.push_back(s); }
    void Add(PointLight const& l) { m_lights.push_back(l); }

    RAYTRACER_EXPORT Color ShadeHit(IntersectionData const& data, uint8_t remaining = 4u) const;
    RAYTRACER_EXPORT Color ReflectedColor(IntersectionData const& data, uint8_t maxRecursion = 4u) const;
    RAYTRACER_EXPORT Color RefractedColor(IntersectionData const& data, uint8_t maxRecursion = 4u) const;
    RAYTRACER_EXPORT Color ColorAt(Ray const& r, uint8_t remaining = 4u) const;

    RAYTRACER_EXPORT bool IsShadowed(Tuple const& point, PointLight const& light) const;

protected:
    friend class Loader;

    ArchetypeConstPtr FindArchetype(std::string const& name) const;
    bool LoadArchetypes(std::vector<json> const& archetypes);
    bool LoadLight(json const& data);

private:
    std::vector<ShapePtr>   m_objects;
    std::vector<PointLight> m_lights;
    ArchetypeMap            m_archetypes;
};
