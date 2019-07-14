#include "Transformations.h"

namespace matrix
{
    Matrix<4, 4> Translation(float x, float y, float z)
    {
        return Matrix<4, 4>({
            {1.f, 0.f, 0.f,  x },
            {0.f, 1.f, 0.f,  y },
            {0.f, 0.f, 1.f,  z },
            {0.f, 0.f, 0.f, 1.f} });
    }

    Matrix<4, 4> Scaling(float x, float y, float z)
    {
        return Matrix<4, 4>({
            { x , 0.f, 0.f, 0.f },
            {0.f,  y , 0.f, 0.f },
            {0.f, 0.f,  z , 0.f },
            {0.f, 0.f, 0.f, 1.f} });
    }

    Matrix<4, 4> RotationX(float radians)
    {
        float const c = std::cos(radians);
        float const s = std::sin(radians);
        return Matrix<4, 4>({
            {1.f, 0.f, 0.f, 0.f},
            {0.f,  c , -s , 0.f},
            {0.f,  s ,  c , 0.f},
            {0.f, 0.f, 0.f, 1.f} });
    }

    Matrix<4, 4> RotationY(float radians)
    {
        float const c = std::cos(radians);
        float const s = std::sin(radians);
        return Matrix<4, 4>({
            { c , 0.f,  s , 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {-s , 0.f,  c , 0.f},
            {0.f, 0.f, 0.f, 1.f} });
    }

    Matrix<4, 4> RotationZ(float radians)
    {
        float const c = std::cos(radians);
        float const s = std::sin(radians);
        return Matrix<4, 4>({
            { c , -s , 0.f, 0.f},
            { s ,  c , 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f} });
    }

    Matrix<4, 4> Shearing(float xy, float xz, float yx, float yz, float zx, float zy)
    {
        return Matrix<4, 4>({
            {1.f,  xy,  xz, 0.f},
            { yx, 1.f,  yz, 0.f},
            { zx,  zy, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f} });
    }

    Matrix<4, 4> View(Tuple const& from, Tuple const& to, Tuple const& up)
    {
        auto const forward = (to - from).Normalized();
        auto const left = forward.Cross(up.Normalized());
        auto const fixedUp = left.Cross(forward);
        Mat44 const orientation{
            {    left[0],    left[1],    left[2], 0.f },
            { fixedUp[0], fixedUp[1], fixedUp[2], 0.f },
            {-forward[0],-forward[1],-forward[2], 0.f },
            {        0.f,        0.f,        0.f, 1.f }
        };
        return orientation * Translation(-from[0], -from[1], -from[2]);
    }
}
