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
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    Matrix(const std::initializer_list<T>& p_elements);
    Matrix(const Matrix& p_other);
    Matrix& operator=(const Matrix& p_other);
    Matrix(Matrix&& p_other) noexcept;
    Matrix& operator=(Matrix&& p_other) noexcept;
    ~Matrix();

    T& at(MatrixSize p_row, MatrixSize p_col)
    {
        return const_cast<T&>(static_cast<const Matrix&>(*this).at(p_row, p_col));
    }

    const T& at(MatrixSize p_row, MatrixSize p_col) const
    {
        return BaseMatrix<T>::atBase(p_row, p_col);
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

    T* data() noexcept
    {
        return m_elements;
    }

    const T* data() const noexcept
    {
        return m_elements;
    }

    template<typename U>
    class MatrixIterator
    {
    public:
        using value_type = Matrix::value_type;
        using difference_type = Matrix::difference_type;
        using reference = Matrix::reference;
        using pointer = Matrix::pointer;
        using iterator_category = std::random_access_iterator_tag;

        MatrixIterator()
            : m_elementsData{nullptr}
        {
        }

        MatrixIterator(U* p_elementsData)
            : m_elementsData{p_elementsData}
        {
        }

        reference operator*() const noexcept
        {
            return *m_elementsData;
        }

        pointer operator->() const noexcept
        {
            return m_elementsData;
        }

        MatrixIterator& operator++() noexcept
        {
            ++m_elementsData;
            return *this;
        }

        MatrixIterator operator++(int) noexcept
        {
            return MatrixIterator(m_elementsData++);
        }

        MatrixIterator& operator--() noexcept
        {
            --m_elementsData;
            return *this;
        }

        MatrixIterator operator--(int) noexcept
        {
            return MatrixIterator(m_elementsData--);
        }

        MatrixIterator& operator+=(difference_type p_distance) noexcept
        {
            m_elementsData += p_distance;
            return *this;
        }

        const MatrixIterator operator+(difference_type p_distance) const noexcept
        {
            return MatrixIterator(m_elementsData + p_distance);
        }

        MatrixIterator& operator-=(difference_type p_distance) noexcept
        {
            m_elementsData -= p_distance;
            return *this;
        }

        const MatrixIterator operator-(difference_type p_distance) const noexcept
        {
            return MatrixIterator(m_elementsData - p_distance);
        }

        reference& operator[](std::size_t p_index)
        {
            return m_elementsData[p_index];
        }

        friend const MatrixIterator operator+(
            difference_type p_distance,
            const MatrixIterator& p_other)
        {
            return MatrixIterator(p_other.m_elementsData + p_distance);
        }

        friend bool operator==(
            const MatrixIterator& p_left,
            const MatrixIterator& p_right) noexcept
        {
            return p_left.m_elementsData == p_right.m_elementsData;
        }

        friend bool operator!=(
            const MatrixIterator& p_left,
            const MatrixIterator& p_right) noexcept
        {
            return !(p_left == p_right);
        }

        friend bool operator<(
            const MatrixIterator& p_left,
            const MatrixIterator& p_right) noexcept
        {
            return p_left.m_elementsData < p_right.m_elementsData;
        }

        friend bool operator>(
            const MatrixIterator& p_left,
            const MatrixIterator& p_right) noexcept
        {
            return p_right.m_elementsData < p_left.m_elementsData;
        }

        friend bool operator<=(
            const MatrixIterator& p_left,
            const MatrixIterator& p_right) noexcept
        {
            return !(p_right.m_elementsData < p_left.m_elementsData);
        }

        friend bool operator>=(
            const MatrixIterator& p_left,
            const MatrixIterator& p_right) noexcept
        {
            return !(p_left.m_elementsData < p_right.m_elementsData);
        }

    private:
        U* m_elementsData;
    };

    using iterator = MatrixIterator<T>;
    using const_iterator = MatrixIterator<const T>;

    iterator begin() noexcept
    {
        return iterator(m_elements);
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(m_elements);
    }

    iterator end() noexcept
    {
        return iterator(m_elements + size());
    }

    const_iterator end() const noexcept
    {
        return const_iterator(m_elements + size());
    }

    size_type size() const noexcept
    {
        return Rows * Cols;
    }

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
    std::copy(p_other.m_elements, p_other.m_elements + (Rows * Cols), m_elements);
    return *this;
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix(Matrix&& p_other) noexcept
    : BaseMatrix<T>(Rows, Cols), m_elements{p_other.m_elements}
{
    p_other.m_elements = nullptr;
    p_other.BaseMatrix<T>::setData(p_other.m_elements);
    BaseMatrix<T>::setData(m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator=(Matrix&& p_other) noexcept
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
