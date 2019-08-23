#include "Cube.h"
#include "Ray.h"

void Cube::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    auto[xtMin, xtMax] = CheckAxis(ray.Origin().X(), ray.Direction().X());
    auto[ytMin, ytMax] = CheckAxis(ray.Origin().Y(), ray.Direction().Y());
    auto[ztMin, ztMax] = CheckAxis(ray.Origin().Z(), ray.Direction().Z());

    float const tMin = std::max(xtMin, std::max(ytMin, ztMin));
    float const tMax = std::min(xtMax, std::min(ytMax, ztMax));

    if (tMin <= tMax)
    {
        xs.push_back({ tMin, shared_from_this() });
        xs.push_back({ tMax, shared_from_this() });
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

std::tuple<float, float> Cube::CheckAxis(float origin, float direction) const
{
    float const oneOverDir = 1.f / direction;
    float const tMin = (-1.f - origin) * oneOverDir;
    float const tMax = (1.f - origin) * oneOverDir;
    return (tMin > tMax) ? std::tie(tMax, tMin) : std::tie(tMin, tMax);
}
