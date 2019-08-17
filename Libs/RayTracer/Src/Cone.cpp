#include "Cone.h"
#include "Ray.h"

Cone::Cone()
{
}

Cone::Cone(float min, float max, bool closed)
    : Cylinder(min, max, closed)
{
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
    return (2.f * o.X() * d.X()) - (2.f * o.Y() * d.Y()) + (2.f * o.Z() * d.Z());
}

float Cone::C(Ray const& ray) const
{
    auto const& o = ray.Origin();
    return (o.X() * o.X()) - (o.Y() * o.Y()) + (o.Z() * o.Z());
}

void Cone::EarlyTest(Ray const& ray, float a, std::vector<Intersection>& xs) const
{
    float const b = B(ray);
    if (std::abs(b) > EPSILON)
    {
        float const c = C(ray);
        xs.push_back({ -c / (2.f * b), shared_from_this() });
    }
}

bool Cone::EarlyTestBefore(Ray const& ray, float a, float distance) const
{
    float const b = B(ray);
    if (std::abs(b) > EPSILON)
    {
        auto const d = -C(ray) / (2.f * b);
        return (d >= EPSILON) && (d < distance);
    }
    return false;
}

float Cone::CalculateNormalY(float y, float d) const
{
    float const t = sqrtf(d);
    return (y > 0.f) ? -t : t;
}
