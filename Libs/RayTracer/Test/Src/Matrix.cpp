#include "Matrix.h"
#include "Tuple.h"

#include <Testing.h>

SCENARIO("Constructing and inspecting a 4x4 matrix", "[math]")
{
    GIVEN("An initialized Matrix m")
    {
        Matrix<4,4> m({
            { 1.0f,  2.0f,  3.0f,  4.0f},
            { 5.5f,  6.5f,  7.5f,  8.5f},
            { 9.0f, 10.0f, 11.0f, 12.0f},
            {13.5f, 14.5f, 15.5f, 16.5f}});

        THEN_REQUIRE((m.At<0, 0>()) == 1.0f)
        AND_THEN_REQUIRE((m.At<0, 1>()) == 2.0f)
        AND_THEN_REQUIRE((m.At<0, 2>()) == 3.0f)
        AND_THEN_REQUIRE((m.At<0, 3>()) == 4.0f)

        AND_THEN_REQUIRE((m.At<1, 0>()) == 5.5f)
        AND_THEN_REQUIRE((m.At<1, 1>()) == 6.5f)
        AND_THEN_REQUIRE((m.At<1, 2>()) == 7.5f)
        AND_THEN_REQUIRE((m.At<1, 3>()) == 8.5f)

        AND_THEN_REQUIRE((m.At<2, 0>()) == 9.0f)
        AND_THEN_REQUIRE((m.At<2, 1>()) == 10.0f)
        AND_THEN_REQUIRE((m.At<2, 2>()) == 11.0f)
        AND_THEN_REQUIRE((m.At<2, 3>()) == 12.0f)

        AND_THEN_REQUIRE((m.At<3, 0>()) == 13.5f)
        AND_THEN_REQUIRE((m.At<3, 1>()) == 14.5f)
        AND_THEN_REQUIRE((m.At<3, 2>()) == 15.5f)
        AND_THEN_REQUIRE((m.At<3, 3>()) == 16.5f)
    }
}

SCENARIO("Constructing and inspecting a 2x2 matrix", "[math]")
{
    GIVEN("An initialized Matrix m")
    {
        Matrix<2, 2> m({
            {1.0f,  2.0f},
            {3.0f,  4.0f}});

        THEN_REQUIRE((m.At<0, 0>()) == 1.0f)
        AND_THEN_REQUIRE((m.At<0, 1>()) == 2.0f)

        AND_THEN_REQUIRE((m.At<1, 0>()) == 3.0f)
        AND_THEN_REQUIRE((m.At<1, 1>()) == 4.0f)
    }
}

SCENARIO("Constructing and inspecting a 3x3 matrix", "[math]")
{
    GIVEN("An initialized Matrix m")
    {
        Matrix<3, 3> m({
            {1.0f, 2.0f, 3.0f},
            {4.0f, 5.0f, 6.0f},
            {7.0f, 8.0f, 9.0f}});

        THEN_REQUIRE((m.At<0, 0>()) == 1.0f)
        AND_THEN_REQUIRE((m.At<0, 1>()) == 2.0f)
        AND_THEN_REQUIRE((m.At<0, 2>()) == 3.0f)

        AND_THEN_REQUIRE((m.At<1, 0>()) == 4.0f)
        AND_THEN_REQUIRE((m.At<1, 1>()) == 5.0f)
        AND_THEN_REQUIRE((m.At<1, 2>()) == 6.0f)

        AND_THEN_REQUIRE((m.At<2, 0>()) == 7.0f)
        AND_THEN_REQUIRE((m.At<2, 1>()) == 8.0f)
        AND_THEN_REQUIRE((m.At<2, 2>()) == 9.0f)
    }
}

SCENARIO("Matrix equality", "[math]")
{
    GIVEN("Two equaly initialized matrices A and B")
    {
        Matrix<4,4> a({
            { 1.0f,  2.0f,  3.0f,  4.0f},
            { 5.0f,  6.0f,  7.0f,  8.0f},
            { 9.0f, 10.0f, 11.0f, 12.0f}, 
            {13.0f, 14.0f, 15.0f, 16.0f}});

        Matrix<4, 4> b({
            { 1.0f,  2.0f,  3.0f,  4.0f},
            { 5.0f,  6.0f,  7.0f,  8.0f},
            { 9.0f, 10.0f, 11.0f, 12.0f},
            {13.0f, 14.0f, 15.0f, 16.0f} });

        THEN_REQUIRE(a == b)
    }

    GIVEN("Two differently initialized matrices A and B")
    {
        Matrix<4, 4> a({
            {1.0f, 2.0f, 3.0f, 4.0f},
            {5.0f, 6.0f, 7.0f, 8.0f},
            {9.0f, 8.0f, 7.0f, 6.0f},
            {5.0f, 4.0f, 3.0f, 2.0f} });

        Matrix<4, 4> b({
            {2.0f, 3.0f, 4.0f, 5.0f},
            {6.0f, 7.0f, 8.0f, 9.0f},
            {8.0f, 7.0f, 6.0f, 5.0f},
            {4.0f, 3.0f, 2.0f, 1.0f} });

        THEN_REQUIRE(a != b)
    }
}

SCENARIO("Matrix multiplication", "[math]")
{
    GIVEN("Two Matrices A and B")
    {
        Matrix<4, 4> a({
            {1.0f, 2.0f, 3.0f, 4.0f},
            {5.0f, 6.0f, 7.0f, 8.0f},
            {9.0f, 8.0f, 7.0f, 6.0f},
            {5.0f, 4.0f, 3.0f, 2.0f} });

        Matrix<4, 4> b({
            {-2.0f, 1.0f, 2.0f,  3.0f},
            { 3.0f, 2.0f, 1.0f, -1.0f},
            { 4.0f, 3.0f, 6.0f,  5.0f},
            { 1.0f, 2.0f, 7.0f,  8.0f} });

        Matrix<4, 4> c({
            { 20.0f, 22.0f,  50.0f,  48.0f},
            { 44.0f, 54.0f, 114.0f, 108.0f},
            { 40.0f, 58.0f, 110.0f, 102.0f},
            { 16.0f, 26.0f,  46.0f,  42.0f} });

        THEN_REQUIRE(a * b == c)
    }
}

SCENARIO("Matrix multiplication with tuple", "[math]")
{
    GIVEN("A matrix m and a tuple t")
    {
        Matrix<4,4> m({
            {1.0f, 2.0f, 3.0f, 4.0f},
            {2.0f, 4.0f, 4.0f, 2.0f},
            {8.0f, 6.0f, 4.0f, 1.0f},
            {0.0f, 0.0f, 0.0f, 1.0f} });
        Tuple const t{1.0f, 2.0f, 3.0f, 1.0f};
        Tuple const t2{ 18.f, 24.f, 33.f, 1.f };
        THEN_REQUIRE(m * t == t2)
    }
}

SCENARIO("Multiplying a matrix by the identity matrix", "[math]")
{
    GIVEN("A matrix m and the identity matrix i")
    {
        Matrix<4, 4> m{
            {0.f, 1.f, 2.f, 4.f},
            {1.f, 2.f, 4.f, 8.f},
            {2.f, 4.f, 8.f, 16.f},
            {4.f, 8.f, 16.f, 32.f}};

        Matrix<4, 4> i{
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f}};

        THEN_REQUIRE(m * i == m)
    }
}

SCENARIO("Multiplying a tuple by the identity matrix", "[math]")
{
    GIVEN("A tuple t and the identity matrix i")
    {
        Tuple const t{1.f, 2.f, 3.f, 4.f};
        Matrix<4, 4> const i{
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f}};

        THEN_REQUIRE(i * t == t)
    }
}

SCENARIO("Transposing a matrix", "[math]")
{
    GIVEN("A matrix m and its transposed matrix mt")
    {
        Matrix<4, 4> m{
            {0.f, 9.f, 3.f, 0.f},
            {9.f, 8.f, 0.f, 8.f},
            {1.f, 8.f, 5.f, 3.f},
            {0.f, 0.f, 5.f, 8.f} };

        Matrix<4, 4> const mt{
            {0.f, 9.f, 1.f, 0.f},
            {9.f, 8.f, 8.f, 0.f},
            {3.f, 0.f, 5.f, 5.f},
            {0.f, 8.f, 3.f, 8.f} };

        WHEN_REQUIRE(m.Transpose(), m == mt)
    }

    GIVEN("The identity matrix i")
    {
        Matrix<4, 4> const identity{
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f} };

        Matrix<4, 4> i = identity;

        WHEN_REQUIRE(i.Transpose(), i == identity)
    }
}

SCENARIO("Calculating the determinant of a 2x2 matrix", "[math]")
{
    GIVEN("A 2x2 matrix m")
    {
        Matrix<2, 2> m({
            { 1.f, 5.f},
            {-3.f, 2.f} });

        THEN_REQUIRE(m.Determinant() == 17.f)
    }
}

SCENARIO("A submatrix of a 3x3 matrix is a 2x2 matrix", "[math]")
{
    GIVEN("A 3x3 matrix m")
    {
        Matrix<3, 3> const m({
            { 1.f, 5.f,  0.f},
            {-3.f, 2.f,  7.f},
            { 0.f, 6.f, -3.f} });

        Matrix<2, 2> const subM({
            {-3.f, 2.f},
            { 0.f, 6.f} });

        THEN_REQUIRE(m.Submatrix(0,2) == subM)
    }
}

SCENARIO("A submatrix of a 4x4 matrix is a 3x3 matrix", "[math]")
{
    GIVEN("A 4x4 matrix m")
    {
        Matrix<4, 4> const m({
            {-6.f, 1.f, 1.f, 6.f},
            {-8.f, 5.f, 8.f, 6.f},
            {-1.f, 0.f, 8.f, 2.f},
            {-7.f, 1.f,-1.f, 1.f} });

        Matrix<3, 3> const subM({
            {-6.f, 1.f, 6.f},
            {-8.f, 8.f, 6.f},
            {-7.f,-1.f, 1.f} });

        THEN_REQUIRE(m.Submatrix(2, 1) == subM)
    }
}

SCENARIO("Calculating a minor of a 3x3 matrix", "[math]")
{
    GIVEN("A 3x3 matrix m")
    {
        Matrix<3, 3> const m({
            {3.f, 5.f, 0.f},
            {2.f,-1.f,-7.f},
            {6.f,-1.f, 5.f} });

        THEN_REQUIRE(m.Submatrix(1, 0).Determinant() == 25.f)
        AND_THEN_REQUIRE(m.Minor(1, 0) == 25.f)
    }
}

SCENARIO("Calculating a cofactor of a 3x3 matrix", "[math]")
{
    GIVEN("A 3x3 matrix m")
    {
        Matrix<3, 3> const m({
            {3.f, 5.f, 0.f},
            {2.f,-1.f,-7.f},
            {6.f,-1.f, 5.f} });

        THEN_REQUIRE(m.Minor(0, 0) == -12.f)
        AND_THEN_REQUIRE(m.Cofactor(0, 0) == -12.f)       
        AND_THEN_REQUIRE(m.Minor(1, 0) == 25.f)
        AND_THEN_REQUIRE(m.Cofactor(1, 0) == -25.f)
    }
}

SCENARIO("Calculating the determinant of a 3x3 matrix", "[math]")
{
    GIVEN("A 3x3 matrix m")
    {
        Matrix<3, 3> const m({
            { 1.f, 2.f, 6.f},
            {-5.f, 8.f,-4.f},
            { 2.f, 6.f, 4.f} });

        THEN_REQUIRE(m.Cofactor(0, 0) == 56.f)
        AND_THEN_REQUIRE(m.Cofactor(0, 1) == 12.f)
        AND_THEN_REQUIRE(m.Cofactor(0, 2) == -46.f)
        AND_THEN_REQUIRE(m.Determinant() == -196)
    }
}

SCENARIO("Calculating the determinant of a 4x4 matrix", "[math]")
{
    GIVEN("A 4x4 matrix m")
    {
        Matrix<4, 4> const m({
            {-2.f,-8.f, 3.f, 5.f},
            {-3.f, 1.f, 7.f, 3.f},
            { 1.f, 2.f,-9.f, 6.f},
            {-6.f, 7.f, 7.f,-9.f} });

        THEN_REQUIRE(m.Cofactor(0, 0) == 690.f)
        AND_THEN_REQUIRE(m.Cofactor(0, 1) == 447.f)
        AND_THEN_REQUIRE(m.Cofactor(0, 2) == 210.f)
        AND_THEN_REQUIRE(m.Cofactor(0, 3) == 51.f)
        AND_THEN_REQUIRE(m.Determinant() == -4071.f)
    }
}

SCENARIO("Testing a matrix for invertibility", "[math]")
{
    GIVEN("An invertible 4x4 matrix m")
    {
        Matrix<4, 4> const m({
            {6.f, 4.f, 4.f, 4.f},
            {5.f, 5.f, 7.f, 6.f},
            {4.f,-9.f, 3.f,-7.f},
            {9.f, 1.f, 7.f,-6.f} });

        THEN_REQUIRE(m.Determinant() == -2120)
        AND_THEN_REQUIRE(m.IsInvertible())
    }

    GIVEN("A non-invertible 4x4 matrix m")
    {
        Matrix<4, 4> const m({
            {-4.f, 2.f,-2.f,-3.f},
            { 9.f, 6.f, 2.f, 6.f},
            { 0.f,-5.f, 1.f,-5.f},
            { 0.f, 0.f, 0.f, 0.f} });

        THEN_REQUIRE(m.Determinant() == 0)
        AND_THEN_REQUIRE(!m.IsInvertible())
    }
}

SCENARIO("Calculating the inverse of a matrix", "[math]")
{
    GIVEN("An invertible 4x4 matrix")
    {
        Matrix<4, 4> const m({
            {-5.f, 2.f, 6.f,-8.f},
            { 1.f,-5.f, 1.f, 8.f},
            { 7.f, 7.f,-6.f,-7.f},
            { 1.f,-3.f, 7.f, 4.f} });
        Matrix<4, 4> const mInverted({
            { 0.21805f, 0.45113f, 0.24060f,-0.04511f},
            {-0.80827f,-1.45677f,-0.44361f, 0.52068f},
            {-0.07895f,-0.22368f,-0.05263f, 0.19737f},
            {-0.52256f,-0.81391f,-0.30075f, 0.30639f} });

        THEN_REQUIRE(m.IsInvertible())
        AND_THEN_REQUIRE(m.Determinant() == 532.f)
        AND_THEN_REQUIRE(m.Cofactor(2, 3) == -160.f)
        AND_THEN_REQUIRE(Equals(mInverted.At<3,2>(), -160.f/532.f))
        AND_THEN_REQUIRE(m.Cofactor(3, 2) == 105.f)
        AND_THEN_REQUIRE(Equals(mInverted.At<2, 3>(), 105.f / 532.f))
        AND_THEN_REQUIRE(m.Inverse() == mInverted)
    }
}

SCENARIO("Multiplying a matrix product by its inverse", "[math]")
{
    GIVEN("Two 4x4 matrices a and b, and its product matrix p")
    {
        Matrix<4, 4> const a({
            { 3.f,-9.f, 7.f, 3.f},
            { 3.f,-8.f, 2.f,-9.f},
            {-4.f, 4.f, 4.f, 1.f},
            { 6.f, 5.f,-1.f, 1.f} });
        Matrix<4, 4> const b({
            { 8.f, 2.f, 2.f, 2.f},
            { 3.f,-1.f, 7.f, 0.f},
            { 7.f, 0.f, 5.f, 4.f},
            { 6.f,-2.f, 0.f, 5.f} });

        WHEN_REQUIRE(
            auto const p = a * b,
            p * b.Inverse() == a)
    }
}
