#include "Ray.h"
#include "Transformations.h"

#include <Testing.h>

SCENARIO("Creating and querying a ray", "[Math]")
{
    GIVEN_2(
        auto const origin = Point(1.f, 2.f, 3.f);
        auto const direction = Vector(4.f, 5.f, 6.f);
    WHEN_,
        auto const r = Ray(origin, direction);
    REQUIRE_,
        r.Origin() == origin,
        r.Direction() == direction )
}

SCENARIO("Computing a point from a distance", "[Math]")
{
    GIVEN_1(
        auto const r = Ray(Point(2.f, 3.f, 4.f), Vector(1.f, 0.f, 0.f));
    REQUIRE_,
        r.Position(0.f) == Point(2.f, 3.f, 4.f),
        r.Position(1.f) == Point(3.f, 3.f, 4.f),
        r.Position(-1.f) == Point(1.f, 3.f, 4.f),
        r.Position(2.5f) == Point(4.5f, 3.f, 4.f) )
}

SCENARIO("A ray intersects a sphere at two points", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto const s = Sphere();
    WHEN_,
        auto xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Distance() == 4.0f,
        xs[1].Distance() == 6.0f )
}

SCENARIO("A ray intersects a sphere at a tangent", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 1.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto const s = Sphere();
    WHEN_,
        auto xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Distance() == 5.0f,
        xs[1].Distance() == 5.0f)
}

SCENARIO("A ray misses a sphere", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto const s = Sphere();
    WHEN_,
        auto xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 0 )
}

SCENARIO("A ray originates inside a sphere", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
        auto const s = Sphere();
    WHEN_,
        auto xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Distance() == -1.0f,
        xs[1].Distance() == 1.0f )
}

SCENARIO("A sphere is behind a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
        auto const s = Sphere();
    WHEN_,
        auto xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Distance() == -6.0f,
        xs[1].Distance() == -4.0f )
}

SCENARIO("Translating a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(1.f, 2.f, 3.f), Vector(0.f, 1.f, 0.f));
        auto const m = matrix::Translation(3.f, 4.f, 5.f);
    WHEN_,
        auto const r2 = m * r;
    REQUIRE_,
        r2.Origin() == Point(4.f, 6.f, 8.f),
        r2.Direction() == Vector(0.f, 1.f, 0.f) )
}

SCENARIO("Scaling a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(1.f, 2.f, 3.f), Vector(0.f, 1.f, 0.f));
        auto const m = matrix::Scaling(2.f, 3.f, 4.f);
    WHEN_,
        auto const r2 = m * r;
    REQUIRE_,
        r2.Origin() == Point(2.f, 6.f, 12.f),
        r2.Direction() == Vector(0.f, 3.f, 0.f))
}

SCENARIO("A sphere's default transformation", "[Math]")
{
    GIVEN_1(
        auto const s = Sphere();
    REQUIRE_,
        s.Transform() == Mat44::Identity() )
}

SCENARIO("Changing a sphere's transformation", "[Math]")
{
    GIVEN_2(
        auto s = Sphere();
        auto const t = matrix::Translation(2.f, 3.f, 4.f);
    WHEN_,
        s.SetTransform(t);
    REQUIRE_,
        s.Transform() == t)
}

SCENARIO("Intersecting a scaled sphere with a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = Sphere();
    WHEN_,
        s.SetTransform(matrix::Scaling(2.f, 2.f, 2.f));
        auto const xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Distance() == 3.f,
        xs[1].Distance() == 7.f )
}

SCENARIO("Intersecting a translated sphere with a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = Sphere();
    WHEN_,
        s.SetTransform(matrix::Translation(5.f, 2.f, 2.f));
        auto const xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 0 )
}
