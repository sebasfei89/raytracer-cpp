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

SCENARIO("Computing the normal on a translated sphere", "[Geometry]")
{
    GIVEN_2(
        auto s = Sphere();
        s.SetTransform(matrix::Translation(0.f, 1.f, 0.f));
    WHEN_,
        auto const n = s.NormalAt(Point(0.f, 1.70711f, -0.70711f));
    REQUIRE_,
        n == Vector(0.f, 0.70711f, -0.70711f))
}

SCENARIO("Computing the normal on a transformed sphere", "[Geometry]")
{
    GIVEN_2(
        auto s = Sphere();
        float const coord = std::sqrt(2.f) / 2.f;
        s.SetTransform(matrix::Scaling(1.f, 0.5f, 1.f) * matrix::RotationZ(PI/5.f));
    WHEN_,
        auto const n = s.NormalAt(Point(0.f, coord, -coord));
    REQUIRE_,
        n == Vector(0.f, 0.97014f, -0.24254f))
}

SCENARIO("A sphere has a default material")
{
    GIVEN_2(
        auto s = Sphere();
    WHEN_,
        auto const m = s.GetMaterial();
    REQUIRE_,
        m == Material() )
}

SCENARIO("A sphere may be assigned a material")
{
    GIVEN_2(
        auto s = Sphere();
    WHEN_,
        auto m = s.GetMaterial();
        m.Diffuse(1.f);
        s.SetMaterial(m);
    REQUIRE_,
        s.GetMaterial() == m )
}
