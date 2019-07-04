#include "Tuple.h"
#include "Util.h"

Tuple Point(float x, float y, float z)
{
    return {x, y, z, 1.0f};
}

Tuple Vector(float x, float y, float z)
{
    return { x, y, z, 0.0f };
}

Tuple operator+(Tuple const& a, Tuple const& b)
{
    return {
        a[0] + b[0],
        a[1] + b[1],
        a[2] + b[2],
        a[3] + b[3]
    };
}

Tuple operator-(Tuple const& a, Tuple const& b)
{
    return {
        a[0] - b[0],
        a[1] - b[1],
        a[2] - b[2],
        a[3] - b[3]
    };
}

Tuple operator-(Tuple const& t)
{
    return {
        -t[0],
        -t[1],
        -t[2],
        -t[3]
    };
}

Tuple operator*(Tuple const& t, float s)
{
    return {
        t[0] * s,
        t[1] * s,
        t[2] * s,
        t[3] * s
    };
}

Tuple operator/(Tuple const& t, float s)
{
    return t * (1.f / s);
}

Tuple::Tuple(float x, float y, float z, float w)
    : m_x(x)
    , m_y(y)
    , m_z(z)
    , m_w(w)
{
}

float Tuple::operator[](size_t index) const
{
    return ((float*)(this))[index];
}

float& Tuple::operator[](size_t index)
{
    return ((float*)(this))[index];
}

bool Tuple::operator!=(Tuple const& other) const
{
    return !Equals(m_x, other.m_x)
        || !Equals(m_y, other.m_y)
        || !Equals(m_z, other.m_z)
        || !Equals(m_w, other.m_w);
}

bool Tuple::operator==(Tuple const& other) const
{
    return !(this->operator!=(other));
}

bool Tuple::IsPoint() const
{
    return Equals(m_w, 1.0f);
}

bool Tuple::IsVector() const
{
    return Equals(m_w, 0.0f);
}

float Tuple::Length() const
{
    return std::sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
}

void Tuple::Normalize()
{
    float const length = Length();
    for (int i = 0; i < 4; i++)
    {
        (this->operator[])(i) /= length;
    }
}

float Tuple::Dot(Tuple const& other) const
{
    float result = 0.f;
    for (int i = 0; i < 4; i++)
    {
        result += (*this)[i] * other[i];
    }

    return result;
}

Tuple Tuple::Cross(Tuple const& other) const
{
    return Vector(
        m_y * other[2] - m_z * other[1],
        m_z * other[0] - m_x * other[2],
        m_x * other[1] - m_y * other[0]
    );
}
