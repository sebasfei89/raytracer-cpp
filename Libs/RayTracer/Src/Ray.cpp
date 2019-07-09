#include "Ray.h"

Ray::Ray(Tuple const& origin, Tuple const& direction)
    : m_origin(origin)
    , m_direction(direction)
{
}

std::vector<Intersection> Ray::Intersect(Sphere const& sphere) const
{
    Ray r = sphere.Transform().Inverse() * (*this);
    auto const sphereToRay = r.m_origin - sphere.Center();
    float const a = r.m_direction.Dot(r.m_direction);
    float const b = -2.f * r.m_direction.Dot(sphereToRay);
    float const c = sphereToRay.Dot(sphereToRay) - 1.f;
    float const discriminant = (b * b) - (4.f * a * c);

    if (discriminant < 0.f) return {};

    float const aTimes2 = 2.f * a;
    float const sqrtDiscriminant = sqrt(discriminant);

    return {
        { (b - sqrtDiscriminant) / aTimes2, &sphere },
        { (b + sqrtDiscriminant) / aTimes2, &sphere }
    };
}

Ray operator*(Mat44 const& m, Ray const& r)
{
    return Ray{ m * r.Origin(), m * r.Direction() };
}
