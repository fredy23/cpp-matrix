#pragma once

#include <ostream>

using MatrixSize = std::size_t;

template<typename T>
class BaseMatrix
{
protected:
    BaseMatrix(MatrixSize p_rows, MatrixSize p_cols)
        : m_rows{p_rows}, m_cols{p_cols}
    {
    }

    void setData(T* p_data)
    {
        m_elementsData = p_data;
    }

    void fillBase(const T& p_value);
    const T& atBase(MatrixSize p_row, MatrixSize p_col) const;

    std::ostream& outputBase(std::ostream& p_out) const;

private:
    MatrixSize m_rows;
    MatrixSize m_cols;

    T* m_elementsData {nullptr};
};

template<typename T>
void BaseMatrix<T>::fillBase(const T& p_value)
{
    for(auto i = 0u; i < m_rows * m_cols; ++i)
    {
        m_elementsData[i] = p_value;
    }
}

template<typename T>
const T& BaseMatrix<T>::atBase(MatrixSize p_row, MatrixSize p_col) const
{
    if(p_row < 0 || p_row >= m_rows)
    {
        throw std::out_of_range("invalid matrix row index");
    }

    if(p_col < 0 || p_col >= m_cols)
    {
        throw std::out_of_range("invalid matrix column index");
    }

    return m_elementsData[p_row * m_cols + p_col];
}

template<typename T>
std::ostream& BaseMatrix<T>::outputBase(std::ostream& p_out) const
{
    if(m_elementsData == nullptr)
    {
        return p_out;
    }

    for(auto row = 0u; row < m_rows; ++row)
    {
        for(auto col = 0u; col < m_cols; ++col)
        {
            p_out << m_elementsData[row * m_cols + col] << ' ';
        }

        if(row < m_rows - 1)
        {
            p_out << '\n';
        }
    }

    return p_out;
}
