#include "Shape.h"

Shape::Shape()
    : m_transform(Mat44::Identity())
    , m_invTransform(Mat44::Identity())
    , m_material()
    , m_castShadows(true)
{
}

Tuple Shape::NormalAt(Tuple const& worldPoint) const
{
    auto const localPoint = m_invTransform * worldPoint;
    auto const localNormal = NormalAtLocal(localPoint);
    auto worldNormal = m_invTransform.Transposed() * localNormal;
    worldNormal[3] = 0.f;
    return worldNormal.Normalized();
}

bool Shape::operator==(Shape const& other) const
{
    return GetMaterial() == other.GetMaterial()
        && Transform() == other.Transform();
}

bool Shape::IntersectsBefore(Ray const& ray, float distance) const
{
    std::vector<Intersection> xs;
    xs.reserve(2);
    Intersect(ray, xs);
    for (auto const& i : xs)
    {
        if ((i.Distance() >= EPSILON) && (i.Distance() < distance))
        {
            return true;
        }
    }
    return false;
}
