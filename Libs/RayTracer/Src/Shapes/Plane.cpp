#include "Shapes/Plane.h"

#include "Ray.h"
#include "Shapes/ShapeFactory.h"
#include "Util.h"

REGISTER_SHAPE(Plane);

Plane::Plane()
{
    auto& bounds = ModifyBounds();
    bounds.Min(Point(-INF, 0.f, -INF));
    bounds.Max(Point(INF, 0.f, INF));
}

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
