#pragma once

#include "raytracer_export.h"

#include "Color.h"
#include "Types.h"

#include <unordered_map>


class Material
{
public:
    RAYTRACER_EXPORT Material();

    RAYTRACER_EXPORT void SetColor(Color const& color);
    RAYTRACER_EXPORT Color const& GetColor() const;

    RAYTRACER_EXPORT Color ColorAt(ShapeConstPtr const&, Tuple const& point) const;

    float Ambient() const { return m_ambient; }
    float Diffuse() const { return m_diffuse; }
    float Specular() const { return m_specular; }
    float Shininess() const { return m_shininess; }
    float Reflective() const { return m_reflective; }
    float Transparency() const { return m_transparency; }
    float RefractiveIndex() const { return m_refractiveIndex; }

    void Ambient(float ambient) { m_ambient = ambient; }
    void Diffuse(float diffuse) { m_diffuse = diffuse; }
    void Specular(float specular) { m_specular = specular; }
    void Shininess(float shininess) { m_shininess = shininess; }
    void Reflective(float reflective) { m_reflective = reflective; }
    void Transparency(float transparency) { m_transparency = transparency; }
    void RefractiveIndex(float refractiveIndex) { m_refractiveIndex = refractiveIndex; }

    void Pattern(PatternPtr const& pattern) { m_pattern = pattern; }
    PatternPtr& ModifyPattern() { return m_pattern; }

    RAYTRACER_EXPORT bool operator==(Material const& other) const;

    // Material type to index of refraction mapping
    enum class Type { Vacuum, Air, Water, Glass, Diamond };
    float GetIndexOfRefraction(Type type) const { return s_iorTable[type]; }

private:
    static std::unordered_map<Type, float> s_iorTable;

    float m_ambient;
    float m_diffuse;
    float m_specular;
    float m_shininess;
    float m_reflective;
    float m_transparency;
    float m_refractiveIndex;
    PatternPtr m_pattern;
};
