#include <RayTracer/Ray.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/Transformations.h>

#include <Beddev/Beddev.h>

SCENARIO("Creating and querying a ray", "math")
{
    GIVEN( auto const origin = Point(1.f, 2.f, 3.f)
         , auto const direction = Vector(4.f, 5.f, 6.f) )
    WHEN( auto const r = Ray(origin, direction) )
    THEN( r.Origin() == origin
        , r.Direction() == direction )
}

SCENARIO("Computing a point from a distance", "math")
{
    GIVEN( auto const r = Ray(Point(2.f, 3.f, 4.f), Vector(1.f, 0.f, 0.f)) )
    THEN( r.Position(0.f) == Point(2.f, 3.f, 4.f)
        , r.Position(1.f) == Point(3.f, 3.f, 4.f)
        , r.Position(-1.f) == Point(1.f, 3.f, 4.f)
        , r.Position(2.5f) == Point(4.5f, 3.f, 4.f) )
}

SCENARIO("A ray intersects a sphere at two points", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const s = std::make_shared<Sphere>() )
    WHEN( std::vector<Intersection> xs
        , r.Intersect(s, xs) )
    THEN( xs.size() == 2
        , xs[0].Distance() == 4.0f
        , xs[1].Distance() == 6.0f )
}

SCENARIO("A ray intersects a sphere at a tangent", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 1.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const s = std::make_shared<Sphere>() )
    WHEN( std::vector<Intersection> xs
        , r.Intersect(s, xs) )
    THEN( xs.size() == 2
        , xs[0].Distance() == 5.0f
        , xs[1].Distance() == 5.0f )
}

SCENARIO("A ray misses a sphere", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const s = std::make_shared<Sphere>() )
    WHEN( std::vector<Intersection> xs
        , r.Intersect(s, xs) )
    THEN( xs.size() == 0 )
}

SCENARIO("A ray originates inside a sphere", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f))
         , auto const s = std::make_shared<Sphere>() )
    WHEN( std::vector<Intersection> xs
        , r.Intersect(s, xs) )
    THEN( xs.size() == 2
        , xs[0].Distance() == -1.0f
        , xs[1].Distance() == 1.0f )
}

SCENARIO("A sphere is behind a ray", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f))
         , auto const s = std::make_shared<Sphere>() )
    WHEN( std::vector<Intersection> xs
        , r.Intersect(s, xs) )
    THEN( xs.size() == 2
        , xs[0].Distance() == -6.0f
        , xs[1].Distance() == -4.0f )
}

SCENARIO("Translating a ray", "math")
{
    GIVEN( auto const r = Ray(Point(1.f, 2.f, 3.f), Vector(0.f, 1.f, 0.f))
         , auto const m = matrix::Translation(3.f, 4.f, 5.f) )
    WHEN( auto const r2 = m * r )
    THEN( r2.Origin() == Point(4.f, 6.f, 8.f)
        , r2.Direction() == Vector(0.f, 1.f, 0.f) )
}

SCENARIO("Scaling a ray", "math")
{
    GIVEN( auto const r = Ray(Point(1.f, 2.f, 3.f), Vector(0.f, 1.f, 0.f))
         , auto const m = matrix::Scaling(2.f, 3.f, 4.f) )
    WHEN( auto const r2 = m * r )
    THEN( r2.Origin() == Point(2.f, 6.f, 12.f)
        , r2.Direction() == Vector(0.f, 3.f, 0.f) )
}
