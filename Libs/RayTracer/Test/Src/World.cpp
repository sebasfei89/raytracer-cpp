#include "Ray.h"
#include "Sphere.h"
#include "TestHelpers.h"
#include "Transformations.h"
#include "World.h"

#include <Testing.h>

SCENARIO("Creating a world", "[Scene]")
{
    GIVEN_1(
        auto const w = World();
    REQUIRE_,
        w.Objects().size() == 0,
        w.Lights().size() == 0 )
}

SCENARIO("The default world", "[Scene]")
{
    GIVEN_2(
        auto const light = PointLight(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
        auto s1 = std::make_shared<Sphere>();
        s1->ModifyMaterial().SetColor(Color(.8f, 1.f, .6f));
        s1->ModifyMaterial().Diffuse(.7f);
        s1->ModifyMaterial().Specular(.2f);
        auto s2 = std::make_shared<Sphere>();
        s2->SetTransform(matrix::Scaling(.5f, .5f, .5f));
    WHEN_,
        auto const w = DefaultWorld();
    REQUIRE_,
        w.Lights().size() == 1,
        w.Lights()[0] == light,
        w.Objects().size() == 2,
        w.Objects()[0]->operator==(*s1.get()),
        w.Objects()[1]->operator==(*s2.get()) )
}

SCENARIO("Intersect a world with a ray", "[Scene]")
{
    GIVEN_2(
        auto const w = DefaultWorld();
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    WHEN_,
        auto const xs = r.Intersect(w);
    REQUIRE_,
        xs.size() == 4,
        xs[0].Distance() == 4.f, 
        xs[1].Distance() == 4.5f,
        xs[2].Distance() == 5.5f,
        xs[3].Distance() == 6.f )
}

SCENARIO("Precomputing the state of an interdection", "[Scene]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = std::make_shared<Sphere>();
        auto const i = Intersection(4.f, s);
    WHEN_,
        auto const c = r.Precompute(i);
    REQUIRE_,
        c.m_distance == i.Distance(),
        c.m_object == i.Object(),
        c.m_point == Point(0.f, 0.f, -1.f),
        c.m_eyev == Vector(0.f, 0.f, -1.f),
        c.m_normalv == Vector(0.f, 0.f, -1.f) )
}

SCENARIO("The hit, when an intersection occurs on the outside", "[Scene]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = std::make_shared<Sphere>();
        auto const i = Intersection(4.f, s);
    WHEN_,
        auto const c = r.Precompute(i);
    REQUIRE_,
        c.m_inside == false )
}

SCENARIO("The hit, when an intersection occurs on the inside", "[Scene]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
        auto s = std::make_shared<Sphere>();
        auto const i = Intersection(1.f, s);
    WHEN_,
        auto const c = r.Precompute(i);
    REQUIRE_,
        c.m_point == Point(0.f, 0.f, 1.f),
        c.m_eyev == Vector(0.f, 0.f, -1.f),
        c.m_normalv == Vector(0.f, 0.f, -1.f),
        c.m_inside == true )
}

SCENARIO("Shading and intersection", "[Scene]")
{
    GIVEN_2(
        auto const w = DefaultWorld();
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto const& s = w.Objects()[0];
        auto const i = Intersection(4.f, s);
    WHEN_,
        auto const comps = r.Precompute(i);
        auto const c = w.ShadeHit(comps);
    REQUIRE_,
        c == Color(0.38066f, 0.47583f, 0.2855f) )
}

SCENARIO("Shading and intersection from the inside", "[Scene]")
{
    GIVEN_2(
        auto w = DefaultWorld();
        w.ModifyLights()[0] = PointLight(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f));
        auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
        auto const& s = w.Objects()[1];
        auto const i = Intersection(.5f, s);
    WHEN_,
        auto const comps = r.Precompute(i);
        auto const c = w.ShadeHit(comps);
    REQUIRE_,
        c == Color(0.90498f, 0.90498f, 0.90498f) )
}

SCENARIO("The color when a ray misses", "[Scene]")
{
    GIVEN_2(
        auto w = DefaultWorld();
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 1.f, 0.f));
    WHEN_,
        auto const c = w.ColorAt(r);
    REQUIRE_,
        c == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The color when a ray hits", "[Scene]")
{
    GIVEN_2(
        auto w = DefaultWorld();
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    WHEN_,
        auto const c = w.ColorAt(r);
    REQUIRE_,
        c == Color(0.38066f, 0.47583f, 0.2855f))
}

SCENARIO("The color with an intersection behind the ray", "[Scene]")
{
    GIVEN_2(
        auto w = DefaultWorld();
        auto& s1 = w.ModifyObjects()[0];
        s1->ModifyMaterial().Ambient(1.f);
        auto& s2 = w.ModifyObjects()[1];
        s2->ModifyMaterial().Ambient(1.f);
        auto const r = Ray(Point(0.f, 0.f, 0.75f), Vector(0.f, 0.f, -1.f));
    WHEN_,
        auto const c = w.ColorAt(r);
    REQUIRE_,
        c == s2->GetMaterial().GetColor() )
}

SCENARIO("There is no shadow when nothing is collinear with the point and light", "[Shadows]")
{
    GIVEN_1(
        auto const w = DefaultWorld();
        auto const point = Point(0.f, 10.f, 0.f);
    REQUIRE_,
        !w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("The shadow when an object is between the point and the light", "[Shadows]")
{
    GIVEN_1(
        auto const w = DefaultWorld();
        auto const point = Point(10.f, -10.f, 10.f);
    REQUIRE_,
        w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("There is no shadow when an object is behind the light", "[Shadows]")
{
    GIVEN_1(
        auto const w = DefaultWorld();
        auto const point = Point(-20.f, 20.f, -20.f);
    REQUIRE_,
        !w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("There is no shadow when an object is behind the point", "[Shadows]")
{
    GIVEN_1(
        auto const w = DefaultWorld();
        auto const point = Point(-2.f, 2.f, -2.f);
    REQUIRE_,
        !w.IsShadowed(point, w.Lights()[0]))
}

SCENARIO("ShadeHit is given an intersection in shadow", "[Scene]")
{
    GIVEN_2(
        auto w = World();
        w.Add(PointLight(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f)));
        auto s1 = std::make_shared<Sphere>();
        w.Add(s1);
        auto s2 = std::make_shared<Sphere>();
        s2->SetTransform(matrix::Translation(0.f, 0.f, 10.f));
        w.Add(s2);
        auto const r = Ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
        auto const i = Intersection(4.f, s2);
    WHEN_,
        auto const comps = r.Precompute(i);
        auto const c = w.ShadeHit(comps);
    REQUIRE_,
        c == Color(0.1f, 0.1f, 0.1f) )
}
