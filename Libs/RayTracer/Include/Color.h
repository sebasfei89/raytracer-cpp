#pragma once

#include "Tuple.h"

class Color : public Tuple
{
public:
    Color(float r, float g, float b) : Tuple(r, g, b, 0.0f) {}

    float Red() const { return this->operator[](0); }
    float Green() const { return this->operator[](1); }
    float Blue() const { return this->operator[](2); }
};
