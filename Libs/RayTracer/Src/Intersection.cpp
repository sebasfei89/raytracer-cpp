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
