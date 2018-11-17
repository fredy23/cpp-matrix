#pragma once

#include <ostream>
#include <initializer_list>
#include <algorithm>
#include <cstdint>

#include "base_matrix.hpp"

template<typename T, MatrixSize Rows, MatrixSize Cols = Rows>
class Matrix : private BaseMatrix<T>
{
public:
    using value_type = T;

    Matrix(const std::initializer_list<T>& p_elements);
    Matrix(const Matrix& p_other);
    Matrix& operator=(const Matrix& p_other);
    Matrix(Matrix&& p_other);
    Matrix& operator=(Matrix&& p_other);
    ~Matrix();

    const T& at(MatrixSize p_row, MatrixSize p_col) const;
    T& at(MatrixSize p_row, MatrixSize p_col);

    void fill(const T& p_value)
    {
        BaseMatrix<T>::fillBase(p_value);
    }

    void zeros()
    {
        fill(0);
    }

    void ones()
    {
        fill(1);
    }

    class MatrixRow
    {
    public:
        MatrixRow(T* p_elementsData, MatrixSize p_row)
            : m_elementsData{p_elementsData}, m_row{p_row}
        {
        }

        T& operator[](int p_col)
        {
            return m_elementsData[m_row * Cols + p_col];
        }

        const T& operator[](int p_col) const
        {
            return m_elementsData[m_row * Cols + p_col];    
        }

    private:
        T* m_elementsData {nullptr};
        MatrixSize m_row;
    };

    MatrixRow operator[](int p_row)
    {
        return MatrixRow(m_elements, p_row);
    }

    const MatrixRow operator[](int p_row) const
    {
        return MatrixRow(m_elements, p_row);
    }

    friend std::ostream& operator<<(std::ostream& p_out, const Matrix& p_matrix)
    {
        return p_matrix.output(p_out);
    }

private:
    T* m_elements;

    std::ostream& output(std::ostream& p_out) const
    {
        return BaseMatrix<T>::outputBase(p_out);
    }
};

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix(const std::initializer_list<T>& p_elements)
    : BaseMatrix<T>(Rows, Cols)
{
    static_assert(Rows > 0 && Cols > 0, "invalid matrix size");

    m_elements = new T[Rows * Cols];

    auto offset = std::max<int>(0, p_elements.size() - Rows * Cols);
    std::copy(p_elements.begin(), p_elements.end() - offset, m_elements);

    BaseMatrix<T>::setData(m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix(const Matrix& p_other)
    : Matrix(std::initializer_list<T>{})
{
    std::copy(p_other.m_elements, p_other.m_elements + (Rows * Cols), m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator=(const Matrix& p_other)
{
    if(this == &p_other)
    {
        return *this;
    }

    std::copy(p_other.m_elements, p_other.m_elements + (Rows * Cols), m_elements);

    return *this;
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix(Matrix&& p_other)
    : BaseMatrix<T>(Rows, Cols), m_elements{p_other.m_elements}
{
    p_other.m_elements = nullptr;
    p_other.BaseMatrix<T>::setData(p_other.m_elements);
    BaseMatrix<T>::setData(m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator=(Matrix&& p_other)
{
    std::swap(m_elements, p_other.m_elements);
    p_other.BaseMatrix<T>::setData(p_other.m_elements);
    BaseMatrix<T>::setData(m_elements);
    
    return *this;
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::~Matrix()
{
    delete[] m_elements;
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
const T& Matrix<T, Rows, Cols>::at(MatrixSize p_row, MatrixSize p_col) const
{
    return BaseMatrix<T>::atBase(p_row, p_col);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
T& Matrix<T, Rows, Cols>::at(MatrixSize p_row, MatrixSize p_col)
{
    return const_cast<T&>(static_cast<const Matrix<T, Rows, Cols>&>(*this).at(p_row, p_col));
}
