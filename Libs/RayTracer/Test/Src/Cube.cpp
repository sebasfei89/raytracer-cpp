#include "TestHelpers.h"

#include <Beddev/Beddev.h>

#include <RayTracer/Cube.h>
#include <RayTracer/Ray.h>

PSCENARIO(TestArg, "A ray intersects a cube", "shape,cube")
{
    PARAMS( { Point(5.f, .5f, 0.f), Vector(-1.f, 0.f, 0.f), 4.f, 6.f }
          , { Point(-5.f, .5f, 0.f), Vector(1.f, 0.f, 0.f), 4.f, 6.f }
          , { Point(.5f, 5.f, 0.f), Vector(0.f, -1.f, 0.f), 4.f, 6.f }
          , { Point(.5f, -5.f, 0.f), Vector(0.f, 1.f, 0.f), 4.f, 6.f }
          , { Point(.5f, 0.f, 5.f), Vector(0.f, 0.f, -1.f), 4.f, 6.f }
          , { Point(.5f, 0.f, -5.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f }
          , { Point(0.f, .5f, 0.f), Vector(0.f, 0.f, 1.f), -1.f, 1.f } )
    GIVEN( auto const arg = GetParam()
         , auto const c = std::make_shared<Cube>()
         , auto const r = Ray(arg.origin, arg.direction)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(c, xs) )
    THEN( xs.size() == 2
        , xs[0].Distance() == arg.t1
        , xs[1].Distance() == arg.t2 )
}

PSCENARIO(TestArg, "A ray misses a cube", "shape,cube")
{
    PARAMS( { Point(-2.f, 0.f, 0.f), Vector(0.2673f, 0.5345f, 0.8018f), 0.f, 0.f }
          , { Point(0.f,-2.f, 0.f), Vector(0.8018f, 0.2673f, 0.5345f), 0.f, 0.f }
          , { Point(0.f, 0.f,-2.f), Vector(0.5345f, 0.8018f, 0.2673f), 0.f, 0.f }
          , { Point(2.f, 0.f, 2.f), Vector(0.f, 0.f,-1.f), 0.f, 0.f }
          , { Point(0.f, 2.f, 2.f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , { Point(2.f, 2.f, 0.f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const c = std::make_shared<Cube>()
         , auto const arg = GetParam()
         , auto const r = Ray(arg.origin, arg.direction)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(c, xs) )
    THEN( xs.size() == 0 )
}

PSCENARIO(TestArg, "The normal on the surface of a cube", "shape,cube")
{
    PARAMS( { Point(1.f, .5f,-.8f), Vector(1.f, 0.f, 0.f), 0.f, 0.f }
          , { Point(-1.f,-.2f, .9f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f }
          , { Point(-.4f, 1.f,-.1f), Vector(0.f, 1.f, 0.f), 0.f, 0.f }
          , { Point(.3f,-1.f,-.7f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , { Point(-.6f, .3f, 1.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f }
          , { Point(.4f, .4f,-1.f), Vector(0.f, 0.f,-1.f), 0.f, 0.f }
          , { Point(1.f, 1.f, 1.f), Vector(1.f, 0.f, 0.f), 0.f, 0.f }
          , { Point(-1.f,-1.f,-1.f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const c = std::make_shared<Cube>()
         , auto const arg = GetParam()
         , auto const p = arg.origin )
    WHEN( auto const n = c->NormalAtLocal(p) )
    THEN( n == arg.direction )
}

SCENARIO("A cube bounds", "shape,cube")
{
    GIVEN( auto const c = std::make_shared<Cube>() )
    WHEN( auto const& b = c->GetBounds() )
    THEN( b == Bounds() )
}
