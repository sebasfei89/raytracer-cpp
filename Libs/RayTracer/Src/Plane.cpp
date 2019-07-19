#include "Plane.h"
#include "Ray.h"
#include "Util.h"

Tuple Plane::NormalAtLocal(Tuple const& point) const
{
    return Vector(0.f, 1.f, 0.f);
}

std::vector<Intersection> Plane::Intersect(Ray const& ray) const
{
    float const yDirection = ray.Direction()[1];
    if (std::abs(yDirection) < EPSILON)
    {
        return {};
    }

    return { {(-ray.Origin()[1]) / yDirection, shared_from_this() } };
}
