#include "Shapes/Cube.h"

#include "Ray.h"
#include "Shapes/ShapeFactory.h"

REGISTER_SHAPE(Cube);

void Cube::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    float t1, t2;
    if (GetBounds().Intersects(ray, t1, t2))
    {
        xs.push_back({ t1, shared_from_this() });
        xs.push_back({ t2, shared_from_this() });
    }
}

Tuple Cube::NormalAtLocal(Tuple const& point) const
{
    auto const absX = std::abs(point.X());
    auto const absY = std::abs(point.Y());
    auto const absZ = std::abs(point.Z());
    auto const maxc = std::max(absX, std::max(absY, absZ));
    if (maxc == absX)
    {
        return Vector(point.X(), 0.f, 0.f);
    }
    else if (maxc == absY)
    {
        return Vector(0.f, point.Y(), 0.f);
    }

    return Vector(0.f, 0.f, point.Z());
}
