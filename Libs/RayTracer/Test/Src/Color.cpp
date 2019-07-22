#include "Color.h"

#include <Testing.h>

SCENARIO("Colors are [Red, Green, Blue] Tuples", "[Color]")
{
    GIVEN("A color c=[-0.5, 0.4, 1.7]")
    {
        Color const c(-0.5f, 0.4f, 1.7f);

        THEN("Fields accessors return valid values")
        {
            REQUIRE(c.Red() == -0.5f);
            REQUIRE(c.Green() == 0.4f);
            REQUIRE(c.Blue() == 1.7f);
        }
    }
}

SCENARIO("Color operations", "[Color]")
{
    GIVEN("Two colors a=[0.9, 0.6, 0.75] and b=[0.7, 0.1, 0.25]")
    {
        Color const a(0.9f, 0.6f, 0.75f);
        Color const b(0.7f, 0.1f, 0.25f);

        WHEN("a is added to b")
        {
            THEN("a + b == [1.6, 0.7, 1.0]")
            {
                REQUIRE(a + b == Color(1.6f, 0.7f, 1.0f));
            }
        }

        WHEN("b is subtracted from a")
        {
            THEN("a - b == [0.2, 0.5, 0.5]")
            {
                REQUIRE(a - b == Color(0.2f, 0.5f, 0.5f));
            }
        }

        WHEN("a is multiplyed by a")
        {
            THEN("a * b == [0.63, 0.06, 0.1875]")
            {
                REQUIRE(a * b == Color(0.63f, 0.06f, 0.1875f));
            }
        }

        AND_GIVEN("A scalar s=2")
        {
            float const s = 2.f;
            WHEN("a is multiplayed by s")
            {
                THEN("a * s == [1.8, 1.2, 1.5]")
                {
                    REQUIRE(a * s == Color(1.8f, 1.2f, 1.5f));
                }
            }
        }
    }
}

SCENARIO("Common color constants")
{
    REQUIRE(Color::White() == Color(1.f, 1.f, 1.f));
    REQUIRE(Color::Black() == Color(0.f, 0.f, 0.f));
}
