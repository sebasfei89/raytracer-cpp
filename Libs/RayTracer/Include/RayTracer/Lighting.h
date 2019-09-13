#pragma once

#include "Material.h"
#include "Color.h"
#include "raytracer_export.h"
#include "Tuple.h"

#include <string>

class PointLight
{
public:
    RAYTRACER_EXPORT PointLight(Tuple m_position, Color m_intensity);

    void SetName(std::string const& name) { m_name = name; }
    std::string const& GetName() const { return m_name; }

    Tuple const& Position() const { return m_position; }
    Color const& Intensity() const { return m_intensity; }

    RAYTRACER_EXPORT bool operator==(PointLight const& other) const;

private:
    Tuple m_position;
    Color m_intensity;
    std::string m_name;
};

RAYTRACER_EXPORT Color Lighting(Material const& m, ShapeConstPtr const& shape, PointLight const& light, Tuple const& position, Tuple const& eye, Tuple const& normal, bool inShadow=false);
