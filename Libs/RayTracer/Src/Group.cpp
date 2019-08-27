#include "Group.h"

#include "Ray.h"

#include <algorithm>

Group::Group()
{
    auto& bounds = ModifyBounds();
    bounds.Min(Point(INF, INF, INF));
    bounds.Max(Point(-INF, -INF, -INF));
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
