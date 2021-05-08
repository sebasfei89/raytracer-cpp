#include "Shapes/Cone.h"

#include "Ray.h"
#include "Shapes/ShapeFactory.h"

REGISTER_SHAPE(Cone);

Cone::Cone()
    : Cone(-INF, INF, false)
{
}

Cone::Cone(float min, float max, bool closed)
    : Cylinder(min, max, closed)
{
    auto& bounds = ModifyBounds();
    auto const maxAbs = std::max(std::abs(min), std::abs(max));
    bounds.Min(Point(-maxAbs, min, -maxAbs));
    bounds.Max(Point(maxAbs, max, maxAbs));
}

float Cone::A(Ray const& ray) const
{
    auto const& d = ray.Direction();
    return (d.X() * d.X()) - (d.Y() * d.Y()) + (d.Z() * d.Z());
}

float Cone::B(Ray const& ray) const
{
    auto const& d = ray.Direction();
    auto const& o = ray.Origin();
    return 2.f * ((o.X() * d.X()) - (o.Y() * d.Y()) + (o.Z() * d.Z()));
}

float Cone::C(Ray const& ray) const
{
    auto const& o = ray.Origin();
    return (o.X() * o.X()) - (o.Y() * o.Y()) + (o.Z() * o.Z());
}

void Cone::EarlyTest(Ray const& ray, std::vector<Intersection>& xs) const
{
    Ray const normalizedRay(ray.Origin(), ray.Direction().Normalized());
    float const b = B(normalizedRay);
    if (std::abs(b) >= EPSILON)
    {
        float const localDistance = -C(normalizedRay) / (2.f * b);
        if (IsInRange(normalizedRay, localDistance))
        {
            xs.push_back({ -C(ray) / (2.f * B(ray)), shared_from_this() });
        }
    }
}

float Cone::CalculateNormalY(float y, float d) const
{
    float const t = sqrtf(d);
    return (y > 0.f) ? -t : t;
}

void Cone::UpdateBounds()
{
    auto& bounds = ModifyBounds();
    auto const maxAbs = std::max(std::abs(Minimum()), std::abs(Maximum()));
    bounds.Min(Point(-maxAbs, Minimum(), -maxAbs));
    bounds.Max(Point(maxAbs, Maximum(), maxAbs));
}
