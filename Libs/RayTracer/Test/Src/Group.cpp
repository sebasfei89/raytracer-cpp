#include "TestHelpers.h"

#include <Beddev/Beddev.h>

#include <RayTracer/Group.h>
#include <RayTracer/Sphere.h>

SCENARIO("Creating a new group", "shapes,groups")
{
    GIVEN( auto const g = std::make_shared<Group>() )
    THEN( g->Transform() == Mat44::Identity()
        , g->IsEmpty() )
}

SCENARIO("Adding a child to a group", "shapes,groups")
{
    GIVEN( auto g = std::make_shared<Group>()
         , auto s = std::make_shared<TestShape>() )
    WHEN( g->AddChild(s) )
    THEN( !g->IsEmpty()
        , g->Includes(s)
        , s->Parent() == g )
}

SCENARIO("Intersecting a ray with an empty group", "shapes,groups")
{
    GIVEN( auto g = std::make_shared<Group>()
         , auto r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( auto xs = std::vector<Intersection>();
        , r.Intersect(g, xs) )
    THEN( xs.empty() )
}

SCENARIO("Intersecting a ray with an nonempty group", "shapes,groups")
{
    GIVEN( auto g = std::make_shared<Group>()
         , auto s1 = std::make_shared<Sphere>()
         , auto s2 = std::make_shared<Sphere>()
         , s2->SetTransform(matrix::Translation(0.f, 0.f, -3.f))
         , auto s3 = std::make_shared<Sphere>()
         , s3->SetTransform(matrix::Translation(5.f, 0.f, 0.f))
         , g->AddChild(s1)
         , g->AddChild(s2)
         , g->AddChild(s3)
         , auto r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( auto xs = std::vector<Intersection>();
        , r.Intersect(g, xs) )
    THEN( xs.size() == 4
        , xs[0].Object() == s2
        , xs[1].Object() == s2
        , xs[2].Object() == s1
        , xs[3].Object() == s1 )
}

SCENARIO("Intersecting a transformed group", "shapes,groups")
{
    GIVEN( auto g = std::make_shared<Group>()
         , g->SetTransform(matrix::Scaling(2.f, 2.f, 2.f))
         , auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Translation(5.f, 0.f, 0.f))
         , g->AddChild(s)
         , auto r = Ray(Point(10.f, 0.f, -10.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( auto xs = std::vector<Intersection>();
        , r.Intersect(g, xs) )
    THEN( xs.size() == 2 )
}
