#include <RayTracer/Group.h>
#include <RayTracer/Ray.h>
#include <RayTracer/Shape.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/Util.h>

#include <Beddev/Beddev.h>

#include "TestHelpers.h"

SCENARIO("A sphere's default transformation", "shapes")
{
    GIVEN( auto const s = std::make_shared<TestShape>() )
    THEN( s->Transform() == Mat44::Identity() )
}

SCENARIO("Changing a sphere's transformation", "shapes")
{
    GIVEN( auto s = std::make_shared<TestShape>()
         , auto const t = matrix::Translation(2.f, 3.f, 4.f) )
    WHEN( s->SetTransform(t) )
    THEN( s->Transform() == t )
}

SCENARIO("Shape default material", "shapes")
{
    GIVEN( auto s = std::make_shared<TestShape>() )
    WHEN( auto const m = s->GetMaterial() )
    THEN( m == Material() )
}

SCENARIO("Assigning a material to a shape", "shapes")
{
    GIVEN( auto s = std::make_shared<TestShape>() )
    WHEN( auto m = s->GetMaterial()
        , m.Ambient(1.f)
        , s->SetMaterial(m) )
    THEN( s->GetMaterial() == m )
}

SCENARIO("Intersecting a scaled shape with a ray", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto s = std::make_shared<TestShape>() )
    WHEN( s->SetTransform(matrix::Scaling(2.f, 2.f, 2.f))
        , std::vector<Intersection> xs = {}
        , r.Intersect(s, xs) )
    THEN( s->m_localRay.Origin() == Point(0.f, 0.f, -2.5f)
        , s->m_localRay.Direction() == Vector(0.f, 0.f, .5f) )
}

SCENARIO("Intersecting a translated shape with a ray", "math")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto s = std::make_shared<TestShape>() )
    WHEN( s->SetTransform(matrix::Translation(5.f, 0.f, 0.f))
        , std::vector<Intersection> xs = {}
        , r.Intersect(s, xs) )
    THEN( s->m_localRay.Origin() == Point(-5.f, 0.f, -5.f)
        , s->m_localRay.Direction() == Vector(0.f, 0.f, 1.f) )
}

SCENARIO("Computing the normal on a translated shape", "shapes")
{
    GIVEN( auto s = std::make_shared<TestShape>() )
    WHEN( s->SetTransform(matrix::Translation(0.f, 1.f, 0.f))
        , auto const n = s->NormalAt(Point(0.f, 1.70711f, -0.70711f)) )
    THEN( n == Vector(0.f, 0.70711f, -0.70711f) )
}

SCENARIO("Computing the normal on a transformed shape", "shapes")
{
    GIVEN( auto s = std::make_shared<TestShape>() )
    WHEN( s->SetTransform(matrix::Scaling(1.f, 0.5f, 1.f) * matrix::RotationZ(PI / 5.f))
        , auto const n = s->NormalAt(Point(0.f, SQRT2OVR2, -SQRT2OVR2)) )
    THEN( n == Vector(0.f, 0.97014f, -0.24254f) )
}

SCENARIO("A shape has a parent attribute", "shapes")
{
    GIVEN( auto s = std::make_shared<TestShape>() )
    THEN( s->Parent() == nullptr )
}

SCENARIO("Converting a point from world to object space", "shapes")
{
    GIVEN( auto g1 = std::make_shared<Group>()
         , g1->SetTransform(matrix::RotationY(PIOVR2) )
         , auto g2 = std::make_shared<Group>()
         , g2->SetTransform(matrix::Scaling(2.f, 2.f, 2.f))
         , g1->AddChild(g2)
         , auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Translation(5.f, 0.f, 0.f))
         , g2->AddChild(s) )
    WHEN( auto const p = s->WorldToLocal(Point(-2.f, 0.f, -10.f)) )
    THEN( p == Point(0.f, 0.f, -1.f) )
}

SCENARIO("Converting a normal from object to world space", "shapes")
{
    GIVEN( auto g1 = std::make_shared<Group>()
         , g1->SetTransform(matrix::RotationY(PIOVR2) )
         , auto g2 = std::make_shared<Group>()
         , g2->SetTransform(matrix::Scaling(1.f, 2.f, 3.f))
         , g1->AddChild(g2)
         , auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Translation(5.f, 0.f, 0.f))
         , g2->AddChild(s)
         , auto const sqrt3ovr3 = std::sqrtf(3.f) / 3.f )
    WHEN( auto const n = s->NormalToWorld(Vector(sqrt3ovr3, sqrt3ovr3, sqrt3ovr3)) )
    THEN( n == Vector(0.2857f, 0.4286f, -0.8571f) )
}

SCENARIO("Finding the normal on a child object", "shapes")
{
    GIVEN( auto g1 = std::make_shared<Group>()
         , g1->SetTransform(matrix::RotationY(PIOVR2) )
         , auto g2 = std::make_shared<Group>()
         , g2->SetTransform(matrix::Scaling(1.f, 2.f, 3.f))
         , g1->AddChild(g2)
         , auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Translation(5.f, 0.f, 0.f))
         , g2->AddChild(s) )
    WHEN( auto const n = s->NormalAt(Point(1.7321f, 1.1547f, -5.5774f)) )
    THEN( n == Vector(0.2857f, 0.4286f, -0.8571f) )
}

SCENARIO("A shape's bounds", "shape")
{
    GIVEN( auto const ts = TestShape() )
    WHEN( auto const& b = ts.GetBounds() )
    THEN( b == Bounds() )
}
