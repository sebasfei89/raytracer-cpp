#include "Ray.h"
#include "Util.h"

Ray::Ray(Tuple const& origin, Tuple const& direction)
    : m_origin(origin)
    , m_direction(direction)
{
}

std::vector<Intersection> Ray::Intersect(ShapePtr const& shape) const
{
    Ray const r = shape->Transform().Inverse() * (*this);
    return shape->Intersect(r);
}

std::vector<Intersection> Ray::Intersect(World const& world) const
{
    std::vector<Intersection> xs;
    for (auto const& obj : world.Objects())
    {
        auto const objXs = Intersect(obj);
        xs.insert(xs.end(), objXs.begin(), objXs.end());
    }
    std::sort(xs.begin(), xs.end());
    return xs;
}

IntersectionData Ray::Precompute(Intersection const& i) const
{
    IntersectionData data;
    data.m_distance = i.Distance();
    data.m_object = i.Object();
    data.m_point = Position(data.m_distance);
    data.m_eyev = -m_direction;
    data.m_normalv = data.m_object->NormalAt(data.m_point);
    data.m_inside = data.m_normalv.Dot(data.m_eyev) < 0.f;
    if (data.m_inside)
    {
        data.m_normalv = -data.m_normalv;
    }
    data.m_overPoint = data.m_point + (data.m_normalv * EPSILON);
    return data;
}

Ray operator*(Mat44 const& m, Ray const& r)
{
    return Ray{ m * r.Origin(), m * r.Direction() };
}
