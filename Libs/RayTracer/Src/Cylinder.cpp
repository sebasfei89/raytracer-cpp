#include "Cylinder.h"

#include "Ray.h"

#include <numeric>
#include <utility>

namespace
{

bool CheckCap(Ray const& ray, float t, float r)
{
    auto const x = ray.Origin().X() + (t * ray.Direction().X());
    auto const z = ray.Origin().Z() + (t * ray.Direction().Z());
    return ((x * x) + (z * z)) < (r + EPSILON);
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
    auto const a = A(ray);
    if (std::abs(a) < EPSILON)
    {
        EarlyTest(ray, a, xs);
        IntersectCaps(ray, xs);
        return;
    }

    auto const b = B(ray);
    auto const c = C(ray);
    auto d = (b * b) - (4.f * a * c);
    if (d < 0)
    {
        if (d < -EPSILON)
        {
            // Ray does not intersect the cylinder
            return;
        }
        else
        {
            // Avoid presicion issues in calculation
            d = 0.f;
        }
    }

    auto const sqrtD = std::sqrtf(d);
    auto const den = 1.f / (2.f * a);
    auto t1 = (-b - sqrtD) * den;
    auto t2 = (-b + sqrtD) * den;

    if (t1 > t2)
    {
        std::swap(t1, t2);
    }

    auto const y0 = ray.Origin().Y() + t1 * ray.Direction().Y();
    if (m_min < y0 && y0 < m_max)
    {
        xs.push_back({ t1, shared_from_this() });
    }

    auto const y1 = ray.Origin().Y() + t2 * ray.Direction().Y();
    if (m_min < y1 && y1 < m_max)
    {
        xs.push_back({ t2, shared_from_this() });
    }

    IntersectCaps(ray, xs);
}

bool Cylinder::IntersectsBefore(Ray const& ray, float distance) const
{
    auto const a = A(ray);
    auto const b = B(ray);
    auto const c = C(ray);

    if (std::abs(a) < EPSILON)
    {
        // Ray is parallel to the y axis
        return EarlyTestBefore(ray, a, distance) || IntersectsCapsBefore(ray, distance);
    }

    auto d = (b * b) - (4.f * a * c);
    if (d < 0.f)
    {
        if (d < -EPSILON)
        {
            // Ray does not intersect the cylinder
            return false;
        }
        else
        {
            // Avoid presicion issues in calculation
            d = 0.f;
        }
    }

    auto const sqrtD = std::sqrtf(d);
    auto const den = 1.f / (2.f * a);
    auto const t1 = (-b - sqrtD) * den;
    auto const t2 = (-b + sqrtD) * den;

    if (((t1 >= EPSILON) && (t1 < distance)) || ((t2 >= EPSILON) && (t2 < distance)))
    {
        return true;
    }

    return IntersectsCapsBefore(ray, distance);
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

bool Cylinder::IntersectsCapsBefore(Ray const& ray, float distance) const
{
    if (!m_closed || (std::abs(ray.Direction().Y()) < EPSILON))
    {
        return false;
    }

    auto t = (m_min - ray.Origin().Y()) / ray.Direction().Y();
    if (CheckCap(ray, t, RadiusAt(m_min)) && (t >= EPSILON) && (t < distance))
    {
        return true;
    }

    t = (m_max - ray.Origin().Y()) / ray.Direction().Y();
    if (CheckCap(ray, t, RadiusAt(m_max)) && (t >= EPSILON) && (t < distance))
    {
        return true;
    }

    return false;
}

float Cylinder::A(Ray const& ray) const
{
    return (ray.Direction().X() * ray.Direction().X())
        + (ray.Direction().Z() * ray.Direction().Z());
}

float Cylinder::B(Ray const& ray) const
{
    return (2.f * ray.Origin().X() * ray.Direction().X())
        + (2.f * ray.Origin().Z() * ray.Direction().Z());
}

float Cylinder::C(Ray const& ray) const
{
    return (ray.Origin().X() * ray.Origin().X())
        + (ray.Origin().Z() * ray.Origin().Z()) - 1.f;
}
