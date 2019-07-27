#include "Canvas.h"
#include "Util.h"

#include <sstream>

Canvas::Canvas(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
    , m_pixels(nullptr)
{
    m_pixels = (Color*)calloc(width * height, sizeof(Color));
}

Canvas::Canvas(Canvas&& other)
    : m_width(std::exchange(other.m_width, 0))
    , m_height(std::exchange(other.m_height, 0))
    , m_pixels(std::exchange(other.m_pixels, nullptr))
{
}

Canvas::~Canvas()
{
    if (m_pixels != nullptr)
    {
        free(m_pixels);
        m_pixels = nullptr;
    }
}

Color Canvas::PixelAt(uint32_t x, uint32_t y) const
{
    return m_pixels[x + (m_width * y)];
}

std::string Canvas::GetAsPPM() const
{
    std::ostringstream oss;
    WritePPMHeader(oss);
    WritePPMPixels(oss);
    return oss.str();
}

void Canvas::WritePixel(uint32_t x, uint32_t y, Color const& color)
{
    if (x >= m_width || y >= m_height) return;
    m_pixels[x + (m_width * y)] = color;
}

void Canvas::WritePPMHeader(std::ostream& os) const
{
    os << "P3" << std::endl
       << m_width << " " << m_height << std::endl
       << "255" << std::endl;
}

void Canvas::WritePPMPixels(std::ostream& os) const
{
    for (uint32_t i = 0; i < m_height; i++)
    {
        WritePPMPixelsRow(os, i);
        os << std::endl;
    }
}

void Canvas::WritePPMPixelsRow(std::ostream& os, uint32_t row) const
{
    uint32_t rowWidth = 0;
    for (uint32_t j = 0; j < m_width; j++)
    {
        rowWidth = WritePPMPixel(os, row, j, rowWidth);
    }
}

uint32_t Canvas::WritePPMPixel(std::ostream& os, uint32_t row, uint32_t col, uint32_t rowWidth) const
{
    Color const c = PixelAt(col, row);
    rowWidth = WritePPMColorComponent(os, c.R(), rowWidth);
    rowWidth = WritePPMColorComponent(os, c.G(), rowWidth);
    return WritePPMColorComponent(os, c.B(), rowWidth);
}

std::string Canvas::GetAsPPMColor(float color) const
{
    return std::to_string(Clamp((int)std::round(color * 255.f), 0, 255));
}

uint32_t Canvas::WritePPMColorComponent(std::ostream& os, float value, uint32_t rowWidth) const
{
    constexpr uint32_t PPM_LINE_MAX_WIDTH = 70u;
    std::string const valueStr = GetAsPPMColor(value);

    // If there is no enough space in the current line lets start a new one
    if ((rowWidth + valueStr.size() + 1) > PPM_LINE_MAX_WIDTH)
    {
        os << std::endl;
        rowWidth = 0;
    }

    // Append separator if we are not begining a new line
    if (rowWidth > 0)
    {
        os << " ";
        rowWidth++;
    }

    os << valueStr;
    return rowWidth + (uint32_t)valueStr.size();
}
