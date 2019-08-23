#include "Sphere.h"
#include "Ray.h"

Tuple Sphere::NormalAtLocal(Tuple const& localPoint) const
{
    return { localPoint.X(), localPoint.Y(), localPoint.Z(), 0.f };
}

void Sphere::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    auto const& rayDir = ray.Direction();
    auto sphereToRay = ray.Origin();
    sphereToRay.W() = 0.f;
    float const a = rayDir.Dot(rayDir);
    float const b = 2.f * rayDir.Dot(sphereToRay);
    float const c = sphereToRay.Dot(sphereToRay) - 1.f;

    float t1, t2;
    if (!SolveQuadratic(a, b, c, t1, t2)) return;

    xs.push_back({ t1, shared_from_this() });
    xs.push_back({ t2, shared_from_this() });
}

bool Sphere::operator==(Shape const& other) const
{
    auto otherSphere = dynamic_cast<Sphere const*>(&other);
    return otherSphere && Shape::operator==(other);
}
