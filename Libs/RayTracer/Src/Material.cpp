#include "Material.h"
#include "Pattern.h"

Material::Material()
    : m_ambient(0.1f)
    , m_diffuse(0.9f)
    , m_specular(0.9f)
    , m_shininess(200.f)
    , m_reflective(0.f)
    , m_pattern(std::make_shared<SolidPattern>(Color(1.f, 1.f, 1.f)))
{
}

bool Material::operator==(Material const& other) const
{
    return GetColor() == other.GetColor()
        && Ambient() == other.Ambient()
        && Diffuse() == other.Diffuse()
        && Specular() == other.Specular()
        && Shininess() == other.Shininess();
}

Color Material::ColorAt(ShapeConstPtr const& shape, Tuple const& point) const
{
    return m_pattern->ShapeColorAt(shape, point);
}

void Material::SetColor(Color const& color)
{
    m_pattern = std::make_shared<SolidPattern>(color);
}

Color const& Material::GetColor() const
{
    static Color const color = m_pattern->ColorAt(Point(0.f, 0.f, 0.f));
    return color;
}
