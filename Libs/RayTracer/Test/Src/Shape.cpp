#include "Ray.h"
#include "Shape.h"
#include "Transformations.h"

#include <Testing.h>

class TestShape : public Shape
{
public:
    TestShape()
        : m_localRay(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f))
    {}

    void Intersect(Ray const& ray, std::vector<Intersection>& xs) const override
    {
        const_cast<TestShape*>(this)->m_localRay = ray;
    }

    Tuple NormalAtLocal(Tuple const& point) const override
    {
        return Vector(point[0], point[1], point[2]);
    }

    Ray m_localRay;
};

SCENARIO("A sphere's default transformation", "[Shapes]")
{
    GIVEN_1(
        auto const s = std::make_shared<TestShape>();
    REQUIRE_,
        s->Transform() == Mat44::Identity() )
}

SCENARIO("Changing a sphere's transformation", "[Shapes]")
{
    GIVEN_2(
        auto s = std::make_shared<TestShape>();
        auto const t = matrix::Translation(2.f, 3.f, 4.f);
    WHEN_,
        s->SetTransform(t);
    REQUIRE_,
        s->Transform() == t )
}

SCENARIO("Shape default material", "[Shapes]")
{
    GIVEN_2(
        auto s = std::make_shared<TestShape>();
    WHEN_,
        auto const m = s->GetMaterial();
    REQUIRE_,
        m == Material() )
}

SCENARIO("Assigning a material to a shape", "[Shapes]")
{
    GIVEN_2(
        auto s = std::make_shared<TestShape>();
    WHEN_,
        auto m = s->GetMaterial();
        m.Ambient(1.f);
        s->SetMaterial(m);
    REQUIRE_,
        s->GetMaterial() == m )
}

SCENARIO("Intersecting a scaled shape with a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = std::make_shared<TestShape>();
    WHEN_,
        s->SetTransform(matrix::Scaling(2.f, 2.f, 2.f));
        std::vector<Intersection> xs;
        r.Intersect(s, xs);
    REQUIRE_,
        s->m_localRay.Origin() == Point(0.f, 0.f, -2.5f),
        s->m_localRay.Direction() == Vector(0.f, 0.f, .5f) )
}

SCENARIO("Intersecting a translated shape with a ray", "[Math]")
{
    GIVEN_2(
        auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
        auto s = std::make_shared<TestShape>();
    WHEN_,
        s->SetTransform(matrix::Translation(5.f, 0.f, 0.f));
        std::vector<Intersection> xs;
        r.Intersect(s, xs);
    REQUIRE_,
        s->m_localRay.Origin() == Point(-5.f, 0.f, -5.f),
        s->m_localRay.Direction() == Vector(0.f, 0.f, 1.f) )
}

SCENARIO("Computing the normal on a translated shape", "[Geometry]")
{
    GIVEN_2(
        auto s = std::make_shared<TestShape>();
    WHEN_,
        s->SetTransform(matrix::Translation(0.f, 1.f, 0.f));
        auto const n = s->NormalAt(Point(0.f, 1.70711f, -0.70711f));
    REQUIRE_,
        n == Vector(0.f, 0.70711f, -0.70711f) )
}

SCENARIO("Computing the normal on a transformed shape", "[Geometry]")
{
    GIVEN_2(
        auto s = std::make_shared<TestShape>();
        float const coord = std::sqrt(2.f) / 2.f;
    WHEN_,
        s->SetTransform(matrix::Scaling(1.f, 0.5f, 1.f) * matrix::RotationZ(PI / 5.f));
        auto const n = s->NormalAt(Point(0.f, coord, -coord));
    REQUIRE_,
        n == Vector(0.f, 0.97014f, -0.24254f) )
}
