#include "Material.h"
#include "Pattern.h"

namespace
{

constexpr static char const* json_key_ambient = "ambient";
constexpr static char const* json_key_diffuse = "diffuse";
constexpr static char const* json_key_specular = "specular";
constexpr static char const* json_key_shininess = "shininess";
constexpr static char const* json_key_reflective = "reflective";
constexpr static char const* json_key_transparency = "transparency";
constexpr static char const* json_key_refractive_index = "refractiveIndex";
constexpr static char const* json_key_pattern = "pattern";

}

std::unordered_map<Material::Type, float> Material::s_iorTable = {
    {Type::Vacuum , 1.f      },
    {Type::Air    , 1.00029f },
    {Type::Water  , 1.333f   },
    {Type::Glass  , 1.52f    },
    {Type::Diamond, 2.417f   }
};

Material::Material()
    : m_ambient(0.1f)
    , m_diffuse(0.9f)
    , m_specular(0.9f)
    , m_shininess(200.f)
    , m_reflective(0.f)
    , m_transparency(0.f)
    , m_refractiveIndex(1.f)
    , m_pattern(std::make_shared<SolidPattern>(Color(1.f, 1.f, 1.f)))
{
}

Material::Material(float ambient, float diffuse, float specular, float shininess, float reflective, float transparency, float refractiveIndex)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular)
    , m_shininess(shininess)
    , m_reflective(reflective)
    , m_transparency(transparency)
    , m_refractiveIndex(refractiveIndex)
    , m_pattern(std::make_shared<SolidPattern>(Color(1.f, 1.f, 1.f)))
{
}

bool Material::operator==(Material const& other) const
{
    return GetColor() == other.GetColor()
        && Ambient() == other.Ambient()
        && Diffuse() == other.Diffuse()
        && Specular() == other.Specular()
        && Shininess() == other.Shininess()
        && Reflective() == other.Reflective()
        && Transparency() == other.Transparency()
        && RefractiveIndex() == other.RefractiveIndex()
        && Pattern()->operator==(other.Pattern());
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

std::ostream& operator<<(std::ostream& os, Material const& m)
{
    os << "{ " << m.Ambient()
       << ", " << m.Diffuse()
       << ", " << m.Specular()
       << ", " << m.Shininess()
       << ", " << m.Reflective()
       << ", " << m.Transparency()
       << ", " << m.RefractiveIndex()
       << ", " << *m.Pattern().get()
       << " }";
    return os;
}

void to_json(json& j, Material const& m)
{
    j = json{
        {json_key_ambient, m.Ambient()},
        {json_key_diffuse, m.Diffuse()},
        {json_key_specular, m.Specular()},
        {json_key_shininess, m.Shininess()},
        {json_key_reflective, m.Reflective()},
        {json_key_transparency, m.Transparency()},
        {json_key_refractive_index, m.RefractiveIndex()},
        //  TODO:
        //{json_key_pattern, *m.Pattern().get()}
    };
}

void from_json(json const& j, Material& m)
{
    if (j.contains(json_key_ambient)) m.Ambient(j.at(json_key_ambient));
    if (j.contains(json_key_diffuse)) m.Diffuse(j.at(json_key_diffuse));
    if (j.contains(json_key_specular)) m.Specular(j.at(json_key_specular));
    if (j.contains(json_key_shininess)) m.Shininess(j.at(json_key_shininess));
    if (j.contains(json_key_reflective)) m.Reflective(j.at(json_key_reflective));
    if (j.contains(json_key_transparency)) m.Transparency(j.at(json_key_transparency));
    if (j.contains(json_key_refractive_index)) m.RefractiveIndex(j.at(json_key_refractive_index));
    if (j.contains(json_key_pattern))
    {
        auto pattern = IPattern::FromJSON(j.at(json_key_pattern));
        m.Pattern(pattern);
    }
}
