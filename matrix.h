#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring> // memcpy
#include <exception>
#include <boost/format.hpp>
#include <cstdlib>
#include <ctime>

using boost::str;
using boost::format;

class MyException : public std::exception
{
public:
    MyException(std::string msg_) : msg(msg_) {}
    const char *what() const noexcept override { return msg.c_str(); }

private:
    std::string msg;
};

template <typename T>
class Matrix
{
public:
    Matrix(std::initializer_list<T> data_, uint16_t rows_, uint16_t cols_)
        : data(nullptr), rows(rows_), cols(cols_)
    {
        if (data_.size() != size())
            throw MyException(boost::str(boost::format("len of data: %1% != # matrix elem: %2%") %
                                         data_.size() % size()));
        data = new int[size()];
        memcpy(data, data_.begin(), sizeof(int) * size());
    }

    Matrix(T default_value, uint16_t rows_, uint16_t cols_)
        : data(nullptr), rows(rows_), cols(cols_)
    {
        data = new T[size()];
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                this->at(r, c) = default_value;
    }

    Matrix(Matrix<T> &other)
        : data(nullptr), rows(other.num_rows()), cols(other.num_cols())
    {
        data = new T[other.size()];
        memcpy(data, other.raw_ptr(), sizeof(int) * size());
    }

    static std::unique_ptr<Matrix<T>> random_matrix_factory(uint16_t rows, uint16_t cols)
    {
        // HW2: Implement this fuction
        // fill a matrix with uniformly distributed random numbers between -1~1
        srand(time(NULL));
        float randTemp = 0;
        int a = -1;
        int b = 1;
        std::unique_ptr<Matrix<T>> ptr = std::make_unique<Matrix<T>>(0, rows, cols);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                {
                    randTemp = rand() % (b+1-a) + a;
                    ptr->at(r, c) = randTemp;
                    //result->at(r, c) = this->at(r, c) + other.at(r, c);
                }
        return std::move(ptr);
    }

    std::unique_ptr<Matrix<T>> row_ptr(uint16_t r)
    {
        // HW2: Implement this fuction
        int col = this->num_cols();
        std::unique_ptr<Matrix<T>> slcie_data = std::make_unique<Matrix<T>>(0, 1, col);
        for (int c = 0; c < col; ++c)
            {
                slcie_data->at(0, c) = this->at(r, c);
            }
        return std::move(slcie_data);
    }


    const uint16_t size() noexcept { return rows * cols; }

    const uint16_t num_rows() noexcept { return rows; }

    const uint16_t num_cols() noexcept{ return cols; }

    const T *raw_ptr() noexcept { return data; }

    // change operator() to at()
    T &at(uint16_t row, uint16_t col)
    {
        if (row >= rows)
            throw MyException(boost::str(boost::format("row index out of range: row %1% >= rows %2%") %
                                         row % rows));
        if (col >= cols)
            throw MyException(boost::str(boost::format("column index out of range: col %1% >= cols %2%") %
                                         col % cols));
        return data[row * cols + col];
    }

    std::unique_ptr<Matrix<T>> operator+(Matrix<T> other)
    {
        if (this->num_rows() != other.num_rows())
            throw MyException(boost::str(boost::format("different rows: %1% != %2%") %
                                         this->num_rows() % other.num_rows()));
        if (this->num_cols() != other.num_cols())
            throw MyException(boost::str(boost::format("different cols: %1% != %2%") %
                                         this->num_cols() % other.num_cols()));
        std::unique_ptr<Matrix<T>> result = std::make_unique<Matrix<T>>(0, this->rows, this->cols);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                result->at(r, c) = this->at(r, c) + other.at(r, c);
        return std::move(result);
    }

    std::unique_ptr<Matrix<T>> operator-(T scalar)
    {
        std::unique_ptr<Matrix<T>> result = std::make_unique<Matrix<T>>(0, this->rows, this->cols);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                result->at(r, c) = this->at(r, c) - scalar;
        return std::move(result);
    }

    std::unique_ptr<Matrix<T>> operator*(Matrix<T> other)
    {
        // HW2: Implement this fuction
        if (this->num_cols() != other.num_rows())
            throw MyException(str(format("different rows and cols: %1% != %2%") %this->num_cols() % other.num_rows()));

        std::unique_ptr<Matrix<T>> result = std::make_unique<Matrix<T>>(0, this->rows, other.num_cols());
        for (int r = 0; r < this->num_rows(); ++r)
            for (int c = 0; c < other.num_cols(); ++c)
                for (int k = 0; k < this->num_cols(); ++k)
                    result->at(r, c) += this -> at(r, k) * other.at(k, c);
        return std::move(result);
    }

    std::unique_ptr<Matrix<T>> operator^(T scalar)
    {
        // HW2: Implement this fuction
        std::unique_ptr<Matrix<T>> result = std::make_unique<Matrix<T>>(0, this->rows,  this->cols);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                result->at(r, c) = this->at(r, c) * scalar;
        return std::move(result);
    }



private:
    T *data;
    uint16_t rows;
    uint16_t cols;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, Matrix<T> &m)
{
    for (int r = 0; r < m.num_rows(); ++r)
    {
        for (int c = 0; c < m.num_cols(); ++c)
            os << m(r, c) << ' ';
        os << std::endl;
    }
    return os;
}

#endif
