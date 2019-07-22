#include "Plane.h"
#include "Ray.h"
#include "Util.h"

Tuple Plane::NormalAtLocal(Tuple const& point) const
{
    return Vector(0.f, 1.f, 0.f);
}

void Plane::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    float const yDirection = ray.Direction().Y();
    if (std::abs(yDirection) < EPSILON)
    {
        return;
    }
    xs.push_back({ (-ray.Origin().Y()) / yDirection, shared_from_this() });
}

bool Plane::IntersectsBefore(Ray const& ray, float distance) const
{
    float const yDirection = ray.Direction().Y();
    if (std::abs(yDirection) < EPSILON)
    {
        return false;
    }

    float const r = -ray.Origin().Y() / yDirection;
    return (r >= EPSILON) && (r < distance);
}
