#include "Material.h"
#include "Sphere.h"
#include "Transformations.h"

#include <Testing.h>

#include <cmath>

SCENARIO("The normal on a sphere at a point on te x axis", "[Geometry]")
{
    GIVEN_2(
        auto const s = Sphere();
    WHEN_,
        auto const n = s.NormalAt(Point(1.f, 0.f, 0.f));
    REQUIRE_,
        n == Vector(1.f, 0.f, 0.f) )
}

SCENARIO("The normal on a sphere at a point on te y axis", "[Geometry]")
{
    GIVEN_2(
        auto const s = Sphere();
    WHEN_,
        auto const n = s.NormalAt(Point(0.f, 1.f, 0.f));
    REQUIRE_,
        n == Vector(0.f, 1.f, 0.f))
}

SCENARIO("The normal on a sphere at a point on te z axis", "[Geometry]")
{
    GIVEN_2(
        auto const s = Sphere();
    WHEN_,
        auto const n = s.NormalAt(Point(0.f, 0.f, 1.f));
    REQUIRE_,
        n == Vector(0.f, 0.f, 1.f))
}

SCENARIO("The normal on a sphere at a non-axial point", "[Geometry]")
{
    GIVEN_2(
        auto const s = Sphere();
        auto const coord = std::sqrt(3.f) / 3.f;
    WHEN_,
        auto const n = s.NormalAt(Point(coord, coord, coord));
    REQUIRE_,
        n == Vector(coord, coord, coord))
}

SCENARIO("The normal is a normalized vector", "[Geometry]")
{
    GIVEN_2(
        auto const s = Sphere();
        auto const coord = std::sqrt(3.f) / 3.f;
    WHEN_,
        auto const n = s.NormalAt(Point(coord, coord, coord));
    REQUIRE_,
        n == n.Normalized() )
}
