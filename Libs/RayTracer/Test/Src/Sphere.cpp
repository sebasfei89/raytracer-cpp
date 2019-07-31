#include "TestHelpers.h"

#include <RayTracer/Material.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/Transformations.h>

#include <Beddev/Beddev.h>

SCENARIO("The normal on a sphere at a point on te x axis", "geometry")
{
    GIVEN( auto const s = Sphere() )
    WHEN( auto const n = s.NormalAt(Point(1.f, 0.f, 0.f)) )
    THEN( n == Vector(1.f, 0.f, 0.f) )
}

SCENARIO("The normal on a sphere at a point on te y axis", "geometry")
{
    GIVEN( auto const s = Sphere() )
    WHEN( auto const n = s.NormalAt(Point(0.f, 1.f, 0.f)) )
    THEN( n == Vector(0.f, 1.f, 0.f) )
}

SCENARIO("The normal on a sphere at a point on te z axis", "geometry")
{
    GIVEN( auto const s = Sphere() )
    WHEN( auto const n = s.NormalAt(Point(0.f, 0.f, 1.f)) )
    THEN( n == Vector(0.f, 0.f, 1.f) )
}

SCENARIO("The normal on a sphere at a non-axial point", "geometry")
{
    GIVEN( auto const s = Sphere();
         , auto const coord = std::sqrt(3.f) / 3.f )
    WHEN( auto const n = s.NormalAt(Point(coord, coord, coord)) )
    THEN( n == Vector(coord, coord, coord) )
}

SCENARIO("The normal is a normalized vector", "geometry")
{
    GIVEN( auto const s = Sphere()
         , auto const coord = std::sqrt(3.f) / 3.f )
    WHEN( auto const n = s.NormalAt(Point(coord, coord, coord)) )
    THEN( n == n.Normalized() )
}

SCENARIO("Helper for producing a sphere with a glassy material", "geometry")
{
    GIVEN( auto const s = GlassySphere() )
    THEN( s->Transform() == Mat44::Identity()
        , s->GetMaterial().Transparency() == 1.f
        , s->GetMaterial().RefractiveIndex() == 1.5f )
}
