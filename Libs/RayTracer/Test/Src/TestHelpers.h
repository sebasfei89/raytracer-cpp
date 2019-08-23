#pragma once

#include <RayTracer/Pattern.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/World.h>

World DefaultWorld();
World DefaultWorld2();
ShapePtr GlassySphere();

class TestPattern : public IPattern
{
public:
    Color ColorAt(Tuple const& point) const override { return Color(point.X(), point.Y(), point.Z()); }
};

struct TestArg
{
    Tuple origin;
    Tuple direction;
    float t1, t2;
};

std::ostream& operator<<(std::ostream& os, TestArg const& arg);
