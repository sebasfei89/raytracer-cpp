#include "World.h"

#include "Lighting.h"
#include "Ray.h"

Color World::ShadeHit(IntersectionData const& data) const
{
    Color color(0.f, 0.f, 0.f);
    for (auto const& light : m_lights)
    {
        color = color + Lighting(data.m_object->GetMaterial(), m_lights[0],
            data.m_point, data.m_eyev, data.m_normalv );
    }
    return color;
}

Color World::ColorAt(Ray const& r) const
{
    auto const xs = r.Intersect(*this);
    auto const i = Hit(xs);
    if (i.Object() == nullptr)
    {
        return Color(0.f, 0.f, 0.f);
    }

    auto const iData = r.Precompute(i);
    return ShadeHit(iData);
}
