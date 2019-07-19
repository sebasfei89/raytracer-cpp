#pragma once

#include "raytracer_export.h"

#include "Canvas.h"
#include "Matrix.h"
#include "Ray.h"
#include "Transformations.h"

class Camera
{
public:
    RAYTRACER_EXPORT Camera(uint32_t hSize, uint32_t vSize, float fov);

    uint32_t HorizontalSize() const { return m_hSize; }
    uint32_t VerticalSize() const { return m_vSize; }
    float FieldOfView() const { return m_fov; }
    float PixelSize() const { return m_pixelSize; }

    void SetTransform(Mat44 const& transform) { m_transform = transform; m_invTransform = transform.Inverse(); }
    Mat44 const& Transform() const { return m_transform; }

    RAYTRACER_EXPORT Ray RayForPixel(uint32_t x, uint32_t y) const;

    RAYTRACER_EXPORT Canvas Render(World const& world) const;

private:
    Mat44 m_transform;
    Mat44 m_invTransform;
    float m_fov;
    float m_halfWidth;
    float m_halfHeight;
    float m_pixelSize;
    uint32_t m_hSize;
    uint32_t m_vSize;
};
