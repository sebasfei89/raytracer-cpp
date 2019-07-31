#include <RayTracer/Plane.h>
#include <RayTracer/Ray.h>

#include <Beddev/Beddev.h>

SCENARIO("The normal of a plane is constant everywhere", "shapes")
{
    GIVEN( Plane const p )
    WHEN( auto const n1 = p.NormalAtLocal(Point(0.f, 0.f, 0.f))
        , auto const n2 = p.NormalAtLocal(Point(10.f, 0.f, -10.f))
        , auto const n3 = p.NormalAtLocal(Point(-5.f, 0.f, 150.f)) )
    THEN( n1 == Vector(0.f, 1.f, 0.f)
        , n2 == Vector(0.f, 1.f, 0.f)
        , n3 == Vector(0.f, 1.f, 0.f) )
}

SCENARIO("Intersects a plane with a ray paralel to the plane", "shapes")
{
    GIVEN( Plane p
         , auto const r = Ray(Point(0.f, 10.f, 0.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( std::vector<Intersection> xs
        , p.Intersect(r, xs) )
    THEN( xs.size() == 0 )
}

SCENARIO("Intersects a plane with a coplanar ray", "shapes")
{
    GIVEN( Plane const p
         , auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( std::vector<Intersection> xs
        , p.Intersect(r, xs) )
    THEN( xs.size() == 0 )
}

SCENARIO("A ray intersecting a plane from above", "shapes")
{
    GIVEN( auto const p = std::make_shared<Plane>()
         , auto const r = Ray(Point(0.f, 1.f, 0.f), Vector(0.f, -1.f, 0.f)) )
    WHEN( std::vector<Intersection> xs
        , p->Intersect(r, xs) )
    THEN( xs.size() == 1
        , xs[0].Distance() == 1.f
        , xs[0].Object()->operator==(*p.get()) )
}

SCENARIO("A ray intersecting a plane from below", "shapes")
{
    GIVEN( auto const p = std::make_shared<Plane>()
         , auto const r = Ray(Point(0.f, -1.f, 0.f), Vector(0.f, 1.f, 0.f)) )
    WHEN( std::vector<Intersection> xs
        , p->Intersect(r, xs) )
    THEN( xs.size() == 1
        , xs[0].Distance() == 1.f
        , xs[0].Object()->operator==(*p.get()) )
}
