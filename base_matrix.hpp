#pragma once

#include <ostream>
#include <algorithm>

using MatrixSize = std::size_t;

template<typename T>
class BaseMatrix
{
protected:
    BaseMatrix(MatrixSize p_rows, MatrixSize p_cols)
        : m_rows{p_rows}, m_cols{p_cols}
    {
    }

    void setData(T* p_elementsData)
    {
        m_elementsData = p_elementsData;
    }

    MatrixSize sizeBase() const
    {
        return m_rows * m_cols;
    }

    const T& atBase(MatrixSize p_row, MatrixSize p_col) const;
    void addToBase(const T* p_otherElementsData);
    void multiplyToDestBase(
        T* p_destElementsData,
        const T* p_transposedData,
        MatrixSize p_transposedRows) const;
    void scalarMulBase(const T& p_scalar);
    void transposeToDestBase(T* p_destElementsData) const;

    std::ostream& outputBase(std::ostream& p_out) const;

private:
    T innerProduct(const T* first1, const T* last1, const T* first2) const;

    MatrixSize m_rows;
    MatrixSize m_cols;

    T* m_elementsData {nullptr};
};

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

    return *(m_elementsData + (p_row * m_cols + p_col));
}

template<typename T>
void BaseMatrix<T>::addToBase(const T* p_otherElementsData)
{
    std::transform(
        m_elementsData,
        m_elementsData + sizeBase(),
        p_otherElementsData,
        m_elementsData,
        [](const T& p_first, const T& p_second)
    {
        return p_first + p_second;
    });
}

template<typename T>
T BaseMatrix<T>::innerProduct(const T* first1, const T* last1, const T* first2) const
{
    T result = *first1++ * *first2++;
    return std::inner_product(first1, last1, first2, result);
}

template<typename T>
void BaseMatrix<T>::multiplyToDestBase(
    T* p_destElementsData,
    const T* p_transposedData,
    MatrixSize p_transposedRows) const
{
    for(auto i = 0u; i < sizeBase(); i += m_cols)
    {
        for(auto j = 0u; j < m_cols * p_transposedRows; j += m_cols)
        {
            *(p_destElementsData++) = innerProduct(
                m_elementsData + i,
                m_elementsData + i + m_cols,
                p_transposedData + j);
        }
    }
}

template<typename T>
void BaseMatrix<T>::scalarMulBase(const T& p_scalar)
{
    std::transform(
        m_elementsData,
        m_elementsData + sizeBase(),
        m_elementsData,
        [&p_scalar](const T& p_elem)
    {
        return p_elem * p_scalar;
    });
}

template<typename T>
void BaseMatrix<T>::transposeToDestBase(T* p_destElementsData) const
{
    for(auto i = 0u; i < m_rows * m_cols; ++i)
    {
        int row = i / m_rows;
        int col = i % m_rows;
        *(p_destElementsData++) = *(m_elementsData + (col * m_cols + row));
    }
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
            p_out << *(m_elementsData + (row * m_cols + col)) << ' ';
        }

        if(row < m_rows - 1)
        {
            p_out << '\n';
        }
    }

    return p_out;
}
