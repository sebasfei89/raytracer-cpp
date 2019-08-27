#include "TestHelpers.h"

#include <Beddev/Beddev.h>
#include <RayTracer/Cylinder.h>
#include <RayTracer/Ray.h>

#include <numeric>

PSCENARIO(TestArg, "A ray missis a cylinder", "shape,cylinder")
{
    PARAMS( { Point(1.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f), 0.f, 0.f }
          , { Point(0.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f), 0.f, 0.f }
          , { Point(0.f, 0.f,-5.f), Vector(1.f, 1.f, 1.f), 0.f, 0.f } )
    GIVEN( auto const arg = GetParam()
         , auto const cyl = std::make_shared<Cylinder>()
         , auto const dir = arg.direction.Normalized();
         , auto const r = Ray(arg.origin, dir)
         , std::vector<Intersection> xs = {})
    WHEN( r.Intersect(cyl, xs) )
    THEN( xs.size() == 0 )
}

PSCENARIO(TestArg, "A ray strikes a cylinder", "shape,cylinder")
{
    PARAMS( { Point(1.f, 0.f,-5.f), Vector(0.f, 0.f, 1.f), 5.f, 5.f }
          , { Point(0.f, 0.f,-5.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f }
          , { Point(.5f, 0.f,-5.f), Vector(.1f, 1.f, 1.f), 6.80798f, 7.08872f } )
    GIVEN( auto const arg = GetParam()
         , auto const cyl = std::make_shared<Cylinder>()
         , auto const dir = arg.direction.Normalized();
         , auto const r = Ray(arg.origin, dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(cyl, xs) )
    THEN( xs.size() == 2
        , Equals(xs[0].Distance(), arg.t1)
        , Equals(xs[1].Distance(), arg.t2 ))
}

PSCENARIO(TestArg, "Normal vector on a cylinder", "shape,cylinder")
{
    PARAMS( { Point(1.f, 0.f, 0.f), Vector(1.f, 0.f, 0.f), 0.f, 0.f }
          , { Point(0.f, 5.f,-1.f), Vector(0.f, 0.f,-1.f), 0.f, 0.f }
          , { Point(0.f,-2.f, 1.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f}
          , { Point(-1.f,1.f, 0.f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const arg = GetParam()
         , auto const cyl = std::make_shared<Cylinder>() )
    WHEN( auto const n = cyl->NormalAtLocal(arg.origin) )
    THEN( n == arg.direction )
}

SCENARIO("The minimum and maximum for a cylinder", "shape,cylinder")
{
    GIVEN( auto const cyl = std::make_shared<Cylinder>()
         , auto const constrainedCyl = std::make_shared<Cylinder>(1.f, 2.f) )
    THEN( cyl->Minimum() == -INF
        , cyl->Maximum() == INF
        , constrainedCyl->Minimum() == 1.f
        , constrainedCyl->Maximum() == 2.f )
}

PSCENARIO(TestArg, "Intersecting a constrained cylinder", "shape,cylinder")
{
    PARAMS( { Point(0.f, 1.5f, 0.f), Vector(.1f, 1.f, 0.f), 0.f, 0.f }
          , { Point(0.f, 3.f,-5.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f }
          , { Point(0.f, 0.f,-5.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f}
          , { Point(0.f, 2.f,-5.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f }
          , { Point(0.f, 1.f,-5.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f }
          , { Point(0.f, 1.5f,-2.f), Vector(0.f, 0.f, 1.f), 2.f, 0.f } )
    GIVEN( auto const arg = GetParam()
         , auto const cyl = std::make_shared<Cylinder>(1.f, 2.f)
         , auto const dir = arg.direction.Normalized();
         , auto const r = Ray(arg.origin, dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(cyl, xs) )
    THEN( xs.size() == arg.t1 )
}

SCENARIO("The default closed value for a cylinder", "shape,cylinder")
{
    GIVEN( auto const cyl = std::make_shared<Cylinder>()
         , auto const closedCyl = std::make_shared<Cylinder>(1.f, 2.f, true) )
    THEN( !cyl->Closed()
        , closedCyl->Closed() )
}

PSCENARIO(TestArg, "Intersecting the caps of a closed cylinder", "shape,cylinder")
{
    PARAMS( { Point(0.f, 3.f, 0.f), Vector(0.f,-1.f, 0.f), 2.f, 0.f }
          , { Point(0.f, 3.f,-2.f), Vector(0.f,-1.f, 2.f), 2.f, 0.f }
          , { Point(0.f, 4.f,-2.f), Vector(0.f,-1.f, 1.f), 2.f, 0.f}
          , { Point(0.f, 0.f,-2.f), Vector(0.f, 1.f, 2.f), 2.f, 0.f }
          , { Point(0.f,-1.f,-2.f), Vector(0.f, 1.f, 1.f), 2.f, 0.f } )
    GIVEN( auto const arg = GetParam()
         , auto const cyl = std::make_shared<Cylinder>(1.f, 2.f, true)
         , auto const dir = arg.direction.Normalized();
         , auto const r = Ray(arg.origin, dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(cyl, xs) )
    THEN( xs.size() == arg.t1 )
}

PSCENARIO(TestArg, "The normal vector on a cylinder's end caps", "shape,cylinder")
{
    PARAMS( { Point(0.f, 1.f, 0.f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , { Point(.5f, 1.f, 0.f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , { Point(0.f, 1.f, .5f), Vector(0.f,-1.f, 0.f), 0.f, 0.f}
          , { Point(0.f, 2.f, 0.f), Vector(0.f, 1.f, 0.f), 0.f, 0.f }
          , { Point(.5f, 2.f, 0.f), Vector(0.f, 1.f, 0.f), 0.f, 0.f }
          , { Point(0.f, 2.f, .5f), Vector(0.f, 1.f, 0.f), 0.f, 0.f })
    GIVEN( auto const arg = GetParam()
         , auto const cyl = std::make_shared<Cylinder>(1.f, 2.f, true) )
    WHEN( auto const n = cyl->NormalAtLocal(arg.origin) )
    THEN( n == arg.direction )
}

SCENARIO("A noncapped cylinder bounds", "shape,cylinder")
{
    GIVEN( auto const cyl = std::make_shared<Cylinder>() )
    WHEN( auto const& b = cyl->GetBounds() )
    THEN( b == Bounds(Point(-1.f, -INF, -1.f), Point(1.f, INF, 1.f)) )
}

SCENARIO("A one-side cylinder cone bounds", "shape,cylinder")
{
    GIVEN( auto const cyl = std::make_shared<Cylinder>(0.f, INF) )
    WHEN( auto const& b = cyl->GetBounds() )
    THEN( b == Bounds(Point(-1.f, 0.f, -1.f), Point(1.f, INF, 1.f)) )
}

using CapsT = std::pair<float, float>;
PSCENARIO(CapsT, "A capped cylinder bounds", "shape,cylinder")
{
    PARAMS( std::make_pair(-1.5f, 3.5f)
          , std::make_pair(-3.5f, 1.5f) )
    GIVEN( auto const cyl = std::make_shared<Cylinder>(GetParam().first, GetParam().second) )
    WHEN( auto const& b = cyl->GetBounds() )
    THEN( b == Bounds(Point(-1.f, GetParam().first, -1.f), Point(1.f, GetParam().second, 1.f)) )
}
