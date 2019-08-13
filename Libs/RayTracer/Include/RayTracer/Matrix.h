#pragma once

#include "Tuple.h"
#include "Util.h"

#if USE_SSE
#   include "MathIntrinsics.h"
#endif

#include <algorithm>
#include <ostream>

template<uint8_t ROWS, uint8_t COLS>
class Matrix
{
public:
    Matrix() {}
    explicit Matrix(std::initializer_list<std::initializer_list<float>> data);

    static Matrix Identity();

    template<uint8_t row, uint8_t col>
    typename std::enable_if<(row < ROWS) && (col < COLS), float>::type
    At() const
    {
#if USE_SSE
        return m_data[col][row];
#else
        return m_data[row][col];
#endif
    }

    float At(uint8_t row, uint8_t col) const
    {
#if USE_SSE
        return m_data[col][row];
#else
        return m_data[row][col];
#endif
    }

    float& At(uint8_t row, uint8_t col)
    {
#if USE_SSE
        return m_data[col][row];
#else
        return m_data[row][col];
#endif
    }

    bool operator==(Matrix const& other) const;
    bool operator!=(Matrix const& other) const { return !this->operator==(other); }

    Matrix operator*(Matrix const& other) const;

    template<uint8_t R = ROWS, uint8_t C = COLS>
    typename std::enable_if<(R == 4) && (C == 4), Tuple>::type
    operator*(Tuple const& tuple) const
    {
#if USE_SSE
        __m128 x = _mm_set1_ps(tuple.X());
        __m128 y = _mm_set1_ps(tuple.Y());
        __m128 z = _mm_set1_ps(tuple.Z());
        __m128 w = _mm_set1_ps(tuple.W());

        //__m128 p1 = _mm_mul_ps(x, m_sseData[0]);
        //__m128 p2 = _mm_mul_ps(y, m_sseData[1]);
        //__m128 p3 = _mm_mul_ps(z, m_sseData[2]);
        //__m128 p4 = _mm_mul_ps(w, m_sseData[3]);

        //return _mm_add_ps(_mm_add_ps(p1, p2), _mm_add_ps(p3, p4));

        __m128 p1 = _mm_mul_ps(x, m_sseData[0]);
        __m128 p2 = _mm_fmadd_ps(y, m_sseData[1], p1);
        __m128 p3 = _mm_mul_ps(z, m_sseData[2]);
        __m128 p4 = _mm_fmadd_ps(w, m_sseData[3], p3);

        return _mm_add_ps(p2, p4);
#else
        return {
            (At<0,0>() * tuple.X()) + (At<0,1>() * tuple.Y()) + (At<0,2>() * tuple.Z()) + (At<0,3>() * tuple.W()),
            (At<1,0>() * tuple.X()) + (At<1,1>() * tuple.Y()) + (At<1,2>() * tuple.Z()) + (At<1,3>() * tuple.W()),
            (At<2,0>() * tuple.X()) + (At<2,1>() * tuple.Y()) + (At<2,2>() * tuple.Z()) + (At<2,3>() * tuple.W()),
            (At<3,0>() * tuple.X()) + (At<3,1>() * tuple.Y()) + (At<3,2>() * tuple.Z()) + (At<3,3>() * tuple.W())
        };
#endif
    }

    Matrix Transposed() const;
    void Transpose();

    template<uint8_t R=ROWS, uint8_t C=COLS>
    typename std::enable_if<(R > 2) && (C > 2), float>::type
    Minor(uint8_t row, uint8_t col) const { return Submatrix(row, col).Determinant(); }

    template<uint8_t R = ROWS, uint8_t C = COLS>
    typename std::enable_if<(R > 2) && (C > 2), float>::type
    Cofactor(uint8_t row, uint8_t col) const { return Minor(row, col) * (((row + col) % 2) == 0 ? 1 : -1); }

    float Determinant() const;

    bool IsInvertible() const { return Determinant() != 0.f; }
    Matrix Inverse() const;

    Matrix<ROWS - 1, COLS - 1> Submatrix(uint8_t row, uint8_t col) const;

private:
    union
    {
#if USE_SSE
        __m128 m_sseData[4];
#endif
        float m_data[ROWS][COLS];
    };
};

using Mat22 = Matrix<2, 2>;
using Mat33 = Matrix<3, 3>;
using Mat44 = Matrix<4, 4>;

template<uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS>::Matrix(std::initializer_list<std::initializer_list<float>> data)
{
    int i = 0;
    for (auto row : data)
    {
        if (i >= ROWS) break;
        int col = 0;
        for (float v : row)
        {
            if (col >= COLS) break;
#if USE_SSE
            m_data[col++][i] = v;
#else
            m_data[i][col++] = v;
#endif
        }
        i++;
    }
}

template<uint8_t ROWS, uint8_t COLS>
bool Matrix<ROWS, COLS>::operator==(Matrix const& other) const
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            if (!Equals(m_data[i][j], other.m_data[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}

#if USE_SSE
template<>
inline Matrix<4,4> Matrix<4, 4>::operator*(Matrix const& other) const
{
    Matrix<4,4> r;
    for (int i = 0; i < 4; i++)
    {
        __m128 brodX = _mm_set1_ps(other.m_sseData[i].m128_f32[0]);
        __m128 brodY = _mm_set1_ps(other.m_sseData[i].m128_f32[1]);
        __m128 brodZ = _mm_set1_ps(other.m_sseData[i].m128_f32[2]);
        __m128 brodW = _mm_set1_ps(other.m_sseData[i].m128_f32[3]);
        r.m_sseData[i] = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(brodX, m_sseData[0]),
                _mm_mul_ps(brodY, m_sseData[1])),
            _mm_add_ps(
                _mm_mul_ps(brodZ, m_sseData[2]),
                _mm_mul_ps(brodW, m_sseData[3])));
    }
    return r;
}
#endif

template<uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::operator*(Matrix const& other) const
{
    Matrix<ROWS, COLS> c;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            c.At(i, j)
                = At(i, 0) * other.At(0, j)
                + At(i, 1) * other.At(1, j)
                + At(i, 2) * other.At(2, j)
                + At(i, 3) * other.At(3, j);
        }
    }

    return c;
}

template<uint8_t ROWS, uint8_t COLS>
void Matrix<ROWS, COLS>::Transpose()
{
    for (int i = 0; i < (ROWS - 1); i++)
    {
        for (int j = (i + 1); j < (COLS); j++)
        {
            std::swap(m_data[i][j], m_data[j][i]);
        }
    }
}

template<uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::Transposed() const
{
    Matrix<ROWS, COLS> tmp = *this;
    tmp.Transpose();
    return tmp;
}

template<>
inline float Matrix<2, 2>::Determinant() const
{
    return m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
}

template<>
inline Matrix<2, 2> Matrix<2, 2>::Identity()
{
    return Matrix<2, 2>{
        {1.f, 0.f},
        { 0.f, 1.f }
    };
}

template<>
inline Matrix<3, 3> Matrix<3, 3>::Identity()
{
    return Matrix<3, 3>{
        {1.f, 0.f, 0.f},
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f }
    };
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::Identity()
{
    return Matrix<4, 4>{
        { 1.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { 0.f, 0.f, 0.f, 1.f }
    };
}

template<uint8_t ROWS, uint8_t COLS>
float Matrix<ROWS,COLS>::Determinant() const
{
    float det = 0.f;
    for (int j = 0; j < COLS; j++)
    {
        det += At(0, j) * Cofactor(0, j);
    }
    return det;
}

template<uint8_t ROWS, uint8_t COLS>
Matrix<ROWS - 1, COLS - 1> Matrix<ROWS, COLS>::Submatrix(uint8_t row, uint8_t col) const
{
    Matrix<ROWS - 1, COLS - 1> subM;
    for (uint8_t i = 0, subI = 0; i < ROWS; i++)
    {
        if (i == row) continue;
        for (uint8_t j = 0, subJ = 0; j < COLS; j++)
        {
            if (j == col) continue;
            subM.At(subI, subJ) = At(i,j);
            subJ++;
        }
        subI++;
    }
    return subM;
}

template<uint8_t ROWS, uint8_t COLS>
Matrix<ROWS, COLS> Matrix<ROWS, COLS>::Inverse() const
{
    // Inverse matrix is buit by dividing the transposed
    // cofactors matrix by the determinant of the original matrix
    float const invDet = 1.f / Determinant();
    Matrix<ROWS, COLS> inverse;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            inverse.At(i, j) = Cofactor(j, i) * invDet;
        }
    }
    return inverse;
}

template<uint8_t ROWS, uint8_t COLS>
std::ostream& operator<<(std::ostream& os, Matrix<ROWS,COLS> const& m)
{
    for (int i = 0; i < ROWS; i++)
    {
        os << "| ";
        for (int j = 0; j < COLS; j++)
        {
            os << m.At(i, j);
            if (j < (COLS - 1)) os << ", ";
        }
        os << " |" << std::endl;
    }
    return os;
}
