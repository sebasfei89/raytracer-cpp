#include "Sphere.h"
#include "Ray.h"

Tuple Sphere::NormalAtLocal(Tuple const& localPoint) const
{
    return localPoint - Center();
}

std::vector<Intersection> Sphere::Intersect(Ray const& ray) const
{
    auto const sphereToRay = ray.Origin() - Center();
    float const a = ray.Direction().Dot(ray.Direction());
    float const b = 2.f * ray.Direction().Dot(sphereToRay);
    float const c = sphereToRay.Dot(sphereToRay) - 1.f;
    float const discriminant = (b * b) - (4.f * a * c);

    if (discriminant < 0.f)
    {
        return {};
    }

    float const aTimes2 = 1.f / (2.f * a);
    float const sqrtDiscriminant = sqrtf(discriminant);

    return {
        { (-b - sqrtDiscriminant) * aTimes2, shared_from_this() },
        { (-b + sqrtDiscriminant) * aTimes2, shared_from_this() }
    };
}

bool Sphere::operator==(Shape const& other) const
{
    auto otherSphere = dynamic_cast<Sphere const*>(&other);
    return otherSphere && Shape::operator==(other);
}
