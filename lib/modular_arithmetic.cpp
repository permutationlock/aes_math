/*
 * modular_arithmetic.cpp
 * Author: Aven Bross, 
 *         based on code by Riccardo Murri (Stack Overflow)
 * Date: 2/24/2015
 * Template class to do modular arithmetic
 */
 
#ifndef MODULAR_ARITHMETIC_CPP
#define MODULAR_ARITHMETIC_CPP

#include "modular_arithmetic.h"

template<typename T>
T Modular<T>::_modulus;
 
template<typename T>
Modular<T>::Modular(const T& value) : _val(value) { };

// Sets the modulus
template<typename T>
void Modular<T>::globalSetModulus(const T& modulus) {
    _modulus = modulus;
}

template<typename T>
Modular<T> Modular<T>::addInverse() const{
    return Modular<T>(_modulus-_val);
}

// Finds multiplicative inverse via extended euclidean algorithm
template<typename T>
Modular<T> Modular<T>::mulInverse() const{
    T t = 0;
    T r = _modulus;
    T new_t = 1;
    T new_r = _val;
    while(new_r != 0){
        T quotient = r / new_r;
        T temp_t = t;
        t = new_t;
        new_t = temp_t - quotient * new_t;
        T temp_r = r;
        r = new_r;
        new_r = temp_r - quotient * new_r;
    }
    if(r>1) throw;    // Should never happen for prime field
    if(t<0) t = t+_modulus;
    return Modular<T>(t);
}

// Self modifying arithmethic operations
template<typename T>
Modular<T>& Modular<T>::operator=(const Modular<T>& other) {
    _val = other._val;
    return *this;
}

template<typename T>
Modular<T>& Modular<T>::operator+=(const Modular<T>& other) {
    _val += other._val;
    _val %= _modulus;
    return *this;
}

template<typename T>
Modular<T>& Modular<T>::operator-=(const Modular<T>& other) {
    _val += _modulus - other._val;
    _val %= _modulus;
    return *this;
}

template<typename T>
Modular<T>& Modular<T>::operator*=(const Modular<T>& other) {
    _val *= other._val; 
    _val %= _modulus;
    return *this;
}

template<typename T>
Modular<T>& Modular<T>::operator/=(const Modular<T>& other) {
    _val *= other.mulInverse()._val;
    _val %= _modulus; return *this;
}

// Non modifying arithmetic operations
template<typename T>
Modular<T> Modular<T>::operator+(const Modular<T>& b) const {
    return Modular<T>((this->_val + b._val) % _modulus);
}

template<typename T>
Modular<T> Modular<T>::operator-(const Modular<T>& b) const {
    return Modular<T>((this->_val + (_modulus - b._val)) % _modulus);
}

template<typename T>
Modular<T> Modular<T>::operator*(const Modular<T>& b) const {
    return Modular<T>((this->_val * b._val) % _modulus);
}

template<typename T>
Modular<T> Modular<T>::operator/(const Modular<T>& b) const {
    return Modular<T>((this->_val * b.mulInverse()._val) % _modulus);
}

// Comparison operations
template<typename T>
bool Modular<T>::operator==(const Modular<T>& other) {
    return _val == other._val;
}

template<typename T>
bool Modular<T>::operator<(const Modular<T>& other) {
    return _val < other._val;
}

template<typename T>
bool Modular<T>::operator>(const Modular<T>& other) {
    return _val > other._val;
}

template<typename T>
bool Modular<T>::operator<=(const Modular<T>& other) {
    return _val <= other._val;
}

template<typename T>
bool Modular<T>::operator>=(const Modular<T>& other) {
    return _val >= other._val;
}

template<typename T>
T Modular<T>::value() const{
    return _val;
}

#endif
