#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring> // memcpy
#include <exception>
#include <random>
#include <boost/format.hpp>

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

    Matrix(std::initializer_list<T> data_, uint32_t rows_, uint32_t cols_)
        : data(nullptr), rows(rows_), cols(cols_)
    {
        if (data_.size() != size())
            throw MyException(boost::str(boost::format("len of data: %1% != # matrix elem: %2%") %
                                         data_.size() % size()));
        data = new T[size()];
        memcpy(data, data_.begin(), sizeof(T) * size());
    }

    Matrix(T default_value, uint32_t rows_, uint32_t cols_)
        : data(nullptr), rows(rows_), cols(cols_)
    {
        data = new T[size()];
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                this->at(r, c) = default_value;
    }

    uint32_t size() { return rows * cols; }

    uint32_t num_rows() { return rows; }

    uint32_t num_cols() { return cols; }

    T *raw_ptr() { return data; }

    // change operator() to at()
    T &at(uint32_t row, uint32_t col)
    {
        if (row >= rows)
            throw MyException(boost::str(boost::format("row index out of range: row %1% >= rows %2%") %
                                         row % rows));
        if (col >= cols)
            throw MyException(boost::str(boost::format("column index out of range: col %1% >= cols %2%") %
                                         col % cols));
        return data[row * cols + col];
    }

    std::shared_ptr<Matrix<T>> operator* (std::shared_ptr<Matrix<T>> scalar)
    {
        std::shared_ptr<Matrix<T>> result(new Matrix<float>(0, this->rows, this->cols));
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                result->at(r, c) = this->at(r, c) * scalar->at(r, c);
        return result;
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
            os << m.at(r, c) << ' ';
        os << std::endl;
    }
    return os;
}

#endif
