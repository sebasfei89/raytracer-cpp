#pragma once

#include "Color.h"
#include "raytracer_export.h"

class RAYTRACER_EXPORT Material
{
public:
    Material();

    Color const& GetColor() const { return m_color; }
    float Ambient() const { return m_ambient; }
    float Diffuse() const { return m_diffuse; }
    float Specular() const { return m_specular; }
    float Shininess() const { return m_shininess; }

    void SetColor(Color const& color) { m_color = color; }
    void Ambient(float ambient) { m_ambient = ambient; }
    void Diffuse(float diffuse) { m_diffuse = diffuse; }
    void Specular(float specular) { m_specular = specular; }
    void Shininess(float shininess) { m_shininess = shininess; }

    bool operator==(Material const& other) const;

private:
    Color m_color;
    float m_ambient;
    float m_diffuse;
    float m_specular;
    float m_shininess;
};
