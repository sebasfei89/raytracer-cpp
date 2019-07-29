#pragma once

#include "Matrix.h"
#include "raytracer_export.h"

constexpr float PI = 3.1415927f;

namespace matrix
{
    RAYTRACER_EXPORT Matrix<4, 4> Translation(float x, float y, float z);
    RAYTRACER_EXPORT Matrix<4, 4> Scaling(float x, float y, float z);
    RAYTRACER_EXPORT Matrix<4, 4> RotationX(float radians);
    RAYTRACER_EXPORT Matrix<4, 4> RotationY(float radians);
    RAYTRACER_EXPORT Matrix<4, 4> RotationZ(float radians);
    RAYTRACER_EXPORT Matrix<4, 4> Shearing(float xy, float xz, float yx, float yz, float zx, float zy);
    RAYTRACER_EXPORT Matrix<4, 4> View(Tuple const& from, Tuple const& to, Tuple const& up);
}
