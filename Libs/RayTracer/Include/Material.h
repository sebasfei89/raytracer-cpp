#pragma once

#include "raytracer_export.h"

#include "Color.h"
#include "Types.h"

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

    void Ambient(float ambient) { m_ambient = ambient; }
    void Diffuse(float diffuse) { m_diffuse = diffuse; }
    void Specular(float specular) { m_specular = specular; }
    void Shininess(float shininess) { m_shininess = shininess; }

    void Pattern(PatternPtr const& pattern) { m_pattern = pattern; }
    PatternPtr& ModifyPattern() { return m_pattern; }

    RAYTRACER_EXPORT bool operator==(Material const& other) const;

private:
    float m_ambient;
    float m_diffuse;
    float m_specular;
    float m_shininess;
    PatternPtr m_pattern;
};
