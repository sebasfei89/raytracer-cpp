#include "World.h"

#include "Lighting.h"
#include "Ray.h"

Color World::ShadeHit(IntersectionData const& data) const
{
    Color color(0.f, 0.f, 0.f);
    for (auto const& light : m_lights)
    {
        bool isInShadow = IsShadowed(data.m_overPoint, light);
        color = color + Lighting(data.m_object->GetMaterial(), light,
            data.m_overPoint, data.m_eyev, data.m_normalv, isInShadow);
    }
    return color;
}

Color World::ColorAt(Ray const& r) const
{
    std::vector<Intersection> xs;
    xs.reserve(10);

    r.Intersect(*this, xs);
    auto const i = Hit(xs);
    if (i.Object() == nullptr)
    {
        return Color(0.f, 0.f, 0.f);
    }

    auto const iData = r.Precompute(i);
    return ShadeHit(iData);
}

bool World::IsShadowed(Tuple const& point, PointLight const& light) const
{
    auto const pointToLight = light.Position() - point;
    auto const distanceToLight = pointToLight.Length();
    auto const rayToLight = Ray(point, pointToLight.Normalized());
    std::vector<Intersection> xs;
    xs.reserve(10);
    rayToLight.Intersect(*this, xs);
    auto const hit = Hit(xs);
    if (hit.Object() == nullptr)
    {
        return false;
    }

    return hit.Distance() < distanceToLight;
}
