#pragma once

#include "raytracer_export.h"
#include "Tuple.h"

class RAYTRACER_EXPORT Color : public Tuple
{
public:
    Color(Tuple const& t) : Tuple(t[0], t[1], t[2], t[3]) {}
    Color(float r, float g, float b) : Tuple(r, g, b, 0.0f) {}

    float R() const { return this->X(); }
    float G() const { return this->Y(); }
    float B() const { return this->Z(); }

    static Color const& White()  { static Color c(1.f, 1.f, 1.f); return c; }
    static Color const& Black()  { static Color c(0.f, 0.f, 0.f); return c; }
    static Color const& Red()    { static Color c(1.f, 0.f, 0.f); return c; }
    static Color const& Green()  { static Color c(0.f, 1.f, 0.f); return c; }
    static Color const& Blue()   { static Color c(0.f, 0.f, 1.f); return c; }
    static Color const& Yellow() { static Color c(1.f, 1.f, 0.f); return c; }
};
