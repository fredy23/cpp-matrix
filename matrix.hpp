#pragma once

#include <ostream>
#include <initializer_list>
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <type_traits>

#include "matrix_base.hpp"

template<typename T, MatrixSize Rows, MatrixSize Cols = Rows>
class Matrix : private MatrixBase<T>
{
public:
    using value_type        = T; 
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;

    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = T*;
    using const_pointer     = const T*;

    Matrix();
    Matrix(const std::initializer_list<T>& p_elements);
    Matrix(const Matrix& p_other);
    Matrix& operator=(const Matrix& p_other);
    Matrix(Matrix&& p_other) noexcept;
    Matrix& operator=(Matrix&& p_other) noexcept;
    ~Matrix();

    reference at(MatrixSize p_row, MatrixSize p_col)
    {
        return const_cast<reference>(static_cast<const Matrix&>(*this).at(p_row, p_col));
    }

    const_reference at(MatrixSize p_row, MatrixSize p_col) const
    {
        return MatrixBase<T>::baseAt(p_row, p_col);
    }

    class MatrixRow
    {
    public:
        MatrixRow(pointer p_elementsData, MatrixSize p_row)
            : m_elementsData{p_elementsData}, m_row{p_row}
        {
        }

        reference operator[](MatrixSize p_col) noexcept
        {
            return *(m_elementsData + (m_row * Cols + p_col));
        }

        const_reference operator[](MatrixSize p_col) const noexcept
        {
            return *(m_elementsData + (m_row * Cols + p_col));
        }

    private:
        pointer m_elementsData {nullptr};
        MatrixSize m_row;
    };

    MatrixRow operator[](MatrixSize p_row) noexcept
    {
        return MatrixRow(m_elements, p_row);
    }

    const MatrixRow operator[](MatrixSize p_row) const noexcept
    {
        return MatrixRow(m_elements, p_row);
    }

    pointer data() noexcept
    {
        return m_elements;
    }

    const_pointer data() const noexcept
    {
        return m_elements;
    }

    template<typename IteratorType>
    class MatrixIterator
    {
    private:
        using traits_type       = std::iterator_traits<IteratorType>;

    public:
        using value_type        = typename traits_type::value_type;
        using difference_type   = typename traits_type::difference_type;
        using reference         = typename traits_type::reference;
        using pointer           = typename traits_type::pointer;
        using iterator_category = typename traits_type::iterator_category;

        MatrixIterator()
            : m_elementsData{nullptr}
        {
        }

        explicit MatrixIterator(const IteratorType& p_elementsData)
            : m_elementsData{p_elementsData}
        {
        }

        template<typename Iterator, typename = typename std::enable_if<
            std::is_same<Iterator, typename Matrix::pointer>::value>::type
        >
        MatrixIterator(const MatrixIterator<Iterator>& p_iterator)
            : m_elementsData{p_iterator.base()}
        {
        }

        const IteratorType& base() const noexcept
        {
            return m_elementsData;
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

        reference& operator[](difference_type p_distance) const noexcept
        {
            return *(m_elementsData + p_distance);
        }

        friend const MatrixIterator operator+(
            difference_type p_distance,
            const MatrixIterator& p_other) noexcept
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
        IteratorType m_elementsData;
    };

    using iterator                  = MatrixIterator<pointer>;
    using const_iterator            = MatrixIterator<const_pointer>;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

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

    const_iterator cbegin() const noexcept
    {
        return const_iterator(m_elements);
    }

    const_iterator cend() const noexcept
    {
        return const_iterator(m_elements + size());
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    size_type size() const noexcept
    {
        return MatrixBase<T>::baseSize();
    }

    Matrix& operator+=(const Matrix& p_other)
    {
        MatrixBase<T>::baseAdd(p_other.data());
        return *this;
    }

    const Matrix operator+(const Matrix& p_other) const
    {
        return Matrix(*this) += p_other;
    }

    const Matrix<T, Cols, Rows> transpose() const
    {
        Matrix<T, Cols, Rows> result {};
        MatrixBase<T>::baseTransposeTo(result.data());
        return result;
    }

    template<MatrixSize RowsOther, MatrixSize ColsOther>
    const Matrix<T, Rows, ColsOther> operator*(const Matrix<T, RowsOther, ColsOther>& p_other)
    {
        static_assert(RowsOther == Cols, "Matrix dimensions must agree");

        Matrix<T, Rows, ColsOther> result {};
        MatrixBase<T>::baseMultiplyTo(result.data(), p_other.transpose().data(), ColsOther);
        return result;
    }

    Matrix& operator*=(const T& p_scalar)
    {
        MatrixBase<T>::baseMultiplyByScalar(p_scalar);
        return *this;
    }

    const Matrix operator*(const T& p_scalar) const
    {
        return Matrix(*this) *= p_scalar;
    }

    friend const Matrix operator*(const T& p_scalar, const Matrix& p_matrix)
    {
        return p_matrix * p_scalar;
    }

    friend std::ostream& operator<<(std::ostream& p_out, const Matrix& p_matrix)
    {
        return p_matrix.output(p_out);
    }

private:
    pointer m_elements;

    std::ostream& output(std::ostream& p_out) const
    {
        return MatrixBase<T>::baseOutput(p_out);
    }
};

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix()
    : MatrixBase<T>(Rows, Cols)
{
    static_assert(Rows > 0 && Cols > 0, "Invalid matrix dimension");

    m_elements = new T[Rows * Cols];
    MatrixBase<T>::baseSetElementsData(m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix(const std::initializer_list<T>& p_elements)
    : Matrix()
{
    auto offset = std::max<int>(0, p_elements.size() - Rows * Cols);
    std::copy(p_elements.begin(), p_elements.end() - offset, m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::Matrix(const Matrix& p_other)
    : Matrix()
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
    : MatrixBase<T>(Rows, Cols), m_elements{p_other.m_elements}
{
    p_other.m_elements = nullptr;
    p_other.MatrixBase<T>::baseSetElementsData(p_other.m_elements);
    MatrixBase<T>::baseSetElementsData(m_elements);
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>& Matrix<T, Rows, Cols>::operator=(Matrix&& p_other) noexcept
{
    std::swap(m_elements, p_other.m_elements);
    p_other.MatrixBase<T>::baseSetElementsData(p_other.m_elements);
    MatrixBase<T>::baseSetElementsData(m_elements);
    return *this;
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
Matrix<T, Rows, Cols>::~Matrix()
{
    delete[] m_elements;
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
bool operator==(
    const Matrix<T, Rows, Cols>& p_left,
    const Matrix<T, Rows, Cols>& p_right)
{
    return std::equal(p_left.begin(), p_left.end(), p_right.begin());
}

template<typename T, MatrixSize Rows, MatrixSize Cols>
bool operator!=(
    const Matrix<T, Rows, Cols>& p_left,
    const Matrix<T, Rows, Cols>& p_right)
{
    return !(p_left == p_right);
}
