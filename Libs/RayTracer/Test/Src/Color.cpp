#include <RayTracer/Color.h>

#include <Beddev/Beddev.h>

SCENARIO("Colors are [Red, Green, Blue] Tuples", "color")
{
    GIVEN( Color const c(-0.5f, 0.4f, 1.7f) )
    THEN( c.R() == -0.5f
        , c.G() == 0.4f
        , c.B() == 1.7f )
}

SCENARIO("Color operations", "color")
{
    GIVEN( Color const a(0.9f, 0.6f, 0.75f)
         , Color const b(0.7f, 0.1f, 0.25f)
         , float const s = 2.f )
    THEN( a + b == Color(1.6f, 0.7f, 1.0f)
        , a - b == Color(0.2f, 0.5f, 0.5f)
        , a * b == Color(0.63f, 0.06f, 0.1875f)
        , a * s == Color(1.8f, 1.2f, 1.5f) )
}

SCENARIO("Common color constants", "color")
{
    REQUIRE( Color::White() == Color(1.f, 1.f, 1.f)
           , Color::Black() == Color(0.f, 0.f, 0.f)
           , Color::Red() == Color(1.f, 0.f, 0.f)
           , Color::Green() == Color(0.f, 1.f, 0.f)
           , Color::Blue() == Color(0.f, 0.f, 1.f)
           , Color::Yellow() == Color(1.f, 1.f, 0.f) )
}
