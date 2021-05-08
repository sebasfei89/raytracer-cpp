#include "Pattern.h"
#include "Shapes/Shape.h"
#include "SimplexNoise.h"
#include "Transformations.h"

namespace
{

constexpr static char const* json_key_type = "type";
constexpr static char const* json_key_color = "color";
constexpr static char const* json_key_transform = "transform";
constexpr static char const* json_key_position = "position";
constexpr static char const* json_key_rotation = "rotation";
constexpr static char const* json_key_scaling = "scaling";
constexpr static char const* json_key_pattern = "pattern";
constexpr static char const* json_key_pattern_a = "pattern_a";
constexpr static char const* json_key_pattern_b = "pattern_b";

}

// static
PatternPtr IPattern::FromJSON(json const& data)
{
    if (!data.contains(json_key_type))
    {
        return nullptr;
    }

    std::string const type = data.at(json_key_type);
    PatternPtr pattern;
    if (type == "Solid")
    {
        Color color = Color::Black();
        if (data.contains(json_key_color))
        {
            data.at(json_key_color).get_to(color);
        }
        pattern = std::make_shared<SolidPattern>(color);
    }
    else if (type == "PerlinNoise")
    {
        auto inner = FromJSON(data.at(json_key_pattern));
        pattern = std::make_shared<PerlinNoisePattern>(inner);
    }
    else if (type == "Strip")
    {
        auto innerA = FromJSON(data.at(json_key_pattern_a));
        auto innerB = FromJSON(data.at(json_key_pattern_b));
        pattern = std::make_shared<StripPattern>(innerA, innerB);
    }
    else if (type == "Gradient")
    {
        auto innerA = FromJSON(data.at(json_key_pattern_a));
        auto innerB = FromJSON(data.at(json_key_pattern_b));
        pattern = std::make_shared<GradientPattern>(innerA, innerB);
    }
    else if (type == "Ring")
    {
        auto innerA = FromJSON(data.at(json_key_pattern_a));
        auto innerB = FromJSON(data.at(json_key_pattern_b));
        pattern = std::make_shared<RingPattern>(innerA, innerB);
    }
    else if (type == "Checker")
    {
        auto innerA = FromJSON(data.at(json_key_pattern_a));
        auto innerB = FromJSON(data.at(json_key_pattern_b));
        pattern = std::make_shared<CheckerPattern>(innerA, innerB);
    }
    else if (type == "RadialGradient")
    {
        auto innerA = FromJSON(data.at(json_key_pattern_a));
        auto innerB = FromJSON(data.at(json_key_pattern_b));
        pattern = std::make_shared<RadialGradientPattern>(innerA, innerB);
    }
    else if (type == "Blend")
    {
        auto innerA = FromJSON(data.at(json_key_pattern_a));
        auto innerB = FromJSON(data.at(json_key_pattern_b));
        pattern = std::make_shared<BlendPattern>(innerA, innerB);
    }

    if (pattern != nullptr)
    {
        Mat44 tx = Mat44::Identity();
        if (data.contains(json_key_position))
        {
            Tuple positionData = data.at(json_key_position);
            tx = matrix::Translation(positionData.X(), positionData.Y(), positionData.Z());
        }
        if (data.contains(json_key_rotation))
        {
            Tuple angles = data.at(json_key_rotation);
            tx = tx * matrix::RotationX(angles.X()) * matrix::RotationY(angles.Y()) * matrix::RotationZ(angles.Z());
        }
        if (data.contains(json_key_scaling))
        {
            Tuple scalingData = data.at(json_key_scaling);
            tx = tx * matrix::Scaling(scalingData.X(), scalingData.Y(), scalingData.Z());
        }
        pattern->SetTransform(tx);
    }

    return pattern;
}

IPattern::IPattern()
    : m_transform(Mat44::Identity())
    , m_invTransform(Mat44::Identity())
{
}

Color IPattern::ShapeColorAt(ShapeConstPtr const& shape, Tuple const& point) const
{
    Tuple const objectPoint = shape->WorldToLocal(point);
    Tuple const patternPoint = InvTransform() * objectPoint;
    return ColorAt(patternPoint);
}

bool SolidPattern::operator==(PatternPtr const& other) const
{
    auto o = std::dynamic_pointer_cast<SolidPattern>(other);
    return o && m_color == o->m_color;
}

Color PerlinNoisePattern::ColorAt(Tuple const& point) const
{
    static SimplexNoise noise;
    float const distortion = noise.Noise(point.X(), point.Y(), point.Z());
    auto disturbedPoint = Point(point.X() + distortion, point.Y() + distortion, point.Z() + distortion);
    return m_pattern->ColorAt(m_pattern->InvTransform() * disturbedPoint);
}

bool PerlinNoisePattern::operator==(PatternPtr const& other) const
{
    auto o = std::dynamic_pointer_cast<PerlinNoisePattern>(other);
    return (o != nullptr) && m_pattern->operator==(o->m_pattern);
}

Color BinaryPattern::ColorA(Tuple const& point) const
{
    return m_patternA->ColorAt(m_patternA->InvTransform() * point);
}

Color BinaryPattern::ColorB(Tuple const& point) const
{
    return m_patternB->ColorAt(m_patternB->InvTransform() * point);
}

bool BinaryPattern::operator==(PatternPtr const& other) const
{
    auto o = std::dynamic_pointer_cast<BinaryPattern>(other);
    return (o != nullptr) && (m_patternA->operator==(o->m_patternA)) && (m_patternB->operator==(o->m_patternB));
}

Color StripPattern::ColorAt(Tuple const& point) const
{
    return ((int)std::floor(point.X()) % 2) ? ColorB(point) : ColorA(point);
}

bool StripPattern::operator==(PatternPtr const& other) const
{
    return std::dynamic_pointer_cast<StripPattern>(other) && BinaryPattern::operator==(other);
}

Color GradientPattern::ColorAt(Tuple const& point) const
{
    auto const delta = ColorB(point) - ColorA(point);
    auto const fraction = std::min(std::max(point.X(), 0.f), 1.f);
    return ColorA(point) + (delta * fraction);
}

bool GradientPattern::operator==(PatternPtr const& other) const
{
    return std::dynamic_pointer_cast<GradientPattern>(other) && BinaryPattern::operator==(other);
}

Color RingPattern::ColorAt(Tuple const& point) const
{
    float const x2 = point.X() * point.X();
    float const z2 = point.Z() * point.Z();
    float const magnitude = std::sqrtf(x2 + z2);
    bool isEven = (((int)std::floor(magnitude)) % 2) == 0;
    return isEven ? ColorA(point) : ColorB(point);
}

bool RingPattern::operator==(PatternPtr const& other) const
{
    return std::dynamic_pointer_cast<RingPattern>(other) && BinaryPattern::operator==(other);
}

Color CheckerPattern::ColorAt(Tuple const& point) const
{

    int const x = static_cast<int>(std::floor(point.X()));
    int const y = static_cast<int>(std::floor(point.Y()));
    int const z = static_cast<int>(std::floor(point.Z()));
    return (((x + y + z) % 2) == 0) ? ColorA(point) : ColorB(point);
}

bool CheckerPattern::operator==(PatternPtr const& other) const
{
    return std::dynamic_pointer_cast<CheckerPattern>(other) && BinaryPattern::operator==(other);
}

Color RadialGradientPattern::ColorAt(Tuple const& point) const
{
    float const x2 = point.X() * point.X();
    float const z2 = point.Z() * point.Z();
    float const magnitude = std::sqrtf(x2 + z2);
    auto const delta = ColorB(point) - ColorA(point);
    return ColorA(point) + (delta * magnitude);
}

bool RadialGradientPattern::operator==(PatternPtr const& other) const
{
    return std::dynamic_pointer_cast<RadialGradientPattern>(other) && BinaryPattern::operator==(other);
}

Color BlendPattern::ColorAt(Tuple const& point) const
{
    return (ColorA(point) + ColorB(point)) * 0.5f;
}

bool BlendPattern::operator==(PatternPtr const& other) const
{
    return std::dynamic_pointer_cast<BlendPattern>(other) && BinaryPattern::operator==(other);
}

std::ostream& operator<<(std::ostream& os, IPattern const& p)
{
    os << "{" << p.Transform() << "}";
    return os;
}
