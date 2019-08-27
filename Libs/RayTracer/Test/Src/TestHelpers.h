#pragma once

#include <RayTracer/Pattern.h>
#include <RayTracer/Ray.h>
#include <RayTracer/Shape.h>
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

class TestShape : public Shape
{
public:
    TestShape()
        : m_localRay(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f))
    {}

    void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override
    {
        const_cast<TestShape*>(this)->m_localRay = ray;
    }

    Tuple NormalAtLocal(Tuple const& point) const override
    {
        return Vector(point[0], point[1], point[2]);
    }

    void SetBounds(Tuple const& min, Tuple const& max)
    {
        ModifyBounds().Min(min);
        ModifyBounds().Max(max);
    }

    Ray m_localRay;
};
