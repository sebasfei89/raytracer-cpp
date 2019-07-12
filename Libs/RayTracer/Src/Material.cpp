#include "Material.h"

Material::Material()
    : m_color(1.f, 1.f, 1.f)
    , m_ambient(0.1f)
    , m_diffuse(0.9f)
    , m_specular(0.9f)
    , m_shininess(200.f)
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
