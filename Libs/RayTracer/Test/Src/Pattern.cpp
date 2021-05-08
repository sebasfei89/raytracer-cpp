#include "TestHelpers.h"

#include <RayTracer/Color.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Shapes/Group.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>

#include <Beddev/Beddev.h>

SCENARIO("The default pattern transformation", "patterns")
{
    GIVEN( auto const p = TestPattern() )
    THEN( p.Transform() == Mat44::Identity() )
}

SCENARIO("Assigning a transformation", "patterns")
{
    GIVEN( auto p = TestPattern() )
    WHEN( p.SetTransform(matrix::Translation(1.f, 2.f, 3.f)) )
    THEN( p.Transform() == matrix::Translation(1.f, 2.f, 3.f) )
}

SCENARIO("A pattern with an object transformation", "patterns")
{
    GIVEN( auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Scaling(2.f, 2.f, 2.f))
         , auto const p = TestPattern() )
    WHEN( auto const c = p.ShapeColorAt(s, Point(2.f, 3.f, 4.f)) )
    THEN( c == Color(1.f, 1.5f, 2.f) )
}

SCENARIO("A pattern with a pattern transformation", "patterns")
{
    GIVEN( auto s = std::make_shared<Sphere>()
         , auto p = TestPattern()
         , p.SetTransform(matrix::Scaling(2.f, 2.f, 2.f)) )
    WHEN( auto const c = p.ShapeColorAt(s, Point(2.f, 3.f, 4.f)) )
    THEN( c == Color(1.f, 1.5f, 2.f) )
}

SCENARIO("A pattern with both an object and a pattern transformation", "patterns")
{
    GIVEN( auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Scaling(2.f, 2.f, 2.f))
         , auto p = TestPattern()
         , p.SetTransform(matrix::Translation(.5f, 1.f, 1.5f)) )
    WHEN( auto const c = p.ShapeColorAt(s, Point(2.5, 3.f, 3.5f)) )
    THEN( c == Color(.75f, .5f, .25f) )
}

SCENARIO("A strip pattern is constant in y", "patterns")
{
    GIVEN( auto const p = StripPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(0.f, 1.f, 0.f)) == Color::White()
        , p.ColorAt(Point(0.f, 2.f, 0.f)) == Color::White() )
}

SCENARIO("A strip pattern is constant in z", "patterns")
{
    GIVEN( auto const p = StripPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(0.f, 0.f, 1.f)) == Color::White()
        , p.ColorAt(Point(0.f, 0.f, 2.f)) == Color::White() )
}

SCENARIO("A strip pattern alternates in x", "patterns")
{
    GIVEN( auto const p = StripPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(.9f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(1.f, 0.f, 0.f)) == Color::Black()
        , p.ColorAt(Point(-.1f, 0.f, 0.f)) == Color::Black()
        , p.ColorAt(Point(-1.f, 0.f, 0.f)) == Color::Black()
        , p.ColorAt(Point(-1.1f, 0.f, 0.f)) == Color::White() )
}

SCENARIO("A gradient linearly interpolates between colors", "patterns")
{
    GIVEN( auto const p = GradientPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(.25f, 0.f, 0.f)) == Color(.75f, .75f, .75f)
        , p.ColorAt(Point(.5f, 0.f, 0.f)) == Color(.5f, .5f, .5f)
        , p.ColorAt(Point(.75f, 0.f, 0.f)) == Color(.25f, .25f, .25f) )
}

SCENARIO("A ring should extend in both x and z", "patterns")
{
    GIVEN( auto const p = RingPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(1.f, 0.f, 0.f)) == Color::Black()
        , p.ColorAt(Point(0.f, 0.f, 1.f)) == Color::Black()
        , p.ColorAt(Point(.708f, 0.f, .708f)) == Color::Black() )
}

SCENARIO("Checkers should repeat in x", "patterns")
{
    GIVEN( auto const p = CheckerPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(.99f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(1.01f, 0.f, 0.f)) == Color::Black() )
}

SCENARIO("Checkers should repeat in y", "patterns")
{
    GIVEN( auto const p = CheckerPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(0.f, .99f, 0.f)) == Color::White()
        , p.ColorAt(Point(0.f, 1.01f, 0.f)) == Color::Black() )
}

SCENARIO("Checkers should repeat in z", "patterns")
{
    GIVEN( auto const p = CheckerPattern(Color::White(), Color::Black()) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color::White()
        , p.ColorAt(Point(0.f, 0.f, 0.99f)) == Color::White()
        , p.ColorAt(Point(0.f, 0.f, 1.01f)) == Color::Black() )
}

SCENARIO("Solid pattern always return same color", "patterns")
{
    GIVEN( auto const p = SolidPattern(Color(1.f, 0.f, 0.f)) )
    THEN( p.ColorAt(Point(0.f, 0.f, 0.f)) == Color(1.f, 0.f, 0.f)
        , p.ColorAt(Point(0.f, 0.f, 0.99f)) == Color(1.f, 0.f, 0.f)
        , p.ColorAt(Point(0.f, 0.f, 1.01f)) == Color(1.f, 0.f, 0.f) )
}

SCENARIO("A pattern applied to a shape in a group", "patterns")
{
    GIVEN( auto g = std::make_shared<Group>()
         , g->SetTransform(matrix::Scaling(1.f, 1.f, 2.f))
         , auto s = std::make_shared<Sphere>()
         , s->SetTransform(matrix::Scaling(1.f, 2.f, 1.f))
         , g->AddChild(s)
         , auto p = TestPattern()
         , p.SetTransform(matrix::Scaling(2.f, 2.f, 2.f)) )
    WHEN( auto const c = p.ShapeColorAt(s, Point(2.f, 3.f, 4.f)) )
    THEN( c == Color(1.f, .75f, 1.f) )
}
