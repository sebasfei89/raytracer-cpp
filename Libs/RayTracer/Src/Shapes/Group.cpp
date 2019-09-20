#include "Shapes/Group.h"

#include "Ray.h"
#include "Shapes/ShapeFactory.h"

#include <algorithm>

namespace
{

constexpr static char const* json_key_children = "children";
constexpr static char const* json_key_type = "type";
constexpr static char const* json_key_archetype = "archetype";

}

REGISTER_SHAPE(Group);

Group::Group()
{
    auto& bounds = ModifyBounds();
    bounds.Min(Point(INF, INF, INF));
    bounds.Max(Point(-INF, -INF, -INF));
}

void Group::Initialize(json const& data, json const& dataOverride, ArchetypeMap const& archetypes)
{
    Shape::Initialize(data, dataOverride, archetypes);

    std::vector<json> children;
    GetProperty(json_key_children, data, dataOverride, children);
    for (auto const& child : children)
    {
        auto instance = ShapeFactory::Get().Create(child, {}, archetypes);
        if (instance != nullptr)
        {
            AddChild(instance);
        }
    }
}

void Group::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    if (GetBounds().Intersects(ray))
    {
        for (auto const& child : m_children)
        {
            ray.Intersect(child, xs);
        }
        std::sort(xs.begin(), xs.end());
    }
}

Tuple Group::NormalAtLocal(Tuple const& point) const
{
    throw std::runtime_error("Groups don't have normal by themselves. NormalAtLocal must be called directly on contained shapes.");
}

void Group::AddChild(ShapePtr child)
{
    m_children.push_back(child);
    child->SetParent(shared_from_this());
    UpdateBounds(child);
}

bool Group::Includes(ShapePtr child) const
{
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

void Group::UpdateBounds(ShapePtr child)
{
    auto childBounds = child->Transform() * child->GetBounds();
    auto& bounds = ModifyBounds();
    bounds.Merge(childBounds);
}

void Group::UpdateBounds()
{
    auto& bounds = ModifyBounds();
    bounds.Min(Point(INF, INF, INF));
    bounds.Max(Point(-INF, -INF, -INF));
    for (auto const& child : m_children)
    {
        UpdateBounds(child);
    }
}
