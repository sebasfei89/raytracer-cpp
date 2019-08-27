#include "TestHelpers.h"

#include <Beddev/Beddev.h>

#include <RayTracer/Bounds.h>
#include <RayTracer/Transformations.h>

SCENARIO("A bounds default extents", "AABB")
{
    GIVEN( auto const b = Bounds() )
    THEN( b.Min() == Point(-1.f, -1.f, -1.f)
        , b.Max() == Point( 1.f,  1.f,  1.f) )
}

SCENARIO("A bounds created with arbitrary min and max extents", "AABB")
{
    GIVEN( auto const b = Bounds(Point(-1.f, -2.f, -3.f), Point(3.f, 2.f, 1.f)) )
    THEN( b.Min() == Point(-1.f, -2.f, -3.f)
        , b.Max() == Point(3.f, 2.f, 1.f) )
}

SCENARIO("Two bounds are merged into one", "AABB")
{
    GIVEN( auto b1 = Bounds(Point(-1.f, -1.f, -1.f), Point( 1.f, 1.f, 1.f))
         , auto const b2 = Bounds(Point( 0.f, -2.f,  3.f), Point(-3.f, 2.f, 0.f)) )
    WHEN( b1.Merge(b2) )
    THEN( b1.Min() == Point(-1.f, -2.f, -1.f)
        , b1.Max() == Point( 1.f,  2.f,  1.f) )
}

SCENARIO("Transforming bounds", "AABB")
{
    GIVEN( auto const b = Bounds(Point(-1.f, -1.f, -1.f), Point(1.f, 1.f, 1.f))
         , auto const m = matrix::Translation(-1.f, 0.f, 0.f) * matrix::RotationZ(-PIOVR4) * matrix::Scaling(2.f, 2.f, 2.f) )
    WHEN( auto const transformedBounds = m * b )
    THEN( transformedBounds == Bounds(Point(-3.8284f, -2.8284f, -2.f), Point(1.8284f, 2.8284f, 2.f)) )
}

PSCENARIO(TestArg, "A ray intersects an AABB", "AABB")
{
    PARAMS( { Point(5.f, .5f, 0.f), Vector(-1.f, 0.f, 0.f), 4.f, 6.f }
          , { Point(-5.f, .5f, 0.f), Vector(1.f, 0.f, 0.f), 4.f, 6.f }
          , { Point(.5f, 5.f, 0.f), Vector(0.f, -1.f, 0.f), 4.f, 6.f }
          , { Point(.5f, -5.f, 0.f), Vector(0.f, 1.f, 0.f), 4.f, 6.f }
          , { Point(.5f, 0.f, 5.f), Vector(0.f, 0.f, -1.f), 4.f, 6.f }
          , { Point(.5f, 0.f, -5.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f }
          , { Point(0.f, .5f, 0.f), Vector(0.f, 0.f, 1.f), -1.f, 1.f } )
    GIVEN( auto const arg = GetParam()
         , auto const b = Bounds()
         , auto const r = Ray(arg.origin, arg.direction)
         , float t1 = 0.f
         , float t2 = 0.f)
    THEN( b.Intersects(r, t1, t2)
        , t1 == arg.t1
        , t2 == arg.t2 )
}

PSCENARIO(TestArg, "A ray misses a cube", "shape,cube")
{
    PARAMS( { Point(-2.f, 0.f, 0.f), Vector(0.2673f, 0.5345f, 0.8018f), 0.f, 0.f }
          , { Point(0.f,-2.f, 0.f), Vector(0.8018f, 0.2673f, 0.5345f), 0.f, 0.f }
          , { Point(0.f, 0.f,-2.f), Vector(0.5345f, 0.8018f, 0.2673f), 0.f, 0.f }
          , { Point(2.f, 0.f, 2.f), Vector(0.f, 0.f,-1.f), 0.f, 0.f }
          , { Point(0.f, 2.f, 2.f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , { Point(2.f, 2.f, 0.f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const b = Bounds()
         , auto const arg = GetParam()
         , auto const r = Ray(arg.origin, arg.direction)
         , float t1 = 0.f
         , float t2 = 0.f )
    THEN( !b.Intersects(r, t1, t2) )
}

PSCENARIO(TestArg, "A ray intersects a non-default AABB", "AABB")
{
    PARAMS( { Point(6.f, 1.5f, 1.f), Vector(-1.f, 0.f, 0.f), 4.f, 6.f }
          , { Point(-4.f, 1.5f, 1.f), Vector(1.f, 0.f, 0.f), 4.f, 6.f }
          , { Point(1.5f, 6.f, 1.f), Vector(0.f, -1.f, 0.f), 4.f, 6.f }
          , { Point(1.5f, -4.f, 1.f), Vector(0.f, 1.f, 0.f), 4.f, 6.f }
          , { Point(1.5f, 1.f, 6.f), Vector(0.f, 0.f, -1.f), 4.f, 6.f }
          , { Point(1.5f, 1.f, -4.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f }
          , { Point(1.f, 1.5f, 1.f), Vector(0.f, 0.f, 1.f), -1.f, 1.f } )
    GIVEN( auto const arg = GetParam()
         , auto const b = Bounds(Point(0.f, 0.f, 0.f), Point(2.f, 2.f, 2.f))
         , auto const r = Ray(arg.origin, arg.direction)
         , float t1 = 0.f
         , float t2 = 0.f )
    THEN( b.Intersects(r, t1, t2)
        , t1 == arg.t1
        , t2 == arg.t2 )
}
