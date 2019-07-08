#include "Tuple.h"
#include "Util.h"

#include <Testing.h>

SCENARIO("A Tuple with w == 1.0 is a Point", "[math]")
{
    GIVEN_1(
        Tuple t(4.3f, -4.2f, 3.1f, 1.0f);
    REQUIRE_
        , t[0] == 4.3f
        , t[1] == -4.2f
        , t[2] == 3.1f
        , t[3] == 1.0f
        , t.IsPoint() == true
        , t.IsVector() == false )
}

SCENARIO("A Tuple with w == 0.0 is a Vector", "[math]")
{
    GIVEN_1(
        Tuple t(4.3f, -4.2f, 3.1f, 0.0f);
    REQUIRE_
        , t[0] == 4.3f
        , t[1] == -4.2f
        , t[2] == 3.1f
        , t[3] == 0.0f
        , t.IsPoint() == false
        , t.IsVector() == true)
}

SCENARIO("Point() cretes Tuples with w == 1.0", "[math]")
{
    GIVEN_1(
        Tuple p = Point(4.3f, -4.2f, 3.1f);
    REQUIRE_,
        p[3] == 1.0f )
}

SCENARIO("Vector() cretes Tuples with w == 0.0", "[math]")
{
    GIVEN_1(
        Tuple v = Vector(4.3f, -4.2f, 3.1f);
    REQUIRE_,
        v[3] == 0.0f )
}

SCENARIO("Adding two Tuples", "[math]")
{
    GIVEN_1(
        Tuple p(3.f, -2.f, 5.f, 1.f);
        Tuple v(-2.f, 3.f, 1.f, 0.f);
    REQUIRE_,
        p + v == Tuple(1.f, 1.f, 6.f, 1.f) )
}

SCENARIO("Substracting tuples", "[math]")
{
    GIVEN_1(
        Tuple p1 = Point(3.f, 2.f, 1.f);
        Tuple p2 = Point(5.f, 6.f, 7.f);
    REQUIRE_,
        p1 - p2 == Vector(-2.f, -4.f, -6.f) )

    GIVEN_1(
        Tuple p = Point(3.f, 2.f, 1.f);
        Tuple v = Vector(5.f, 6.f, 7.f);
    REQUIRE_,
        p - v == Point(-2.f, -4.f, -6.f) )

    GIVEN_1(
        Tuple v1 = Vector(3.f, 2.f, 1.f);
        Tuple v2 = Vector(5.f, 6.f, 7.f);
    REQUIRE_,
        v1 - v2 == Vector(-2.f, -4.f, -6.f) )

    GIVEN_1(
        Tuple zeroVector = Vector(0.f, 0.f, 0.f);
        Tuple v = Vector(1.f, -2.f, 3.f);
    REQUIRE_,
        zeroVector - v == Vector(-1.f, 2.f, -3.f) )
}

SCENARIO("Negating a tuple", "[math]")
{
    GIVEN_1(
        Tuple t(1.f, -2.f, 3.f, -4.f);
    REQUIRE_,
        -t == Tuple(-1.f, 2.f, -3.f, 4.f) )
}

SCENARIO("Multiplication/Divition of a tuple by a scalar", "[math]")
{
    GIVEN_1(
        Tuple const t(1.f, -2.f, 3.f, -4.f);
        float const s = 0.5f;
    REQUIRE_
        , t * 3.5f == Tuple(3.5f, -7.f, 10.5f, -14.f)
        , t / 2.f == Tuple(0.5f, -1.f, 1.5f, -2.f)
        , t * s == Tuple(0.5f, -1.f, 1.5f, -2.f) )
}

SCENARIO("Magnitude of a Vector", "[math]")
{
    GIVEN_1(
        Tuple const v = Vector(1.f, 0.f, 0.f);
    REQUIRE_,
        v.Length() == 1.f )

    GIVEN_1(
        Tuple const v = Vector(0.f, 1.f, 0.f);
    REQUIRE_,
        v.Length() == 1.f )

    GIVEN_1(
        Tuple const v = Vector(0.f, 0.f, 1.f);
    REQUIRE_,
        v.Length() == 1.f )

    GIVEN_1(
        Tuple const v = Vector(1.f, 2.f, 3.f);
    REQUIRE_
        , v.Length() == std::sqrtf(14.f)
        , (-v).Length() == std::sqrtf(14.f) )
}

SCENARIO("Vector normalization", "[math]")
{
    GIVEN_2(
        Tuple v = Vector(4.f, 0.f, 0.f);
    WHEN_,
        v.Normalize();
    REQUIRE_,
        v == Vector(1.f, 0.f, 0.f) )

    GIVEN_2(
        Tuple v = Vector(1.f, 2.f, 3.f);
    WHEN_,
        v.Normalize();
    REQUIRE_,
        v == Vector(0.26726f, 0.53452f, 0.80178f),
        Equals(v.Length(), 1.f) );
}

SCENARIO("Vectors dot product", "[math]")
{
    GIVEN_1(
        Tuple const v1 = Vector(1.f, 2.f, 3.f);
        Tuple const v2 = Vector(2.f, 3.f, 4.f);
    REQUIRE_,
        v1.Dot(v2) == 20.f )
}

SCENARIO("Vectors cross product", "[math]")
{
    GIVEN_1(
        Tuple const v1 = Vector(1.f, 2.f, 3.f);
        Tuple const v2 = Vector(2.f, 3.f, 4.f);
    REQUIRE_
        , v1.Cross(v2) == Vector(-1.f, 2.f, -1.f)
        , v2.Cross(v1) == Vector(1.f, -2.f, 1.f) )
}
