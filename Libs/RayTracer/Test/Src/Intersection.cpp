#include "Intersection.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "TestHelpers.h"
#include "Util.h"

#include <Beddev/Beddev.h>

SCENARIO("An intersection encapsulates the distance from the ray origin and the object intersected", "math")
{
    GIVEN( auto const s = std::make_shared<Sphere>() )
    WHEN( auto const i = Intersection(3.5f, s) )
    THEN( i.Distance() == 3.5f
        , i.Object() == s )
}

SCENARIO("Agregating intersections", "math")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const i1 = Intersection(1.f, s)
         , auto const i2 = Intersection(2.f, s) )
    WHEN( auto const xs = Intersections(i1, i2) )
    THEN( xs.size() == 2
        , xs[0].Distance() == 1.f
        , xs[1].Distance() == 2.f )
}

SCENARIO("Intersect sets the object on the intersection", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const s = std::make_shared<Sphere>() )
    WHEN( std::vector<Intersection> xs = {}
        , r.Intersect(s, xs) )
    THEN( xs.size() == 2
        , xs[0].Object() == s
        , xs[1].Object() == s )
}

SCENARIO("The hit, when all intersections have positive t", "math")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const i1 = Intersection(1.f, s)
         , auto const i2 = Intersection(2.f, s)
         , auto const xs = Intersections(i1, i2) )
    WHEN( auto const i = Hit(xs) )
    THEN( i == i1 )
}

SCENARIO("The hit, when some intersections have negative t", "math")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const i1 = Intersection(-1.f, s)
         , auto const i2 = Intersection(1.f, s)
         , auto const xs = Intersections(i1, i2) )
    WHEN( auto const i = Hit(xs) )
    THEN( i == i2 )
}

SCENARIO("The hit, when all intersections have negative t", "math")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const i1 = Intersection(-2.f, s)
         , auto const i2 = Intersection(-1.f, s)
         , auto const xs = Intersections(i1, i2) )
    WHEN( auto const i = Hit(xs) )
    THEN( i.Distance() == 0.f
        , i.Object() == nullptr )
}

SCENARIO("The hit is always the lowest nonnegative intersection", "math")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const i1 = Intersection(5.f, s)
         , auto const i2 = Intersection(7.f, s)
         , auto const i3 = Intersection(-3.f, s)
         , auto const i4 = Intersection(2.f, s)
         , auto const xs = Intersections(i1, i2, i3, i4) )
    WHEN( auto const i = Hit(xs) )
    THEN( i == i4 )
}

SCENARIO("The hit should offset the point", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Translation(0.f, 0.f, 1.f))
         , auto const i = Intersection(5.f, s) )
    WHEN( auto const iData = r.Precompute(i) )
    THEN( iData.m_overPoint[2] < -(EPSILON/2.f)
        , iData.m_point[2] > iData.m_overPoint[2] )
}

SCENARIO("Precomputing the reflection vector", "reflection")
{
    GIVEN( auto const shape = std::make_shared<Plane>()
         , auto const r = Ray(Point(0.f, 1.f, -1.f), Vector(0.f, -SQRT2OVR2, SQRT2OVR2))
         , auto const i = Intersection(SQRT2, shape) )
    WHEN( auto comps = r.Precompute(i) )
    THEN( comps.m_reflectv == Vector(0.f, SQRT2OVR2, SQRT2OVR2) )
}

SCENARIO("Finding n1 and n2 at various intersections", "refraction")
{
    GIVEN( auto a = GlassySphere()
         , a->SetTransform(matrix::Scaling(2.f, 2.f, 2.f))
         , a->ModifyMaterial().RefractiveIndex(1.5f)
         , auto b = GlassySphere()
         , b->SetTransform(matrix::Translation(0.f, 0.f, -.25f))
         , b->ModifyMaterial().RefractiveIndex(2.f)
         , auto c = GlassySphere()
         , c->SetTransform(matrix::Translation(0.f, 0.f, .25f))
         , c->ModifyMaterial().RefractiveIndex(2.5f)
         , auto const r = Ray(Point(0.f, 0.f, -4.f), Vector(0.f, 0.f, 1.f))
         , auto const xs = Intersections(
             Intersection{ 2.f, a }, Intersection{ 2.75f, b },
             Intersection{ 3.25f, c }, Intersection{ 4.75f, b },
             Intersection{ 5.25f, c }, Intersection{6.f, a}) )
    WHEN( auto const comps0 = r.Precompute(xs[0], xs)
        , auto const comps1 = r.Precompute(xs[1], xs)
        , auto const comps2 = r.Precompute(xs[2], xs)
        , auto const comps3 = r.Precompute(xs[3], xs)
        , auto const comps4 = r.Precompute(xs[4], xs)
        , auto const comps5 = r.Precompute(xs[5], xs) )
    THEN( comps0.m_n1 == 1.f, comps0.m_n2 == 1.5f
        , comps1.m_n1 == 1.5f, comps1.m_n2 == 2.f
        , comps2.m_n1 == 2.f, comps2.m_n2 == 2.5f
        , comps3.m_n1 == 2.5f, comps3.m_n2 == 2.5f
        , comps4.m_n1 == 2.5f, comps4.m_n2 == 1.5f
        , comps5.m_n1 == 1.5f, comps5.m_n2 == 1.f )
}

SCENARIO("The under point is offset below the surface", "refraction")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto s = GlassySphere()
         , s->SetTransform(matrix::Translation(0.f, 0.f, 1.f))
         , auto const i = Intersection(5.f, s)
         , auto const xs = Intersections(i) )
    WHEN( auto const comps = r.Precompute(i, xs) )
    THEN( comps.m_underPoint.Z() > (EPSILON / 2.f)
        , comps.m_point.Z() < comps.m_underPoint.Z() )
}

SCENARIO("The Schlick aproximation under total internal reflection", "refraction")
{
    GIVEN( auto s = GlassySphere()
         , auto const r = Ray(Point(0.f, 0.f, SQRT2OVR2), Vector(0.f, 1.f, 0.f))
         , auto const xs = Intersections(Intersection{-SQRT2OVR2, s}, Intersection{SQRT2OVR2, s}) )
    WHEN( auto const comps = r.Precompute(xs[1], xs)
        , auto const reflectance = Schlick(comps) )
    THEN( reflectance == 1.0f )
}

SCENARIO("The Schlick aproximation with a perpendicular viewing angle", "refraction")
{
    GIVEN( auto s = GlassySphere()
         , auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f))
         , auto const xs = Intersections(Intersection{ -1.f, s }, Intersection{ 1.f, s }) )
    WHEN( auto const comps = r.Precompute(xs[1], xs)
        , auto const reflectance = Schlick(comps) )
    THEN( Equals(reflectance, .04f) )
}

SCENARIO("The Schlick aproximation with small angle and n2 > n1", "refraction")
{
    GIVEN( auto s = GlassySphere()
         , auto const r = Ray(Point(0.f, .99f, -2.f), Vector(0.f, 0.f, 1.f))
         , auto const xs = Intersections(Intersection{ 1.8589f, s }) )
    WHEN( auto const comps = r.Precompute(xs[0], xs)
        , auto const reflectance = Schlick(comps) )
    THEN( Equals(reflectance, .48873f) )
}
