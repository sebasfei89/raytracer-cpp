#include "Matrix.h"

template<>
float Matrix<2, 2>::Determinant() const
{
    return m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
}
