#pragma once

#include "Lighting.h"

PointLight::PointLight(Tuple position, Color intensity)
    : m_position(position)
    , m_intensity(intensity)
{
}

Color Lighting(Material const& m, PointLight const& light, Tuple const& position, Tuple const& eye, Tuple const& normal)
{
    Color effectiveColor = m.GetColor() * light.Intensity();

    Tuple const lightDir = (light.Position() - position).Normalized();
    Color const ambient = effectiveColor * m.Ambient();

    float const lightDotNormal = lightDir.Dot(normal);
    if (lightDotNormal < 0.f)
    {
        return ambient;
    }

    Color const diffuse = effectiveColor * m.Diffuse() * lightDotNormal;
    auto const reflect = (-lightDir).Reflect(normal);
    float const reflectDotEye = reflect.Dot(eye);
    if (reflectDotEye <= 0.0f)
    {
        return ambient + diffuse;
    }

    float const factor = std::powf(reflectDotEye, m.Shininess());
    Color const specular = light.Intensity() * m.Specular() * factor;
    return ambient + diffuse + specular;
}
