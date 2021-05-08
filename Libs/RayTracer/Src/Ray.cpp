#include "Ray.h"
#include "Util.h"

Ray::Ray(Tuple const& origin, Tuple const& direction)
    : m_origin(origin)
    , m_direction(direction)
{
}

void Ray::Intersect(ShapePtr const& shape, std::vector<Intersection>& xs) const
{
    Ray const r = shape->InvTransform() * (*this);
    shape->Intersect(r, xs);
}

void Ray::Intersect(World const& world, std::vector<Intersection>& xs) const
{
    for (auto const& obj : world.Objects())
    {
        Intersect(obj, xs);
    }
    std::sort(xs.begin(), xs.end());
}

bool Ray::IntersectsBefore(ShapePtr const& shape, float distance) const
{
    Ray const r = shape->InvTransform() * (*this);
    return shape->CastShadows() && shape->IntersectsBefore(r, distance);
}

bool Ray::HasIntersectionNearThan(World const& world, float distance) const
{
    for (auto const& obj : world.Objects())
    {
        if (IntersectsBefore(obj, distance))
        {
            return true;
        }
    }
    return false;
}

IntersectionData Ray::Precompute(Intersection const& i, std::vector<Intersection> const& xs) const
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
    data.m_underPoint = data.m_point - (data.m_normalv * EPSILON);
    data.m_reflectv = m_direction.Reflect(data.m_normalv);

    std::vector<ShapeConstPtr> containers;
    for (auto const& current : xs)
    {
        bool const currentIsHit = current == i;
        if (currentIsHit)
        {
            data.m_n1 = containers.empty() ? 1.f : containers.back()->GetMaterial().RefractiveIndex();
        }

        auto it = std::find(containers.begin(), containers.end(), current.Object());
        if (it != containers.end())
        {
            containers.erase(it);
        }
        else
        {
            containers.push_back(current.Object());
        }

        if (currentIsHit)
        {
            data.m_n2 = containers.empty() ? 1.f : containers.back()->GetMaterial().RefractiveIndex();
            break;
        }
    }

    return data;
}

Ray operator*(Mat44 const& m, Ray const& r)
{
    return Ray{ m * r.Origin(), m * r.Direction() };
}
