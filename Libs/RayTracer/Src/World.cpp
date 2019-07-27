#include "World.h"

#include "Lighting.h"
#include "Ray.h"

Color World::ShadeHit(IntersectionData const& data, uint8_t remaining) const
{
    Color color(0.f, 0.f, 0.f);
    for (auto const& light : m_lights)
    {
        bool isInShadow = IsShadowed(data.m_overPoint, light);
        color = color + Lighting(data.m_object->GetMaterial(), data.m_object, light,
            data.m_overPoint, data.m_eyev, data.m_normalv, isInShadow);
    }
    Color const reflected = ReflectedColor(data, remaining);
    Color const refracted = RefractedColor(data, remaining);

    auto const& material = data.m_object->GetMaterial();
    if ((material.Reflective() > 0.f) && (material.Transparency() > 0.f))
    {
        auto const reflectance = Schlick(data);
        return color
            + (reflected * reflectance)
            + (refracted * (1.f - reflectance));
    }

    return color + reflected + refracted;
}

Color World::ColorAt(Ray const& r, uint8_t remaining) const
{
    std::vector<Intersection> xs;
    xs.reserve(10);

    r.Intersect(*this, xs);
    auto const i = Hit(xs);
    if (i.Object() == nullptr)
    {
        return Color(0.f, 0.f, 0.f);
    }

    auto const iData = r.Precompute(i, xs);
    return ShadeHit(iData, remaining);
}

bool World::IsShadowed(Tuple const& point, PointLight const& light) const
{
    auto const pointToLight = light.Position() - point;
    auto const distanceToLight = pointToLight.Length();
    auto const rayToLight = Ray(point, pointToLight.Normalized());

    return rayToLight.HasIntersectionNearThan(*this, distanceToLight);
}

Color World::ReflectedColor(IntersectionData const& data, uint8_t remaining) const
{
    float const reflective = data.m_object->GetMaterial().Reflective();
    if ((reflective == 0.f) || (remaining < 1))
    {
        return { 0.f, 0.f, 0.f };
    }

    Ray const reflectedRay(data.m_overPoint, data.m_reflectv);
    return ColorAt(reflectedRay, remaining - 1) * reflective;
}

Color World::RefractedColor(IntersectionData const& data, uint8_t maxRecursion) const
{
    float const transparency = data.m_object->GetMaterial().Transparency();
    if (transparency == 0.f || maxRecursion < 1)
    {
        return Color::Black();
    }

    // Check for total internal reflection!
    float const nRatio = data.m_n1 / data.m_n2;
    float const cosI = data.m_eyev.Dot(data.m_normalv);
    float const sin2T = (nRatio * nRatio) * (1.f - (cosI * cosI));
    if (sin2T > 1.f)
    {
        return Color::Black();
    }

    float const cosT = std::sqrtf(1.f - sin2T);
    auto const direction = (data.m_normalv * ((nRatio * cosI) - cosT)) - (data.m_eyev * nRatio);
    Ray const refractedRay(data.m_underPoint, direction);

    return ColorAt(refractedRay, maxRecursion - 1) * transparency;
}
