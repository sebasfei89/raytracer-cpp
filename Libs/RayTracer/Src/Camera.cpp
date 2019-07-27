#include "Camera.h"

#include <thread>

#ifndef RENDER_TASKS
#   define RENDER_TASKS 1
#endif

namespace
{
    constexpr uint32_t kRenderTasks = RENDER_TASKS;
    void RenderRow(Camera const& camera, World const& world, Canvas &canvas, uint32_t rowBegin, uint32_t rowEnd, uint32_t cols)
    {
        for (uint32_t y = rowBegin; y < rowEnd; y++)
        {
            for (uint32_t x = 0; x < cols; x++)
            {
                Ray const r = camera.RayForPixel(x, y);
                canvas.WritePixel(x, y, world.ColorAt(r));
            }
        }
    }
}

Camera::Camera(uint32_t hSize, uint32_t vSize, float fov)
    : m_hSize(hSize)
    , m_vSize(vSize)
    , m_fov(fov)
    , m_transform(Mat44::Identity())
    , m_invTransform(Mat44::Identity())
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
    auto const pixel = m_invTransform * Point(worldX, worldY, -1.f);
    auto const origin = m_invTransform * Point(0.f, 0.f, 0.f);
    auto const direction = (pixel - origin).Normalized();
    return Ray(origin, direction);
}

Canvas Camera::Render(World const& world) const
{
    Canvas c(m_hSize, m_vSize);

    std::vector<std::thread> threadPool;
    uint32_t const rowsPerThread = m_vSize / kRenderTasks;

    // Divide work between kRenderTasks - 1 threads
    for (uint32_t y = 0; y < (kRenderTasks - 1); y++)
    {
        uint32_t const begin = y * rowsPerThread;
        uint32_t const end = begin + rowsPerThread;
        threadPool.push_back(std::thread(RenderRow, std::ref(*this), std::ref(world), std::ref(c), begin, end, m_hSize));
    }

    // Last task is run on main thread
    uint32_t const begin = (kRenderTasks - 1) * rowsPerThread;
    RenderRow(*this, world, c, begin, m_vSize, m_hSize);

    for (auto& t : threadPool)
    {
        t.join();
    }

    return c;
}
