#include "Color.h"
#include "Pattern.h"
#include "Sphere.h"
#include "Transformations.h"

#include <Testing.h>

class TestPattern : public IPattern
{
public:
    Color ColorAt(Tuple const& point) const override { return Color(point.X(), point.Y(), point.Z()); }
};

SCENARIO("The default pattern transformation", "[Patterns]")
{
    GIVEN_1(
        auto const p = TestPattern();
    REQUIRE_,
        p.Transform() == Mat44::Identity())
}

SCENARIO("Assigning a transformation", "[Patterns]")
{
    GIVEN_2(
        auto p = TestPattern();
    WHEN_,
        p.SetTransform(matrix::Translation(1.f, 2.f, 3.f));
    REQUIRE_,
        p.Transform() == matrix::Translation(1.f, 2.f, 3.f))
}

SCENARIO("A pattern with an object transformation", "[Patterns]")
{
    GIVEN_2(
        auto s = std::make_shared<Sphere>();
    s->SetTransform(matrix::Scaling(2.f, 2.f, 2.f));
    auto const p = TestPattern();
    WHEN_,
        auto const c = p.ShapeColorAt(s, Point(2.f, 3.f, 4.f));
    REQUIRE_,
        c == Color(1.f, 1.5f, 2.f))
}

SCENARIO("A pattern with a pattern transformation", "[Patterns]")
{
    GIVEN_2(
        auto s = std::make_shared<Sphere>();
    auto p = TestPattern();
    p.SetTransform(matrix::Scaling(2.f, 2.f, 2.f));
    WHEN_,
        auto const c = p.ShapeColorAt(s, Point(2.f, 3.f, 4.f));
    REQUIRE_,
        c == Color(1.f, 1.5f, 2.f))
}

SCENARIO("A pattern with both an object and a pattern transformation", "[Patterns]")
{
    GIVEN_2(
        auto s = std::make_shared<Sphere>();
    s->SetTransform(matrix::Scaling(2.f, 2.f, 2.f));
    auto p = TestPattern();
    p.SetTransform(matrix::Translation(.5f, 1.f, 1.5f));
    WHEN_,
        auto const c = p.ShapeColorAt(s, Point(2.5, 3.f, 3.5f));
    REQUIRE_,
        c == Color(.75f, .5f, .25f))
}

SCENARIO("A strip pattern is constant in y", "[Patterns]")
{
    GIVEN_1(
        auto const p = StripPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(0.f, 1.f, 0.f)) == Color::White(),
        p.ColorAt(Point(0.f, 2.f, 0.f)) == Color::White() )
}

SCENARIO("A strip pattern is constant in z", "[Patterns]")
{
    GIVEN_1(
        auto const p = StripPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(0.f, 0.f, 1.f)) == Color::White(),
        p.ColorAt(Point(0.f, 0.f, 2.f)) == Color::White() )
}

SCENARIO("A strip pattern alternates in x", "[Patterns]")
{
    GIVEN_1(
        auto const p = StripPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(.9f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(1.f, 0.f, 0.f)) == Color::Black(),
        p.ColorAt(Point(-.1f, 0.f, 0.f)) == Color::Black(),
        p.ColorAt(Point(-1.f, 0.f, 0.f)) == Color::Black(),
        p.ColorAt(Point(-1.1f, 0.f, 0.f)) == Color::White() )
}

SCENARIO("A gradient linearly interpolates between colors", "[Patterns]")
{
    GIVEN_1(
        auto const p = GradientPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(.25f, 0.f, 0.f)) == Color(.75f, .75f, .75f),
        p.ColorAt(Point(.5f, 0.f, 0.f)) == Color(.5f, .5f, .5f),
        p.ColorAt(Point(.75f, 0.f, 0.f)) == Color(.25f, .25f, .25f) )
}

SCENARIO("A ring should extend in both x and z", "[Patterns]")
{
    GIVEN_1(
        auto const p = RingPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(1.f, 0.f, 0.f)) == Color::Black(),
        p.ColorAt(Point(0.f, 0.f, 1.f)) == Color::Black(),
        p.ColorAt(Point(.708f, 0.f, .708f)) == Color::Black() )
}

SCENARIO("Checkers should repeat in x", "[Patterns]")
{
    GIVEN_1(
        auto const p = CheckerPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(.99f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(1.01f, 0.f, 0.f)) == Color::Black() )
}

SCENARIO("Checkers should repeat in y", "[Patterns]")
{
    GIVEN_1(
        auto const p = CheckerPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(0.f, .99f, 0.f)) == Color::White(),
        p.ColorAt(Point(0.f, 1.01f, 0.f)) == Color::Black() )
}

SCENARIO("Checkers should repeat in z", "[Patterns]")
{
    GIVEN_1(
        auto const p = CheckerPattern(Color::White(), Color::Black());
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White(),
        p.ColorAt(Point(0.f, 0.f, 0.99f)) == Color::White(),
        p.ColorAt(Point(0.f, 0.f, 1.01f)) == Color::Black() )
}

SCENARIO("Solid pattern always return same color", "[Patterns]")
{
    GIVEN_1(
        auto const p = SolidPattern(Color(1.f, 0.f, 0.f));
    REQUIRE_,
        p.ColorAt(Point(0.f, 0.f, 0.f)) == Color(1.f, 0.f, 0.f),
        p.ColorAt(Point(0.f, 0.f, 0.99f)) == Color(1.f, 0.f, 0.f),
        p.ColorAt(Point(0.f, 0.f, 1.01f)) == Color(1.f, 0.f, 0.f) )
}
