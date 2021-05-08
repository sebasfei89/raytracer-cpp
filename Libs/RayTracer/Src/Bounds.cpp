#include "Bounds.h"
#include "Ray.h"

#include <vector>

Bounds operator*(Mat44 const& m, Bounds const& b)
{
    std::vector<Tuple> vertices = {
        m * Point(b.Min().X(), b.Min().Y(), b.Min().Z()),
        m * Point(b.Min().X(), b.Max().Y(), b.Min().Z()),
        m * Point(b.Max().X(), b.Min().Y(), b.Min().Z()),
        m * Point(b.Max().X(), b.Max().Y(), b.Min().Z()),
        m * Point(b.Min().X(), b.Min().Y(), b.Max().Z()),
        m * Point(b.Min().X(), b.Max().Y(), b.Max().Z()),
        m * Point(b.Max().X(), b.Min().Y(), b.Max().Z()),
        m * Point(b.Max().X(), b.Max().Y(), b.Max().Z()) };

    Tuple min = Point(INF, INF, INF);
    Tuple max = Point(-INF, -INF, -INF);
    for (auto const v : vertices)
    {
        if (v.X() < min.X()) min.X() = v.X();
        if (v.Y() < min.Y()) min.Y() = v.Y();
        if (v.Z() < min.Z()) min.Z() = v.Z();

        if (v.X() > max.X()) max.X() = v.X();
        if (v.Y() > max.Y()) max.Y() = v.Y();
        if (v.Z() > max.Z()) max.Z() = v.Z();
    }
    return Bounds(min, max);
}

std::ostream& operator<<(std::ostream& os, Bounds const& b)
{
    os << "[" << b.Min() << ", " << b.Max() << "]";
    return os;
}

Bounds::Bounds()
    : Bounds(Point(-1.f, -1.f, -1.f), Point(1.f, 1.f, 1.f))
{
}

Bounds::Bounds(Tuple const& min, Tuple const& max)
    : m_min(min)
    , m_max(max)
{
}

bool Bounds::operator==(Bounds const& other) const
{
    return m_min == other.m_min
        && m_max == other.m_max;
}

void Bounds::Merge(Bounds const& other)
{
    m_min = Point( std::min(m_min.X(), other.m_min.X())
                 , std::min(m_min.Y(), other.m_min.Y())
                 , std::min(m_min.Z(), other.m_min.Z()) );
    m_max = Point( std::max(m_max.X(), other.m_max.X())
                 , std::max(m_max.Y(), other.m_max.Y())
                 , std::max(m_max.Z(), other.m_max.Z()) );
}

bool Bounds::Intersects(Ray const& ray, float& t1, float& t2) const
{
    auto[xtMin, xtMax] = CheckAxis(ray.Origin().X(), ray.Direction().X(), m_min.X(), m_max.X());
    auto[ytMin, ytMax] = CheckAxis(ray.Origin().Y(), ray.Direction().Y(), m_min.Y(), m_max.Y());
    auto[ztMin, ztMax] = CheckAxis(ray.Origin().Z(), ray.Direction().Z(), m_min.Z(), m_max.Z());

    t1 = std::max(xtMin, std::max(ytMin, ztMin));
    t2 = std::min(xtMax, std::min(ytMax, ztMax));

    return t1 <= t2;
}

std::tuple<float, float> Bounds::CheckAxis(float origin, float direction, float min, float max) const
{
    float const oneOverDir = 1.f / direction;
    float const tMin = (min - origin) * oneOverDir;
    float const tMax = (max - origin) * oneOverDir;
    return (tMin > tMax) ? std::tie(tMax, tMin) : std::tie(tMin, tMax);
}
