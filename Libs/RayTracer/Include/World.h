#pragma once

#include "raytracer_export.h"

#include "Color.h"
#include "Intersection.h"
#include "Lighting.h"
#include "Sphere.h"
#include "Transformations.h"

class Ray;

class World
{
public:
    std::vector<Sphere> const& Objects() const { return m_objects; }
    std::vector<Sphere>& ModifyObjects() { return m_objects; }

    std::vector<PointLight> const& Lights() const { return m_lights; }
    std::vector<PointLight>& ModifyLights() { return m_lights; }

    void Add(Sphere const& s) { m_objects.push_back(s); }
    void Add(PointLight const& s) { m_lights.push_back(s); }

    RAYTRACER_EXPORT Color ShadeHit(IntersectionData const& data) const;
    RAYTRACER_EXPORT Color ColorAt(Ray const& r) const;

    RAYTRACER_EXPORT bool IsShadowed(Tuple const& point, PointLight const& light) const;

private:
    std::vector<Sphere> m_objects;
    std::vector<PointLight> m_lights;
};

RAYTRACER_EXPORT inline World DefaultWorld()
{
    World w;
    Sphere s1, s2;

    s1.ModifyMaterial().SetColor(Color(.8f, 1.f, .6f));
    s1.ModifyMaterial().Diffuse(.7f);
    s1.ModifyMaterial().Specular(.2f);
    w.Add(s1);

    s2.SetTransform(matrix::Scaling(.5f, .5f, .5f));
    w.Add(s2);

    PointLight l(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.Add(l);

    return w;
}
