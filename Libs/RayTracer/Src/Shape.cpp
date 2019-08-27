#include "Shape.h"

Shape::Shape()
    : m_transform(Mat44::Identity())
    , m_invTransform(Mat44::Identity())
    , m_material()
    , m_castShadows(true)
    , m_parent()
    , m_bounds()
{
}

Tuple Shape::NormalAt(Tuple const& worldPoint) const
{
    auto const localPoint = WorldToLocal(worldPoint);
    auto const localNormal = NormalAtLocal(localPoint);
    return NormalToWorld(localNormal);
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

Tuple Shape::WorldToLocal(Tuple const& point) const
{
    auto localPnt = (m_parent != nullptr) ? m_parent->WorldToLocal(point) : point;
    return m_invTransform * localPnt;
}

Tuple Shape::NormalToWorld(Tuple const& normal) const
{
    auto nParent = m_invTransform.Transposed() * normal;
    nParent[3] = 0.f;
    nParent.Normalize();
    return (m_parent != nullptr) ? m_parent->NormalToWorld(nParent) : nParent;
}

void Shape::SetTransform(Mat44 const& t)
{
    m_transform = t;
    m_invTransform = t.Inverse();
    if (m_parent != nullptr)
    {
        m_parent->UpdateBounds();
    }
}

void Shape::UpdateBounds()
{
    throw std::runtime_error("A concrete shape cannot be set as a parent of other shape. Only groups are allowed!");
}
