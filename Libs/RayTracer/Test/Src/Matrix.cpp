#include "Matrix.h"
#include "Tuple.h"

#include <Testing.h>

SCENARIO("Constructing and inspecting a 4x4 matrix", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            { 1.0f,  2.0f,  3.0f,  4.0f},
            { 5.5f,  6.5f,  7.5f,  8.5f},
            { 9.0f, 10.0f, 11.0f, 12.0f},
            {13.5f, 14.5f, 15.5f, 16.5f}});
    REQUIRE_
        , m.At(0, 0) == 1.0f
        , m.At(0, 3) == 4.0f
        , m.At(1, 0) == 5.5f
        , m.At(1, 2) == 7.5f
        , m.At(2, 2) == 11.0f
        , m.At(3, 0) == 13.5f
        , m.At(3, 3) == 16.5f)
}

SCENARIO("Constructing and inspecting a 2x2 matrix", "[math]")
{
    GIVEN_1(
        Mat22 m({
            {1.0f,  2.0f},
            {3.0f,  4.0f}});
    REQUIRE_
        , m.At(0, 0) == 1.0f
        , m.At(0, 1) == 2.0f
        , m.At(1, 0) == 3.0f
        , m.At(1, 1) == 4.0f )
}

SCENARIO("Constructing and inspecting a 3x3 matrix", "[math]")
{
    GIVEN_1(
        Mat33 m({
            {1.0f, 2.0f, 3.0f},
            {4.0f, 5.0f, 6.0f},
            {7.0f, 8.0f, 9.0f}});
    REQUIRE_
        , m.At(0, 0) == 1.0f
        , m.At(0, 1) == 2.0f
        , m.At(0, 2) == 3.0f
        , m.At(1, 0) == 4.0f
        , m.At(1, 1) == 5.0f
        , m.At(1, 2) == 6.0f
        , m.At(2, 0) == 7.0f
        , m.At(2, 1) == 8.0f
        , m.At(2, 2) == 9.0f )
}

SCENARIO("Matrix equality", "[math]")
{
    GIVEN_1(
        Mat44 const a({
            { 1.0f,  2.0f,  3.0f,  4.0f},
            { 5.0f,  6.0f,  7.0f,  8.0f},
            { 9.0f, 10.0f, 11.0f, 12.0f}, 
            {13.0f, 14.0f, 15.0f, 16.0f}});
        Mat44 const b({
            { 1.0f,  2.0f,  3.0f,  4.0f},
            { 5.0f,  6.0f,  7.0f,  8.0f},
            { 9.0f, 10.0f, 11.0f, 12.0f},
            {13.0f, 14.0f, 15.0f, 16.0f} });
    REQUIRE_,
        a == b )

    GIVEN_1(
        Mat44 const a({
            {1.0f, 2.0f, 3.0f, 4.0f},
            {5.0f, 6.0f, 7.0f, 8.0f},
            {9.0f, 8.0f, 7.0f, 6.0f},
            {5.0f, 4.0f, 3.0f, 2.0f} });
        Mat44 const b({
            {2.0f, 3.0f, 4.0f, 5.0f},
            {6.0f, 7.0f, 8.0f, 9.0f},
            {8.0f, 7.0f, 6.0f, 5.0f},
            {4.0f, 3.0f, 2.0f, 1.0f} });
    REQUIRE_,
        a != b )
}

SCENARIO("Matrix multiplication", "[math]")
{
    GIVEN_1(
        Mat44 const a({
            {1.0f, 2.0f, 3.0f, 4.0f},
            {5.0f, 6.0f, 7.0f, 8.0f},
            {9.0f, 8.0f, 7.0f, 6.0f},
            {5.0f, 4.0f, 3.0f, 2.0f} });
        Mat44 const b({
            {-2.0f, 1.0f, 2.0f,  3.0f},
            { 3.0f, 2.0f, 1.0f, -1.0f},
            { 4.0f, 3.0f, 6.0f,  5.0f},
            { 1.0f, 2.0f, 7.0f,  8.0f} });
        Mat44 const c({
            { 20.0f, 22.0f,  50.0f,  48.0f},
            { 44.0f, 54.0f, 114.0f, 108.0f},
            { 40.0f, 58.0f, 110.0f, 102.0f},
            { 16.0f, 26.0f,  46.0f,  42.0f} });
    REQUIRE_,
        a * b == c )
}

SCENARIO("Matrix multiplication with tuple", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            {1.0f, 2.0f, 3.0f, 4.0f},
            {2.0f, 4.0f, 4.0f, 2.0f},
            {8.0f, 6.0f, 4.0f, 1.0f},
            {0.0f, 0.0f, 0.0f, 1.0f} });
        Tuple const t(1.0f, 2.0f, 3.0f, 1.0f);
        Tuple const t2(18.f, 24.f, 33.f, 1.f);
    REQUIRE_,
        m * t == t2 )
}

SCENARIO("Multiplying a matrix by the identity matrix", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            {0.f, 1.f, 2.f, 4.f},
            {1.f, 2.f, 4.f, 8.f},
            {2.f, 4.f, 8.f, 16.f},
            {4.f, 8.f, 16.f, 32.f}});
        Mat44 const i({
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f}});
    REQUIRE_,
        m * i == m )
}

SCENARIO("Multiplying a tuple by the identity matrix", "[math]")
{
    GIVEN_1(
        Tuple const t(1.f, 2.f, 3.f, 4.f);
        Mat44 const i({
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f}});
    REQUIRE_,
        i * t == t )
}

SCENARIO("Transposing a matrix", "[math]")
{
    GIVEN_2(
        Mat44 m({
            {0.f, 9.f, 3.f, 0.f},
            {9.f, 8.f, 0.f, 8.f},
            {1.f, 8.f, 5.f, 3.f},
            {0.f, 0.f, 5.f, 8.f} });
        Mat44 const mt({
            {0.f, 9.f, 1.f, 0.f},
            {9.f, 8.f, 8.f, 0.f},
            {3.f, 0.f, 5.f, 5.f},
            {0.f, 8.f, 3.f, 8.f} });
    WHEN_,
        m.Transpose();
    REQUIRE_,
        m == mt )

    GIVEN_2(
        Mat44 const identity({
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f} });
    WHEN_,
        Mat44 i = identity;
        i.Transpose();
    REQUIRE_,
        i == identity )
}

SCENARIO("Calculating the determinant of a 2x2 matrix", "[math]")
{
    GIVEN_1(
        Mat22 const m({
            { 1.f, 5.f},
            {-3.f, 2.f} });
    REQUIRE_,
        m.Determinant() == 17.f )
}

SCENARIO("A submatrix of a 3x3 matrix is a 2x2 matrix", "[math]")
{
    GIVEN_1(
        Mat33 const m({
            { 1.f, 5.f,  0.f},
            {-3.f, 2.f,  7.f},
            { 0.f, 6.f, -3.f} });
        Mat22 const subM({
            {-3.f, 2.f},
            { 0.f, 6.f} });
    REQUIRE_,
        m.Submatrix(0,2) == subM )
}

SCENARIO("A submatrix of a 4x4 matrix is a 3x3 matrix", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            {-6.f, 1.f, 1.f, 6.f},
            {-8.f, 5.f, 8.f, 6.f},
            {-1.f, 0.f, 8.f, 2.f},
            {-7.f, 1.f,-1.f, 1.f} });
        Mat33 const subM({
            {-6.f, 1.f, 6.f},
            {-8.f, 8.f, 6.f},
            {-7.f,-1.f, 1.f} });
    REQUIRE_,
        m.Submatrix(2, 1) == subM )
}

SCENARIO("Calculating a minor of a 3x3 matrix", "[math]")
{
    GIVEN_1(
        Mat33 const m({
            {3.f, 5.f, 0.f},
            {2.f,-1.f,-7.f},
            {6.f,-1.f, 5.f} });
    REQUIRE_
        , m.Submatrix(1, 0).Determinant() == 25.f
        , m.Minor(1, 0) == 25.f )
}

SCENARIO("Calculating a cofactor of a 3x3 matrix", "[math]")
{
    GIVEN_1(
        Mat33 const m({
            {3.f, 5.f, 0.f},
            {2.f,-1.f,-7.f},
            {6.f,-1.f, 5.f} });
    REQUIRE_
        , m.Minor(0, 0) == -12.f
        , m.Cofactor(0, 0) == -12.f
        , m.Minor(1, 0) == 25.f
        , m.Cofactor(1, 0) == -25.f )
}

SCENARIO("Calculating the determinant of a 3x3 matrix", "[math]")
{
    GIVEN_1(
        Mat33 const m({
            { 1.f, 2.f, 6.f},
            {-5.f, 8.f,-4.f},
            { 2.f, 6.f, 4.f} });
    REQUIRE_
        , m.Cofactor(0, 0) == 56.f
        , m.Cofactor(0, 1) == 12.f
        , m.Cofactor(0, 2) == -46.f
        , m.Determinant() == -196 )
}

SCENARIO("Calculating the determinant of a 4x4 matrix", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            {-2.f,-8.f, 3.f, 5.f},
            {-3.f, 1.f, 7.f, 3.f},
            { 1.f, 2.f,-9.f, 6.f},
            {-6.f, 7.f, 7.f,-9.f} });
    REQUIRE_
        , m.Cofactor(0, 0) == 690.f
        , m.Cofactor(0, 1) == 447.f
        , m.Cofactor(0, 2) == 210.f
        , m.Cofactor(0, 3) == 51.f
        , m.Determinant() == -4071.f )
}

SCENARIO("Testing a matrix for invertibility", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            {6.f, 4.f, 4.f, 4.f},
            {5.f, 5.f, 7.f, 6.f},
            {4.f,-9.f, 3.f,-7.f},
            {9.f, 1.f, 7.f,-6.f} });
    REQUIRE_
        , m.Determinant() == -2120
        , m.IsInvertible() )

    GIVEN_1(
        Mat44 const m({
            {-4.f, 2.f,-2.f,-3.f},
            { 9.f, 6.f, 2.f, 6.f},
            { 0.f,-5.f, 1.f,-5.f},
            { 0.f, 0.f, 0.f, 0.f} });
    REQUIRE_
        , m.Determinant() == 0
        , !m.IsInvertible() )
}

SCENARIO("Calculating the inverse of a matrix", "[math]")
{
    GIVEN_1(
        Mat44 const m({
            {-5.f, 2.f, 6.f,-8.f},
            { 1.f,-5.f, 1.f, 8.f},
            { 7.f, 7.f,-6.f,-7.f},
            { 1.f,-3.f, 7.f, 4.f} });
        Mat44 const mInverted({
            { 0.21805f, 0.45113f, 0.24060f,-0.04511f},
            {-0.80827f,-1.45677f,-0.44361f, 0.52068f},
            {-0.07895f,-0.22368f,-0.05263f, 0.19737f},
            {-0.52256f,-0.81391f,-0.30075f, 0.30639f} });
    REQUIRE_
        , m.IsInvertible()
        , m.Determinant() == 532.f
        , m.Cofactor(2, 3) == -160.f
        , Equals(mInverted.At<3,2>(), -160.f/532.f)
        , m.Cofactor(3, 2) == 105.f
        , Equals(mInverted.At<2, 3>(), 105.f / 532.f)
        , m.Inverse() == mInverted )
}

SCENARIO("Multiplying a matrix product by its inverse", "[math]")
{
    GIVEN_2(
        Mat44 const a({
            { 3.f,-9.f, 7.f, 3.f},
            { 3.f,-8.f, 2.f,-9.f},
            {-4.f, 4.f, 4.f, 1.f},
            { 6.f, 5.f,-1.f, 1.f} });
        Mat44 const b({
            { 8.f, 2.f, 2.f, 2.f},
            { 3.f,-1.f, 7.f, 0.f},
            { 7.f, 0.f, 5.f, 4.f},
            { 6.f,-2.f, 0.f, 5.f} });
    WHEN_,
        auto const p = a * b;
    REQUIRE_,
        p * b.Inverse() == a )
}
