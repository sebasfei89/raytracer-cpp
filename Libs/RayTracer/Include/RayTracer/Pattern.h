#pragma once

#include "Color.h"
#include "Matrix.h"
#include "raytracer_export.h"
#include "Types.h"

#include <algorithm>

class IPattern
{
public:
    RAYTRACER_EXPORT IPattern();

    RAYTRACER_EXPORT Color ShapeColorAt(ShapeConstPtr const& shape, Tuple const& point) const;
    virtual Color ColorAt(Tuple const& point) const = 0;

    void SetTransform(Mat44 const& t) { m_transform = t; m_invTransform = t.Inverse(); }
    Mat44 const& Transform() const { return m_transform; }
    Mat44 const& InvTransform() const { return m_invTransform; }

private:
    Mat44 m_transform;
    Mat44 m_invTransform;
};

using PatternPtr = std::shared_ptr<IPattern>;

class SolidPattern : public IPattern
{
public:
    SolidPattern(Color const& color) : m_color(color) {}

    RAYTRACER_EXPORT Color ColorAt(Tuple const&) const override { return m_color; }

private:
    Color m_color;
};

class PerlinNoisePattern : public IPattern
{
public:
    PerlinNoisePattern(PatternPtr pattern) : m_pattern(pattern) {}

    RAYTRACER_EXPORT Color ColorAt(Tuple const& point) const override;

private:
    PatternPtr m_pattern;
};

class BinaryPattern : public IPattern
{
public:
    BinaryPattern(PatternPtr a, PatternPtr b) : m_patternA(a), m_patternB(b) {}

protected:
    Color ColorA(Tuple const& point) const;
    Color ColorB(Tuple const& point) const;

private:
    PatternPtr m_patternA;
    PatternPtr m_patternB;
};

#define DECLARE_BINARY_PATTERN(className) \
    class className : public BinaryPattern \
    { \
    public: \
        className(Color const& a, Color const& b) : className(std::make_shared<SolidPattern>(a), std::make_shared<SolidPattern>(b)) {} \
        className(PatternPtr a, PatternPtr b) : BinaryPattern(a, b) {} \
        RAYTRACER_EXPORT Color ColorAt(Tuple const& point) const override; \
    };

DECLARE_BINARY_PATTERN(StripPattern)
DECLARE_BINARY_PATTERN(GradientPattern)
DECLARE_BINARY_PATTERN(RingPattern)
DECLARE_BINARY_PATTERN(CheckerPattern)
DECLARE_BINARY_PATTERN(RadialGradientPattern)
DECLARE_BINARY_PATTERN(BlendPattern)

#undef DECLARE_BINARY_PATTERN
