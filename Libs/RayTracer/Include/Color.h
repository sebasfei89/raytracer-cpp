#pragma once

#include "raytracer_export.h"
#include "Tuple.h"

class RAYTRACER_EXPORT Color : public Tuple
{
public:
    Color(Tuple const& t) : Tuple(t[0], t[1], t[2], t[3]) {}
    Color(float r, float g, float b) : Tuple(r, g, b, 0.0f) {}

    float Red() const { return this->operator[](0); }
    float Green() const { return this->operator[](1); }
    float Blue() const { return this->operator[](2); }
};
