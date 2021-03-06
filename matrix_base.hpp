#pragma once

#include <ostream>
#include <algorithm>

using MatrixSize = std::size_t;

template<typename T>
class MatrixBase
{
protected:
    MatrixBase(MatrixSize p_rows, MatrixSize p_cols)
        : m_rows{p_rows}, m_cols{p_cols}
    {
    }

    void baseSetElementsData(T* p_elementsData)
    {
        m_elementsData = p_elementsData;
    }

    MatrixSize baseSize() const
    {
        return m_rows * m_cols;
    }

    const T& baseAt(MatrixSize p_row, MatrixSize p_col) const;
    void baseAdd(const T* p_otherElementsData);
    void baseMultiplyTo(T* p_destElementsData, const T* p_transposedData, MatrixSize p_transposedRows) const;
    void baseMultiplyByScalar(const T& p_scalar);
    void baseTransposeTo(T* p_destElementsData) const;

    std::ostream& baseOutput(std::ostream& p_out) const;

private:
    static T innerProduct(const T* first1, const T* last1, const T* first2);

    MatrixSize m_rows;
    MatrixSize m_cols;

    T* m_elementsData {nullptr};
};

template<typename T>
const T& MatrixBase<T>::baseAt(MatrixSize p_row, MatrixSize p_col) const
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
void MatrixBase<T>::baseAdd(const T* p_otherElementsData)
{
    std::transform(
        m_elementsData,
        m_elementsData + baseSize(),
        p_otherElementsData,
        m_elementsData,
        [](const T& p_first, const T& p_second)
    {
        return p_first + p_second;
    });
}

template<typename T>
T MatrixBase<T>::innerProduct(const T* first1, const T* last1, const T* first2)
{
    T result = *first1++ * *first2++;
    return std::inner_product(first1, last1, first2, result);
}

template<typename T>
void MatrixBase<T>::baseMultiplyTo(T* p_destElementsData, const T* p_transposedData, MatrixSize p_transposedRows) const
{
    for(auto i = 0u; i < baseSize(); i += m_cols)
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
void MatrixBase<T>::baseMultiplyByScalar(const T& p_scalar)
{
    std::transform(
        m_elementsData,
        m_elementsData + baseSize(),
        m_elementsData,
        [&p_scalar](const T& p_elem)
    {
        return p_elem * p_scalar;
    });
}

template<typename T>
void MatrixBase<T>::baseTransposeTo(T* p_destElementsData) const
{
    for(auto i = 0u; i < m_rows * m_cols; ++i)
    {
        int row = i / m_rows;
        int col = i % m_rows;
        *(p_destElementsData++) = *(m_elementsData + (col * m_cols + row));
    }
}

template<typename T>
std::ostream& MatrixBase<T>::baseOutput(std::ostream& p_out) const
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
