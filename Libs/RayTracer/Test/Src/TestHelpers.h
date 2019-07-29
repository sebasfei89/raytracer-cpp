#pragma once

#include <RayTracer/Pattern.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/World.h>

World DefaultWorld();
ShapePtr GlassySphere();

class TestPattern : public IPattern
{
public:
    Color ColorAt(Tuple const& point) const override { return Color(point.X(), point.Y(), point.Z()); }
};
