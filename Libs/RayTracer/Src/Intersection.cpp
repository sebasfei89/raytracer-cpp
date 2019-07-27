#include "Intersection.h"

Intersection::Intersection(float distance, ShapeConstPtr const& s)
    : m_distance(distance)
    , m_shape(s)
{}

bool Intersection::operator==(Intersection const& other) const
{
    return m_distance == other.m_distance && m_shape == other.m_shape;
}

bool Intersection::operator<(Intersection const& other) const
{
    return m_distance < other.m_distance;
}

Intersection Hit(std::vector<Intersection> const& xs)
{
    for (auto const& i : xs)
    {
        if (i.Distance() >= 0.f)
            return i;
    }
    return { 0.f, nullptr };
}

std::ostream& operator<<(std::ostream& os, Intersection const& i)
{
    os << "[" << i.Distance() << ", 0x" << std::hex << i.Object() << "]";
    return os;
}

float Schlick(IntersectionData const& iData)
{
    // cosine of the angle between the eye and normal vectors
    auto cos = iData.m_eyev.Dot(iData.m_normalv);

    // total internal reflection can only ocurr if n1 > n2
    if (iData.m_n1 > iData.m_n2)
    {
        auto const n = iData.m_n1 / iData.m_n2;
        auto const sin2T = n * n * (1.f - (cos * cos));
        if (sin2T > 1.f)
        {
            return 1.f;
        }

        // compute cosine of theta using trig identity
        auto const cosT = std::sqrtf(1.f - sin2T);

        // when n1 > n2 use cosine of theta instead
        cos = cosT;
    }

    auto const r0 = std::powf((iData.m_n1 - iData.m_n2) / (iData.m_n1 + iData.m_n2), 2.f);
    return r0 + ((1.f - r0) * std::powf(1.f - cos, 5.f));
}
