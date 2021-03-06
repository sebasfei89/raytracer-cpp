#include "Tuple.h"
#include "Util.h"

void to_json(json& j, Tuple const& t)
{
    j = json::array({ t[0], t[1], t[2], t[3] });
}

void from_json(json const& j, Tuple& t)
{
    auto coords = j.get<std::vector<float>>();
    for (int i = 0; i < coords.size(); i++)
    {
        t[i] = coords[i];
    }
}

Tuple Point(float x, float y, float z)
{
    return {x, y, z, 1.0f};
}

Tuple Vector(float x, float y, float z)
{
    return { x, y, z, 0.0f };
}

std::ostream& operator<<(std::ostream& os, Tuple const& t)
{
    os << "[" << t[0];
    for (int i = 1; i < 4; i++)
    {
        os << ", " << t[i];
    }
    os << "]";

    return os;
}

Tuple::Tuple()
    : m_x(0.f)
    , m_y(0.f)
    , m_z(0.f)
    , m_w(0.f)
{
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
    float const oneOverLength = 1.f / Length();
    m_x *= oneOverLength;
    m_y *= oneOverLength;
    m_z *= oneOverLength;
    m_w *= oneOverLength;
}

Tuple Tuple::Normalized() const
{
    float const oneOverLength = 1.f / Length();
    return {
        m_x * oneOverLength,
        m_y * oneOverLength,
        m_z * oneOverLength,
        m_w * oneOverLength
    };
}

float Tuple::Dot(Tuple const& other) const
{
    return (X() * other.X()) + (Y() * other.Y()) + (Z() * other.Z()) + (W() * other.W());
}

Tuple Tuple::Cross(Tuple const& other) const
{
    return Vector(
        m_y * other[2] - m_z * other[1],
        m_z * other[0] - m_x * other[2],
        m_x * other[1] - m_y * other[0]
    );
}

Tuple Tuple::Reflect(Tuple const& other) const
{
    return (*this) - (other * 2.f * this->Dot(other));
}
