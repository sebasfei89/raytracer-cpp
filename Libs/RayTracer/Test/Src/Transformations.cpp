#include "Transformations.h"

#include <Testing.h>

SCENARIO("Multiplying by a translation matrix", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Translation(5.f, -3.f, 2.f);
        auto const p = Point(-3.f, 4.f, 5.f);
    REQUIRE_,
        tx * p == Point(2.f, 1.f, 7.f) )
}

SCENARIO("Multiplying by a the inverse of a translation matrix", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Translation(5.f, -3.f, 2.f);
        auto const itx = tx.Inverse();
        auto const p = Point(-3.f, 4.f, 5.f);
    REQUIRE_,
        itx * p == Point(-8.f, 7.f, 3.f) )
}

SCENARIO("Translation does not affect vectors", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Translation(5.f, -3.f, 2.f);
        auto const v = Vector(-3.f, 4.f, 5.f);
    REQUIRE_,
        tx * v == v )
}

SCENARIO("A scaling matrix applied to a point", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Scaling(2.f, 3.f, 4.f);
        auto const p = Point(-4.f, 6.f, 8.f);
    REQUIRE_,
        tx * p == Point(-8.f, 18.f, 32.f) )
}

SCENARIO("A scaling matrix applied to a vector", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Scaling(2.f, 3.f, 4.f);
        auto const v = Vector(-4.f, 6.f, 8.f);
    REQUIRE_,
        tx * v == Vector(-8.f, 18.f, 32.f) )
}

SCENARIO("Multiplying by the inverse of a scaling matrix", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Scaling(2.f, 3.f, 4.f);
        auto const itx = tx.Inverse();
        auto const v = Vector(-4.f, 6.f, 8.f);
    REQUIRE_,
        itx * v == Vector(-2.f, 2.f, 2.f) )
}

SCENARIO("Reflection is scaling by a negative value", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Scaling(-1.f, 1.f, 1.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(-2.f, 3.f, 4.f) )
}

SCENARIO("Rotating a point around the x axis", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::RotationX(PI / 4.f);
        auto const p = Point(0.f, 1.f, 0.f);
    REQUIRE_,
        tx * p == Point(0.f, sqrt(2.f) / 2.f, sqrt(2.f) / 2.f) )

    GIVEN_1(
        auto const tx = matrix::RotationX(PI / 2.f);
        auto const p = Point(0.f, 1.f, 0.f);
    REQUIRE_,
        tx * p == Point(0.f, 0.f, 1.f) )
}

SCENARIO("The inverse of an x-rotation rotates in the opposite direction", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::RotationX(PI / 4.f);
        auto const itx = tx.Inverse();
        auto const p = Point(0.f, 1.f, 0.f);
    REQUIRE_,
        itx * p == Point(0.f, sqrt(2.f) / 2.f, -sqrt(2.f) / 2.f) )
}

SCENARIO("Rotating a point around the y axis", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::RotationY(PI / 4.f);
        auto const p = Point(0.f, 0.f, 1.f);
    REQUIRE_,
        tx * p == Point(sqrt(2.f) / 2.f, 0.f, sqrt(2.f) / 2.f) )

    GIVEN_1(
        auto const tx = matrix::RotationY(PI / 2.f);
        auto const p = Point(0.f, 0.f, 1.f);
    REQUIRE_,
        tx * p == Point(1.f, 0.f, 0.f) )
}

SCENARIO("Rotating a point around the z axis", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::RotationZ(PI / 4.f);
        auto const p = Point(0.f, 1.f, 0.f);
    REQUIRE_,
        tx * p == Point(-sqrt(2.f) / 2.f, sqrt(2.f) / 2.f, 0.f) )

    GIVEN_1(
        auto const tx = matrix::RotationZ(PI / 2.f);
        auto const p = Point(0.f, 1.f, 0.f);
    REQUIRE_,
        tx * p == Point(-1.f, 0.f, 0.f) )
}

SCENARIO("A shearing transformation moves one component in proportion to other", "[math]")
{
    GIVEN_1(
        auto const tx = matrix::Shearing(1.f, 0.f, 0.f, 0.f, 0.f, 0.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(5.f, 3.f, 4.f) )

    GIVEN_1(
        auto const tx = matrix::Shearing(0.f, 1.f, 0.f, 0.f, 0.f, 0.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(6.f, 3.f, 4.f) )

    GIVEN_1(
        auto const tx = matrix::Shearing(0.f, 0.f, 1.f, 0.f, 0.f, 0.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(2.f, 5.f, 4.f) )

    GIVEN_1(
        auto const tx = matrix::Shearing(0.f, 0.f, 0.f, 1.f, 0.f, 0.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(2.f, 7.f, 4.f) )

    GIVEN_1(
        auto const tx = matrix::Shearing(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(2.f, 3.f, 6.f) )

    GIVEN_1(
        auto const tx = matrix::Shearing(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
        auto const p = Point(2.f, 3.f, 4.f);
    REQUIRE_,
        tx * p == Point(2.f, 3.f, 7.f) )
}

SCENARIO("Individual transformations are applied in sequence", "[math]")
{
    GIVEN_2(
        auto const p = Point(1.f, 0.f, 1.f);
        auto const mr = matrix::RotationX(PI / 2.f);
        auto const ms = matrix::Scaling(5.f, 5.f, 5.f);
        auto const mt = matrix::Translation(10.f, 5.f, 7.f);
    WHEN_,
        auto const p2 = mr * p;
        auto const p3 = ms * p2;
        auto const p4 = mt * p3;
    REQUIRE_
        , p2 == Point(1.f, -1.f, 0.f)
        , p3 == Point(5.f, -5.f, 0.f)
        , p4 == Point(15.f, 0.f, 7.f))
}

SCENARIO("Chained transformations must be applied in reverse order", "[math]")
{
    GIVEN_2(
        auto const p = Point(1.f, 0.f, 1.f);
        auto const mr = matrix::RotationX(PI / 2.f);
        auto const ms = matrix::Scaling(5.f, 5.f, 5.f);
        auto const mt = matrix::Translation(10.f, 5.f, 7.f);
    WHEN_,
        auto const tx = mt * ms * mr;
    REQUIRE_,
        tx * p == Point(15.f, 0.f, 7.f) )
}
