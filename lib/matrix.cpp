/*
 * matrix.cpp
 * Author: Aven Bross,
 *         based on code by Michael Halls-Moore (blog post)
 * Date: 3/3/2015
 * 
 * Simple template matrix class I found to use for AES
 */
 
#ifndef QS_MATRIX_CPP
#define QS_MATRIX_CPP

#include "matrix.h"

// Parameter constructor with initial value
template<typename T>
QSMatrix<T>::QSMatrix(int rows, int cols, const T& _initial) {
    _mat.resize(rows);
    for (int i=0; i<_mat.size(); i++) {
        _mat[i].resize(cols, _initial);
    }
    _rows = rows;
    _cols = cols;
}

// Parameter constructor with initial vector
template<typename T>
QSMatrix<T>::QSMatrix(int rows, int cols, const vector<T> & _initial) {
    _mat.resize(rows);
    for (int i=0; i<_mat.size(); i++) {
        _mat[i].resize(cols);
        for (int j=0; j<_mat[i].size(); j++){
            _mat[i][j] = _initial[i*rows+j];
        }
    }
    _rows = rows;
    _cols = cols;
}

// Copy constructor
template<typename T>
QSMatrix<T>::QSMatrix(const QSMatrix<T>& rhs) {
    _mat = rhs._mat;
    _rows = rhs.getRows();
    _cols = rhs.getCols();
}

// Deconstructor
template<typename T>
QSMatrix<T>::~QSMatrix() {}

// Assignment operator
template<typename T>
QSMatrix<T>& QSMatrix<T>::operator=(const QSMatrix<T>& rhs) {
    if (&rhs == this)
        return *this;

    int newrows = rhs.getRows();
    int newcols = rhs.getCols();

    _mat.resize(newrows);
    for (int i=0; i<_mat.size(); i++) {
        _mat[i].resize(newcols);
    }

    for (int i=0; i<newrows; i++) {
        for (int j=0; j<newcols; j++) {
            _mat[i][j] = rhs(i, j);
        }
    }
    _rows = newrows;
    _cols = newcols;

    return *this;
}

// Add two matrices
template<typename T>
QSMatrix<T> QSMatrix<T>::operator+(const QSMatrix<T>& rhs) const {
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[i][j] + rhs(i,j);
        }
    }

    return result;
}

// Add two matrices
template<typename T>
QSMatrix<T>& QSMatrix<T>::operator+=(const QSMatrix<T>& rhs) {
    int _rows = rhs.getRows();
    int _cols = rhs.getCols();

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            this->_mat[i][j] += rhs(i,j);
        }
    }

    return *this;
}

// Subtract two matrices
template<typename T>
QSMatrix<T> QSMatrix<T>::operator-(const QSMatrix<T>& rhs) const {
    int _rows = rhs.getRows();
    int _cols = rhs.getCols();
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[i][j] - rhs(i,j);
        }
    }

    return result;
}

// Subtract two matrices
template<typename T>
QSMatrix<T>& QSMatrix<T>::operator-=(const QSMatrix<T>& rhs) {
    int _rows = rhs.getRows();
    int _cols = rhs.getCols();

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            this->_mat[i][j] -= rhs(i,j);
        }
    }

    return *this;
}

// Multiply two matrices
template<typename T>
QSMatrix<T> QSMatrix<T>::operator*(const QSMatrix<T>& rhs) const {
    int _rows = rhs.getRows();
    int _cols = rhs.getCols();
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            for (int k=0; k<_rows; k++) {
                result(i,j) += this->_mat[i][k] * rhs(k,j);
            }
        }
    }

    return result;
}

// Multiply two matrices
template<typename T>
QSMatrix<T>& QSMatrix<T>::operator*=(const QSMatrix<T>& rhs) {
    QSMatrix result = (*this) * rhs;
    (*this) = result;
    return *this;
}

// Find transpose
template<typename T>
QSMatrix<T> QSMatrix<T>::transpose() const {
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[j][i];
        }
    }

    return result;
}

// Addition by scalar
template<typename T>
QSMatrix<T> QSMatrix<T>::operator+(const T& rhs) const {
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[i][j] + rhs;
        }
    }

    return result;
}

// Subtraction by scalar
template<typename T>
QSMatrix<T> QSMatrix<T>::operator-(const T& rhs) const {
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[i][j] - rhs;
        }
    }

    return result;
}

// Multiplication by scalar
template<typename T>
QSMatrix<T> QSMatrix<T>::operator*(const T& rhs) const {
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[i][j] * rhs;
        }
    }

    return result;
}

// Division by scalar
template<typename T>
QSMatrix<T> QSMatrix<T>::operator/(const T& rhs) const {
    QSMatrix result(_rows, _cols, 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result(i,j) = this->_mat[i][j] / rhs;
        }
    }

    return result;
}

// Multiply vector
template<typename T>
std::vector<T> QSMatrix<T>::operator*(const std::vector<T>& rhs) const {
    std::vector<T> result(rhs.size(), 0.0);

    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            result[i] = this->_mat[i][j] * rhs[j];
        }
    }

    return result;
}

// Get diagonal elements
template<typename T>
std::vector<T> QSMatrix<T>::diag_vec() {
    std::vector<T> result(_rows, 0.0);

    for (int i=0; i<_rows; i++) {
        result[i] = this->_mat[i][i];
    }

    return result;
}

// Access matrix elements
template<typename T>
T& QSMatrix<T>::operator()(const int& row, const int& col) {
    return this->_mat[row][col];
}

// Access matrix elements
template<typename T>
const T& QSMatrix<T>::operator()(const int& row, const int& col) const {
    return this->_mat[row][col];
}

// Get number of _rows
template<typename T>
int QSMatrix<T>::getRows() const {
    return this->_rows;
}

// Get number of columns
template<typename T>
int QSMatrix<T>::getCols() const {
    return this->_cols;
}

#endif
