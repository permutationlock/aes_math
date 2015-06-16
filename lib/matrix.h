/*
 * matrix.h
 * Author: Aven Bross,
 *         based on code by Michael Halls-Moore (blog post)
 * Date: 3/3/2015
 * 
 * Simple template matrix class I found to use for AES
 */

#ifndef QS_MATRIX_H
#define QS_MATRIX_H

#include <vector>
#include <ostream>

using std::vector;
using std::ostream;

template <typename T> class QSMatrix {
private:
    vector<vector<T> > _mat;
    int _rows;
    int _cols;

public:
    QSMatrix(int rows, int cols, const T& _initial);
    QSMatrix(int rows, int cols, const vector<T> & _initial);
    QSMatrix(const QSMatrix<T>& rhs);
    virtual ~QSMatrix();

    // Operator overloading, for "standard" mathematical matrix operations
    QSMatrix<T>& operator=(const QSMatrix<T>& rhs);

    // Matrix mathematical operations
    QSMatrix<T> operator+(const QSMatrix<T>& rhs) const;
    QSMatrix<T>& operator+=(const QSMatrix<T>& rhs);
    QSMatrix<T> operator-(const QSMatrix<T>& rhs) const;
    QSMatrix<T>& operator-=(const QSMatrix<T>& rhs);
    QSMatrix<T> operator*(const QSMatrix<T>& rhs) const;
    QSMatrix<T>& operator*=(const QSMatrix<T>& rhs);
    QSMatrix<T> transpose() const;

    // Matrix/scalar operations
    QSMatrix<T> operator+(const T& rhs) const;
    QSMatrix<T> operator-(const T& rhs) const;
    QSMatrix<T> operator*(const T& rhs) const;
    QSMatrix<T> operator/(const T& rhs) const;

    // Matrix/vector operations
    vector<T> operator*(const vector<T>& rhs) const;
    vector<T> diag_vec();

    // Access the individual elements
    T& operator()(const int& row, const int& col);
    const T& operator()(const int& row, const int& col) const;

    // Access the row and column sizes
    int getRows() const;
    int getCols() const;
};

#include "matrix.cpp"   // Compile implementation since it is a template class

#endif
