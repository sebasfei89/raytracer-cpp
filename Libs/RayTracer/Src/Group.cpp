#include "Group.h"

#include "Ray.h"

#include <algorithm>

void Group::Intersect(Ray const& ray, std::vector<Intersection>& xs) const
{
    for (auto const& child : m_children)
    {
        ray.Intersect(child, xs);
    }
    std::sort(xs.begin(), xs.end());
}

Tuple Group::NormalAtLocal(Tuple const& point) const
{
    throw std::runtime_error("Groups don't have normal by themselves. NormalAtLocal must be called directly on contained shapes.");
}

void Group::AddChild(ShapePtr child)
{
    m_children.push_back(child);
    child->SetParent(shared_from_this());
}

bool Group::Includes(ShapePtr child) const
{
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}
