#include "Pattern.h"
#include "Shape.h"
#include "SimplexNoise.h"

IPattern::IPattern()
    : m_transform(Mat44::Identity())
    , m_invTransform(Mat44::Identity())
{
}

Color IPattern::ShapeColorAt(ShapeConstPtr const& shape, Tuple const& point) const
{
    Tuple const objectPoint = shape->InvTransform() * point;
    Tuple const patternPoint = InvTransform() * objectPoint;
    return ColorAt(patternPoint);
}

Color PerlinNoisePattern::ColorAt(Tuple const& point) const
{
    static SimplexNoise noise;
    float const distortion = noise.Noise(point.X(), point.Y(), point.Z());
    auto disturbedPoint = Point(point.X() + distortion, point.Y() + distortion, point.Z() + distortion);
    return m_pattern->ColorAt(m_pattern->InvTransform() * disturbedPoint);
}

Color BinaryPattern::ColorA(Tuple const& point) const
{
    return m_patternA->ColorAt(m_patternA->InvTransform() * point);
}

Color BinaryPattern::ColorB(Tuple const& point) const
{
    return m_patternB->ColorAt(m_patternB->InvTransform() * point);
}

Color StripPattern::ColorAt(Tuple const& point) const
{
    return ((int)std::floor(point.X()) % 2) ? ColorB(point) : ColorA(point);
}

Color GradientPattern::ColorAt(Tuple const& point) const
{
    auto const delta = ColorB(point) - ColorA(point);
    auto const fraction = std::min(std::max(point.X(), 0.f), 1.f);
    return ColorA(point) + (delta * fraction);
}

Color RingPattern::ColorAt(Tuple const& point) const
{
    float const x2 = point.X() * point.X();
    float const z2 = point.Z() * point.Z();
    float const magnitude = std::sqrtf(x2 + z2);
    bool isEven = (((int)std::floor(magnitude)) % 2) == 0;
    return isEven ? ColorA(point) : ColorB(point);
}

Color CheckerPattern::ColorAt(Tuple const& point) const
{

    int const x = static_cast<int>(std::floor(point.X()));
    int const y = static_cast<int>(std::floor(point.Y()));
    int const z = static_cast<int>(std::floor(point.Z()));
    return (((x + y + z) % 2) == 0) ? ColorA(point) : ColorB(point);
}

Color RadialGradientPattern::ColorAt(Tuple const& point) const
{
    float const x2 = point.X() * point.X();
    float const z2 = point.Z() * point.Z();
    float const magnitude = std::sqrtf(x2 + z2);
    auto const delta = ColorB(point) - ColorA(point);
    return ColorA(point) + (delta * magnitude);
}

Color BlendPattern::ColorAt(Tuple const& point) const
{
    return (ColorA(point) + ColorB(point)) * 0.5f;
}
