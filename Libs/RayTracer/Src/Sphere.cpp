#include "Sphere.h"
#include "Ray.h"

Tuple Sphere::NormalAtLocal(Tuple const& localPoint) const
{
    return { localPoint.X(), localPoint.Y(), localPoint.Z(), 0.f };
}

void Sphere::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    auto sphereToRay = ray.Origin();
    sphereToRay.W() = 0.f;
    float const a = ray.Direction().Dot(ray.Direction());
    float const b = 2.f * ray.Direction().Dot(sphereToRay);
    float const c = sphereToRay.Dot(sphereToRay) - 1.f;
    float const discriminant = (b * b) - (4.f * a * c);

    if (discriminant < 0.f)
    {
        return;
    }

    float const aTimes2 = 1.f / (2.f * a);
    float const sqrtDiscriminant = sqrtf(discriminant) * aTimes2;
    float const t = -b * aTimes2;
    xs.push_back({ t - sqrtDiscriminant, shared_from_this() });
    xs.push_back({ t + sqrtDiscriminant, shared_from_this() });
}

bool Sphere::operator==(Shape const& other) const
{
    auto otherSphere = dynamic_cast<Sphere const*>(&other);
    return otherSphere && Shape::operator==(other);
}
