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

}
