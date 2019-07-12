#include "Sphere.h"

Sphere::Sphere()
    : m_transform(Mat44::Identity())
    , m_material()
{
}

Tuple Sphere::NormalAt(Tuple const& worldPoint) const
{
    auto invTx = m_transform.Inverse();
    auto const localPoint = invTx * worldPoint;
    auto const localNormal = localPoint - Center();
    invTx.Transpose();
    auto worldNormal = invTx * localNormal;
    worldNormal[3] = 0.f;
    return worldNormal.Normalized();
}
