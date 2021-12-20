#ifndef MATRIX_H
#define MATRIX_H
#include <random>
#include <iostream>
#include <cstring> // memcpy
#include <exception>
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
    Matrix();
    Matrix(std::initializer_list<T> data_, uint16_t rows_, uint16_t cols_)
        : data(nullptr), rows(rows_), cols(cols_)
    {
        if (data_.size() != size())
            throw MyException(boost::str(boost::format("len of data: %1% != # matrix elem: %2%") %
                                         data_.size() % size()));
        data = new T[size()];
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

    uint16_t size() { return rows * cols; }

    uint16_t num_rows() { return rows; }

    uint16_t num_cols() { return cols; }

    const T *raw_ptr() { return data; }

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

    Matrix operator+(Matrix<T> other)
    {
        if (this->num_rows() != other.num_rows())
            throw MyException(boost::str(boost::format("different rows: %1% != %2%") % 
                                        this->num_rows() % other.num_rows()));
        if (this->num_cols() != other.num_cols())
            throw MyException(boost::str(boost::format("different cols: %1% != %2%") % 
                                        this->num_cols() % other.num_cols()));
        Matrix<T> result = std::make_unique<Matrix<T>>(0, this->rows, this->cols);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                result->at(r, c) = this->at(r, c) + other.at(r, c);
        return std::move(result);
    }



    Matrix  operator*(Matrix<T> other)
    {
        std::cout<<"matrix*"<<std::endl;
        Matrix<T> result = std::make_unique<Matrix<T>>(0, this->rows, this->cols);
        for (int r = 0; r < this->rows; ++r)
            for (int c = 0; c < other.cols; ++c)
                for (int s = 0; s < this->cols; ++s)
                    result->at(r, c) += this->at(r, s) * other.at(s, c);
        return result;
        return std::move(result);
    }
    

    Matrix row_ptr(uint16_t rows)
    {
        
        Matrix<T> slice_matirx = Matrix<T>(0, 1, this->cols);
        for(int i=0; i<this->cols; i++)
        {
            slice_matirx.at(0, i) = this->at(rows, i);
        }
        auto sliced_matrix = std::make_unique<Matrix<T>>(slice_matirx);
        return std::move(sliced_matrix);
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
