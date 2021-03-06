#include "TestHelpers.h"

#include <Beddev/Beddev.h>
#include <RayTracer/Shapes/Cone.h>
#include <RayTracer/Ray.h>

#include <numeric>
#include <tuple>

PSCENARIO(TestArg, "Intersecting a cone with a ray", "shape,cone")
{
    PARAMS({ Point(0.f, 0.f,-5.f), Vector( 0.f, 0.f, 1.f), 5.f, 5.f }
         , { Point(0.f, 0.f,-5.f), Vector( 1.f, 1.f, 1.f), 8.66025f, 8.66025f }
         , { Point(1.f, 1.f,-5.f), Vector(-.5f,-1.f, 1.f), 4.55006f, 49.44994f } )
    GIVEN( auto const arg = GetParam()
         , auto const cone = std::make_shared<Cone>()
         , auto const dir = arg.direction.Normalized()
         , auto const r = Ray(arg.origin, dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(cone, xs) )
    THEN( xs.size() == 2
        , Equals(xs[0].Distance(), arg.t1)
        , Equals(xs[1].Distance(), arg.t2) )
}

SCENARIO("Intersecting a cone with a ray parallel to one of its halves", "shape,cone")
{
    GIVEN( auto const cone = std::make_shared<Cone>()
         , auto const dir = Vector(0.f, 1.f, 1.f).Normalized()
         , auto const r = Ray(Point(0.f, 0.f, -1.f), dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(cone, xs) )
    THEN( xs.size() == 1
        , Equals(xs[0].Distance(), .35355f) )
}

PSCENARIO(TestArg, "Intersecting a cone's end caps", "shape,cone")
{
    PARAMS({ Point(0.f, 0.f,-5.f), Vector( 0.f, 1.f, 0.f), 0.f, 0.f }
         , { Point(0.f, 0.f,-.25f), Vector( 0.f, 1.f, 1.f), 2.f, 0.f }
         , { Point(0.f, 0.f,-.25f), Vector(0.f, 1.f, 0.f), 4.f, 0.f } )
    GIVEN( auto const arg = GetParam()
         , auto const cone = std::make_shared<Cone>(-.5f, .5f, true)
         , auto const dir = arg.direction.Normalized();
         , auto const r = Ray(arg.origin, dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(cone, xs) )
    THEN( xs.size() == arg.t1 )
}

PSCENARIO(TestArg, "Computing the normal vector on a cone", "shape,cone")
{
    PARAMS({ Point(0.f, 0.f, 0.f), Vector( 0.f, 0.f, 0.f), 0.f, 0.f }
         , { Point(1.f, 1.f, 1.f), Vector( 1.f, -SQRT2, 1.f), 0.f, 0.f }
         , { Point(-1.f, -1.f, 0.f), Vector(-1.f, 1.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const arg = GetParam()
         , auto const cone = std::make_shared<Cone>()
         , auto const n = cone->NormalAtLocal(arg.origin) )
    THEN( n == arg.direction )
}

SCENARIO("A noncapped cone bounds", "shape,cone")
{
    GIVEN( auto const cone = std::make_shared<Cone>() )
    WHEN( auto const& b = cone->GetBounds() )
    THEN( b == Bounds(Point(-INF, -INF, -INF), Point(INF, INF, INF)) )
}

SCENARIO("A one-side capped cone bounds", "shape,cone")
{
    GIVEN( auto const cone = std::make_shared<Cone>(0.f, INF) )
    WHEN( auto const& b = cone->GetBounds() )
    THEN( b == Bounds(Point(-INF, 0.f, -INF), Point(INF, INF, INF)) )
}

using CapsT = std::pair<float, float>;
PSCENARIO(CapsT, "A capped cone bounds", "shape,cone")
{
    PARAMS( std::make_pair(-1.5f, 3.5f)
          , std::make_pair(-3.5f, 1.5f) )
    GIVEN( auto const cone = std::make_shared<Cone>(GetParam().first, GetParam().second) )
    WHEN( auto const& b = cone->GetBounds() )
    THEN( b == Bounds(Point(-3.5f, GetParam().first, -3.5f), Point(3.5f, GetParam().second, 3.5f)))
}
