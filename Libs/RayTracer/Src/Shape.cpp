#include "Shape.h"

Shape::Shape()
    : m_transform(Mat44::Identity())
    , m_material()
{
}

Tuple Shape::NormalAt(Tuple const& worldPoint) const
{
    auto invTx = Transform().Inverse();
    auto const localPoint = invTx * worldPoint;
    auto const localNormal = NormalAtLocal(localPoint);
    invTx.Transpose();
    auto worldNormal = invTx * localNormal;
    worldNormal[3] = 0.f;
    return worldNormal.Normalized();
}

bool Shape::operator==(Shape const& other) const
{
    return GetMaterial() == other.GetMaterial()
        && Transform() == other.Transform();
}
