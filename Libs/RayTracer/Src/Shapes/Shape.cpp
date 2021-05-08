#include "Shapes/Shape.h"
#include "Transformations.h"

namespace
{

constexpr static char const* json_key_name = "name";
constexpr static char const* json_key_position = "position";
constexpr static char const* json_key_rotation = "rotation";
constexpr static char const* json_key_scaling = "scaling";
constexpr static char const* json_key_material = "material";
constexpr static char const* json_key_cast_shadows = "cast_shadows";

}

Shape::Shape()
    : m_name("<unnamed>")
    , m_transform(Mat44::Identity())
    , m_invTransform(Mat44::Identity())
    , m_material()
    , m_castShadows(true)
    , m_parent()
    , m_bounds()
{
}

void Shape::Initialize(json const& data, json const& dataOverride, ArchetypeMap const& archetypes)
{
    GetProperty(json_key_name, data, dataOverride, m_name);

    Tuple pos = Point(0.f, 0.f, 0.f);
    Tuple angles = Point(0.f, 0.f, 0.f);
    Tuple scale = Point(1.f, 1.f, 1.f);
    GetProperty(json_key_position, data, dataOverride, pos);
    GetProperty(json_key_rotation, data, dataOverride, angles);
    GetProperty(json_key_scaling, data, dataOverride, scale);

    SetTransform(
        matrix::Translation(pos.X(), pos.Y(), pos.Z())
        * matrix::RotationX(angles.X())
        * matrix::RotationY(angles.Y())
        * matrix::RotationZ(angles.Z())
        * matrix::Scaling(scale.X(), scale.Y(), scale.Z()) );

    GetProperty(json_key_cast_shadows, data, dataOverride, m_castShadows);

    // TODO: reference material by name
    GetProperty(json_key_material, data, dataOverride, m_material);
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
