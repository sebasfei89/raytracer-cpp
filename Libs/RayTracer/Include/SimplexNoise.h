#pragma once

#include <cstdint>

class SimplexNoise
{
public:
    float Noise(float x, float y, float z) const;

private:
    int32_t fastfloor(float fp) const
    {
        int32_t i = static_cast<int32_t>(fp);
        return (fp < i) ? (i - 1) : (i);
    }

    uint8_t hash(int32_t i) const;

    float grad(int32_t hash, float x, float y, float z) const
    {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }
};
