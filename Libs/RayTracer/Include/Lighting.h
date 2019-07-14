#pragma once

#include "Material.h"
#include "Color.h"
#include "raytracer_export.h"
#include "Tuple.h"

class PointLight
{
public:
    RAYTRACER_EXPORT PointLight(Tuple m_position, Color m_intensity);

    Tuple const& Position() const { return m_position; }
    Color const& Intensity() const { return m_intensity; }

    RAYTRACER_EXPORT bool operator==(PointLight const& other) const;

private:
    Tuple m_position;
    Color m_intensity;
};

RAYTRACER_EXPORT Color Lighting(Material const& m, PointLight const& light, Tuple const& position, Tuple const& eye, Tuple const& normal);
