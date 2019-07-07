#include "Tuple.h"
#include "Util.h"

#include <Testing.h>

SCENARIO("A Tuple with w == 1.0 is a Point", "[math]")
{
    GIVEN("A Tuple initialized with w == 1.0")
    {
        Tuple t{ 4.3f, -4.2f, 3.1f, 1.0f };

        THEN("Fields accessors return valid values")
        {
            REQUIRE(t[0] == 4.3f);
            REQUIRE(t[1] == -4.2f);
            REQUIRE(t[2] == 3.1f);
            REQUIRE(t[3] == 1.0f);
        }
        AND_THEN_REQUIRE(t.IsPoint() == true)
        AND_THEN_REQUIRE(t.IsVector() == false)
    }
}

SCENARIO("A Tuple with w == 0.0 is a Vector", "[math]")
{
    GIVEN("A Tuple initialized with w == 0.0")
    {
        Tuple t{ 4.3f, -4.2f, 3.1f, 0.0f };

        THEN_REQUIRE(t[0] == 4.3f)
        AND_THEN_REQUIRE(t[1] == -4.2f)
        AND_THEN_REQUIRE(t[2] == 3.1f)
        AND_THEN_REQUIRE(t[3] == 0.0f)

        AND_THEN_REQUIRE(t.IsPoint() == false)
        AND_THEN_REQUIRE(t.IsVector() == true)
    }
}

SCENARIO("Point() cretes Tuples with w == 1.0", "[math]")
{
    GIVEN("A Tuple p constructed from Point()")
    {
        Tuple p = Point(4.3f, -4.2f, 3.1f);
        THEN_REQUIRE(p[3] == 1.0f)
    }
}

SCENARIO("Vector() cretes Tuples with w == 0.0", "[math]")
{
    GIVEN("A Tuple v constructed from Vector()")
    {
        Tuple v = Vector(4.3f, -4.2f, 3.1f);
        THEN_REQUIRE(v[3] == 0.0f);
    }
}

SCENARIO("Adding two Tuples", "[math]")
{
    GIVEN("A point and a vector tuples")
    {
        Tuple p{ 3.f, -2.f, 5.f, 1.f };
        Tuple v{ -2.f, 3.f, 1.f, 0.f };

        THEN_REQUIRE( p + v == Tuple(1.f, 1.f, 6.f, 1.f) )
    }
}

SCENARIO("Substracting tuples", "[math]")
{
    GIVEN("Two points")
    {
        Tuple p1 = Point(3.f, 2.f, 1.f);
        Tuple p2 = Point(5.f, 6.f, 7.f);
        THEN_REQUIRE(p1 - p2 == Vector(-2.f, -4.f, -6.f))
    }

    GIVEN("A vector and a point")
    {
        Tuple p = Point(3.f, 2.f, 1.f);
        Tuple v = Vector(5.f, 6.f, 7.f);
        THEN_REQUIRE(p - v == Point(-2.f, -4.f, -6.f))
    }

    GIVEN("Two vectors")
    {
        Tuple v1 = Vector(3.f, 2.f, 1.f);
        Tuple v2 = Vector(5.f, 6.f, 7.f);
        THEN_REQUIRE(v1 - v2 == Vector(-2.f, -4.f, -6.f))
    }

    GIVEN("A vector and the Zero vector")
    {
        Tuple zeroVector = Vector(0.f, 0.f, 0.f);
        Tuple v = Vector(1.f, -2.f, 3.f);
        THEN_REQUIRE(zeroVector - v == Vector(-1.f, 2.f, -3.f))
    }
}

SCENARIO("Negating a tuple", "[math]")
{
    GIVEN("A tuple")
    {
        Tuple t{ 1.f, -2.f, 3.f, -4.f };
        THEN_REQUIRE( -t == Tuple(-1.f, 2.f, -3.f, 4.f) )
    }
}

SCENARIO("Multiplication/Divition of a tuple by a scalar", "[math]")
{
    Tuple const t{ 1.f, -2.f, 3.f, -4.f };

    GIVEN("A tuple and a scalar")
    {
        WHEN("We multiply them")
        {
            THEN_REQUIRE(t * 3.5f == Tuple(3.5f, -7.f, 10.5f, -14.f));
        }

        WHEN("We divide them")
        {
            THEN_REQUIRE(t / 2.f == Tuple(0.5f, -1.f, 1.5f, -2.f))
        }
    }

    GIVEN("A tuple and a fraction")
    {
        float const s = 0.5f;
        THEN_REQUIRE(t * s == Tuple(0.5f, -1.f, 1.5f, -2.f))
    }
}

SCENARIO("Magnitude of a Vector", "[math]")
{
    GIVEN("The Vector v=[1,0,0]")
    {
        Tuple const v = Vector(1.f, 0.f, 0.f);
        THEN_REQUIRE(v.Length() == 1.f)
    }

    GIVEN("The Vector v=[0,1,0]")
    {
        Tuple const v = Vector(0.f, 1.f, 0.f);
        THEN_REQUIRE(v.Length() == 1.f)
    }

    GIVEN("The Vector v=[0,0,1]")
    {
        Tuple const v = Vector(0.f, 0.f, 1.f);
        THEN_REQUIRE(v.Length() == 1.f)
    }

    GIVEN("The Vector v=[1,2,3]")
    {
        Tuple const v = Vector(1.f, 2.f, 3.f);
        THEN_REQUIRE(v.Length() == std::sqrtf(14.f))
        AND_THEN_REQUIRE((-v).Length() == std::sqrtf(14.f))
    }
}

SCENARIO("Vector normalization", "[math]")
{
    GIVEN_WHEN_REQUIRE(
        Tuple v = Vector(4.f, 0.f, 0.f),
        v.Normalize(),
        v == Vector(1.f, 0.f, 0.f))

    GIVEN("A non-unit vector v=[1,2,3]")
    {
        Tuple v = Vector(1.f, 2.f, 3.f);
        WHEN("Calling v.Nomralize()")
        {
            v.Normalize();
            THEN("Vector is unit lenght")
            {
                REQUIRE(v == Vector(0.26726f, 0.53452f, 0.80178f));
                REQUIRE(Equals(v.Length(), 1.f));
            }
        }
    }
}

SCENARIO("Vectors dot product", "[math]")
{
    GIVEN("Two vectors v1=[1,2,3] and v2=[2,3,4]")
    {
        Tuple const v1 = Vector(1.f, 2.f, 3.f);
        Tuple const v2 = Vector(2.f, 3.f, 4.f);
        THEN_REQUIRE(v1.Dot(v2) == 20.f)
    }
}

SCENARIO("Vectors cross product", "[math]")
{
    GIVEN("Two vectors v1=[1,2,3] and v2=[2,3,4]")
    {
        Tuple const v1 = Vector(1.f, 2.f, 3.f);
        Tuple const v2 = Vector(2.f, 3.f, 4.f);

        THEN_REQUIRE(v1.Cross(v2) == Vector(-1.f, 2.f, -1.f))
        AND_THEN_REQUIRE(v2.Cross(v1) == Vector(1.f, -2.f, 1.f))
    }
}
