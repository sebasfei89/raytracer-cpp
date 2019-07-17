#include "Ray.h"
#include "Util.h"

Ray::Ray(Tuple const& origin, Tuple const& direction)
    : m_origin(origin)
    , m_direction(direction)
{
}

std::vector<Intersection> Ray::Intersect(Sphere const& sphere) const
{
    Ray const r = sphere.Transform().Inverse() * (*this);
    auto const sphereToRay = r.m_origin - sphere.Center();
    float const a = r.m_direction.Dot(r.m_direction);
    float const b = 2.f * r.m_direction.Dot(sphereToRay);
    float const c = sphereToRay.Dot(sphereToRay) - 1.f;
    float const discriminant = (b * b) - (4.f * a * c);

    if (discriminant < 0.f) return {};

    float const aTimes2 = 1.f / (2.f * a);
    float const sqrtDiscriminant = sqrtf(discriminant);

    return {
        { (-b - sqrtDiscriminant) * aTimes2, &sphere },
        { (-b + sqrtDiscriminant) * aTimes2, &sphere }
    };
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
