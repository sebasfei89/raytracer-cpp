#include "Cylinder.h"

#include "Ray.h"

#include <numeric>
#include <utility>

namespace
{

bool CheckCap(Ray const& ray, float t, float r)
{
    auto const& rayOrig = ray.Origin();
    auto const& rayDir = ray.Direction();
    auto const x = rayOrig.X() + (t * rayDir.X());
    auto const z = rayOrig.Z() + (t * rayDir.Z());
    return ((x * x) + (z * z)) <= r + EPSILON;
}

}

Cylinder::Cylinder()
    : Cylinder(-std::numeric_limits<float>::infinity()
              , std::numeric_limits<float>::infinity()
              , false)
{
}

Cylinder::Cylinder(float min, float max, bool closed)
    : m_min(min)
    , m_max(max)
    , m_closed(closed)
{
}

void Cylinder::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    Ray r(ray.Origin(), ray.Direction().Normalized());
    if (std::abs(A(r)) < EPSILON)
    {
        EarlyTest(ray, xs);
    }
    else
    {
        float t1, t2;
        if (!SolveQuadratic(A(ray), B(ray), C(ray), t1, t2)) return;

        if (IsInRange(ray, t1)) xs.push_back({ t1, shared_from_this() });
        if (IsInRange(ray, t2)) xs.push_back({ t2, shared_from_this() });
    }
    IntersectCaps(ray, xs);
}

Tuple Cylinder::NormalAtLocal(Tuple const& point) const
{
    auto const d = (point.X() * point.X()) + (point.Z() * point.Z());
    if (d < 1.f)
    {
        if (point.Y() >= (m_max - EPSILON))
        {
            return Vector(0.f, 1.f, 0.f);
        }
        else if (point.Y() <= (m_min + EPSILON))
        {
            return Vector(0.f, -1.f, 0.f);
        }
    }

    float const y = CalculateNormalY(point.Y(), d);
    return Vector(point.X(), y, point.Z());
}

void Cylinder::IntersectCaps(Ray const& ray, std::vector<Intersection>& xs) const
{
    if (!m_closed || (std::abs(ray.Direction().Y()) < EPSILON))
    {
        return;
    }

    auto t = (m_min - ray.Origin().Y()) / ray.Direction().Y();
    if (CheckCap(ray, t, RadiusAt(m_min)))
    {
        xs.push_back({ t, shared_from_this() });
    }

    t = (m_max - ray.Origin().Y()) / ray.Direction().Y();
    if (CheckCap(ray, t, RadiusAt(m_max)))
    {
        xs.push_back({ t, shared_from_this() });
    }
}

float Cylinder::A(Ray const& ray) const
{
    auto const& rayDir = ray.Direction();
    return (rayDir.X() * rayDir.X()) + (rayDir.Z() * rayDir.Z());
}

float Cylinder::B(Ray const& ray) const
{
    auto const& rayDir = ray.Direction();
    return (2.f * ray.Origin().X() * rayDir.X()) + (2.f * ray.Origin().Z() * rayDir.Z());
}

float Cylinder::C(Ray const& ray) const
{
    return (ray.Origin().X() * ray.Origin().X()) + (ray.Origin().Z() * ray.Origin().Z()) - 1.f;
}

bool Cylinder::IsInRange(Ray const& ray, float t) const
{
    auto const y = ray.Origin().Y() + t * ray.Direction().Y();
    return (m_min < y) && (y < m_max);
}
