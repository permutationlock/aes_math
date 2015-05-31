/*
 * modular_arithmetic.h
 * Author: Aven Bross, 
 *         based on code by Riccardo Murri (Stack Overflow)
 * Date: 2/24/2015
 * Template class to do modular arithmetic
 */
 
#ifndef MODULAR_ARITHMETIC_H
#define MODULAR_ARITHMETIC_H

template<typename T> // `T` is an integer type
class Modular 
{
public:
    Modular(const T& value);

    // Sets the modulus
    static void globalSetModulus(const T& modulus);
    
    Modular<T> addInverse() const;

    // Finds multiplicative inverse via extended euclidean algorithm
    Modular<T> mulInverse() const;

    // Self modifying arithmethic operations
    Modular<T>& operator=(const Modular<T>& other);
    Modular<T>& operator+=(const Modular<T>& other);
    Modular<T>& operator-=(const Modular<T>& other);
    Modular<T>& operator*=(const Modular<T>& other);
    Modular<T>& operator/=(const Modular<T>& other);

    // Non modifying arithmetic operations
    Modular<T> operator+(const Modular<T>& b) const;
    Modular<T> operator-(const Modular<T>& b) const;
    Modular<T> operator*(const Modular<T>& b) const;
    Modular<T> operator/(const Modular<T>& b) const;
    
    // Comparison operations
    bool operator==(const Modular<T>& other);
    bool operator<(const Modular<T>& other);
    bool operator>(const Modular<T>& other);
    bool operator<=(const Modular<T>& other);
    bool operator>=(const Modular<T>& other);
    
    T value() const;

private:
    T _val;
    static T _modulus;

};

#include "modular_arithmetic.cpp"   // Compile implementation since it is a template class

#endif
