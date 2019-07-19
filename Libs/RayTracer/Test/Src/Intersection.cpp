#include "Intersection.h"
#include "Ray.h"
#include "Sphere.h"
#include "Util.h"

#include <Testing.h>

SCENARIO("An intersection encapsulates the distance from the ray origin and the object intersected", "[Math]")
{
    GIVEN_2(
        auto const s = std::make_shared<Sphere>();
    WHEN_,
        auto const i = Intersection(3.5f, s)
        REQUIRE_,
        i.Distance() == 3.5f,
        i.Object() == s )
}

SCENARIO("Agregating intersections", "[math]")
{
    GIVEN_2(
        auto const s = std::make_shared<Sphere>();
        auto const i1 = Intersection(1.f, s);
        auto const i2 = Intersection(2.f, s);
    WHEN_,
        auto const xs = Intersections(i1, i2);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Distance() == 1.f,
        xs[1].Distance() == 2.f)
}

SCENARIO("Intersect sets the object on the intersection", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto const s = std::make_shared<Sphere>();
    WHEN_,
        auto xs = r.Intersect(s);
    REQUIRE_,
        xs.size() == 2,
        xs[0].Object() == s,
        xs[1].Object() == s)
}

SCENARIO("The hit, when all intersections have positive t", "[math]")
{
    GIVEN_2(
        auto const s = std::make_shared<Sphere>();
        auto const i1 = Intersection(1.f, s);
        auto const i2 = Intersection(2.f, s);
        auto const xs = Intersections(i1, i2);
    WHEN_,
        auto const i = Hit(xs);
    REQUIRE_,
        i == i1 )
}

SCENARIO("The hit, when some intersections have negative t", "[math]")
{
    GIVEN_2(
        auto const s = std::make_shared<Sphere>();
        auto const i1 = Intersection(-1.f, s);
        auto const i2 = Intersection(1.f, s);
        auto const xs = Intersections(i1, i2);
    WHEN_,
        auto const i = Hit(xs);
    REQUIRE_,
        i == i2 )
}

SCENARIO("The hit, when all intersections have negative t", "[math]")
{
    GIVEN_2(
        auto const s = std::make_shared<Sphere>();
        auto const i1 = Intersection(-2.f, s);
        auto const i2 = Intersection(-1.f, s);
        auto const xs = Intersections(i1, i2);
    WHEN_,
        auto const i = Hit(xs);
    REQUIRE_,
        i.Distance() == 0.f,
        i.Object() == nullptr )
}

SCENARIO("The hit is always the lowest nonnegative intersection", "[math]")
{
    GIVEN_2(
        auto const s = std::make_shared<Sphere>();
        auto const i1 = Intersection(5.f, s);
        auto const i2 = Intersection(7.f, s);
        auto const i3 = Intersection(-3.f, s);
        auto const i4 = Intersection(2.f, s);
        auto const xs = Intersections(i1, i2, i3, i4);
    WHEN_,
        auto const i = Hit(xs);
    REQUIRE_,
        i == i4 )
}

SCENARIO("The hit should offset the point", "[math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = std::make_shared<Sphere>();
        s->SetTransform(matrix::Translation(0.f, 0.f, 1.f));
        auto const i = Intersection(5.f, s);
    WHEN_,
        auto const iData = r.Precompute(i);
    REQUIRE_,
        iData.m_overPoint[2] < -(EPSILON/2.f),
        iData.m_point[2] > iData.m_overPoint[2] )
}
