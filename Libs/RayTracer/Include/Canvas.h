#pragma once

#include "Color.h"
#include <string>

// TODO: non-copyable
class Canvas
{
public:
    RAYTRACER_EXPORT Canvas(uint32_t width, uint32_t height);
    RAYTRACER_EXPORT ~Canvas();

    RAYTRACER_EXPORT int Width() const { return m_width; }
    RAYTRACER_EXPORT int Height() const { return m_height; }

    RAYTRACER_EXPORT Color PixelAt(uint32_t x, uint32_t y) const;
    RAYTRACER_EXPORT void WritePixel(uint32_t x, uint32_t y, Color const& color);

    RAYTRACER_EXPORT std::string GetAsPPM() const;

protected:
    // PPM helper methods
    void WritePPMHeader(std::ostream& os) const;
    void WritePPMPixels(std::ostream& os) const;
    void WritePPMPixelsRow(std::ostream& os, uint32_t row) const;
    uint32_t WritePPMPixel(std::ostream& os, uint32_t row, uint32_t col, uint32_t rowWidth) const;
    uint32_t Canvas::WritePPMColorComponent(std::ostream& os, float value, uint32_t rowWidth) const;
    std::string GetAsPPMColor(float color) const;

private:
    uint32_t m_width;
    uint32_t m_height;
    Color* m_pixels;
};
