#pragma once

#include "Pattern.h"
#include "Sphere.h"
#include "World.h"

World DefaultWorld();
ShapePtr GlassySphere();

class TestPattern : public IPattern
{
public:
    Color ColorAt(Tuple const& point) const override { return Color(point.X(), point.Y(), point.Z()); }
};
