#include "Camera.h"

Camera::Camera(uint32_t hSize, uint32_t vSize, float fov)
    : m_hSize(hSize)
    , m_vSize(vSize)
    , m_fov(fov)
    , m_transform(Mat44::Identity())
{
    float const halfView = std::tanf(m_fov / 2.f);
    float const aspect = (float)m_hSize / (float)m_vSize;
    m_halfWidth = aspect < 1.f ? halfView * aspect : halfView;
    m_halfHeight = aspect < 1.f ? halfView : halfView / aspect;
    m_pixelSize = (m_halfWidth * 2.f) / (float)m_hSize;
}

Ray Camera::RayForPixel(uint32_t x, uint32_t y) const
{
    float const xOffset = (x + .5f) * m_pixelSize;
    float const yOffset = (y + .5f) * m_pixelSize;
    float const worldX = m_halfWidth - xOffset;
    float const worldY = m_halfHeight - yOffset;
    auto const invTx = m_transform.Inverse();
    auto const pixel = invTx * Point(worldX, worldY, -1.f);
    auto const origin = invTx * Point(0.f, 0.f, 0.f);
    auto const direction = (pixel - origin).Normalized();
    return Ray(origin, direction);
}

Canvas Camera::Render(World const& world) const
{
    Canvas c(m_hSize, m_vSize);
    for (uint32_t y = 0; y < m_vSize; y++)
    {
        for (uint32_t x = 0; x < m_hSize; x++)
        {
            Ray const r = RayForPixel(x, y);
            c.WritePixel(x, y, world.ColorAt(r));
        }
    }

    return c;
}
