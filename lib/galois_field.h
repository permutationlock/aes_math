/*
 * galois_field.h
 * Author: Aven Bross
 * Date: 2/20/2015
 * 
 * Implementation of galois field, allows construction of
 * objects in a galois field, addition, multiplication
 * and extended euclidean algorithm for finding inverses.
 */

#ifndef GALOIS_FIELD_H
#define GALOIS_FIELD_H

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "modular_arithmetic.h"

using std::vector;
using std::string;
using std::pow;
using std::max;
using std::runtime_error;
using std::to_string;

/*
 * Polynomial
 * Represents a polynomial of the form a_n*x^n+...+a_1*x+a_0
 * Allows for add, subtract, multiply and modulus operations,
 * as well as pulling out coefficients by [] operators.
 */
class Polynomial{
public:
    Polynomial(int value = 0, int p = 2, int n = 8);
    Polynomial(const vector<Modular<int>> & a, int p = 2);
    Polynomial(const Polynomial & other);
    
    // Add two polynomials
    Polynomial & operator+=(const Polynomial & other);
    // Subtract two polynomials
    Polynomial & operator-=(const Polynomial & other);
    // Multiply two polynomials (results in higher degree n)
    Polynomial & operator*=(const Polynomial & other);
    // Take the modulus of two polynomials (results in lower degree n)
    Polynomial & operator%=(const Polynomial & other);
    // Divide two polynomials (results in lower degree n)
    Polynomial & operator/=(const Polynomial & other);
    
    // Add two polynomials
    Polynomial operator+(const Polynomial & other) const;
    // Subtract two polynomials
    Polynomial operator-(const Polynomial & other) const;
    // Multiply two polynomials (results in higher degree n)
    Polynomial operator*(const Polynomial & other) const;
    // Take the modulus of two polynomials (results in lower degree n)
    Polynomial operator%(const Polynomial & other) const;
    // Divide two polynomials (results in lower degree n)
    Polynomial operator/(const Polynomial & other) const;
    
    // Grab a coefficient from the polynomial
    const Modular<int> & operator[](int i) const;
    
    // Returns degree of polynomial
    int size() const;
    
    // Returns the base 10 integer representation of the polynomial
    int toInt() const;
    // Returns detailed string representation of polynomial
    string toPoly() const;
    // Returns simple string representation of polynomial
    string toString() const;
    
    int getPrime() const;
    
private:
    void reduce();
    
    vector<Modular<int>> _a;
    int _p;
};

/*
 * GaloisPolynomial
 * Represents a polynomial in a galois field. Does addition, subtraction
 * and multiplication mod the modulus polynomial, keeping the degree of
 * the polynomial <= degree of the galois field.  Also allows for
 * calculating the multiplicative inverse mod the modulus polynomial.
 */

class GaloisPolynomial{
public:
    // Sets up the galois field for the polynomials
    static void globalSetModulus(const Polynomial & modulus);
    
    GaloisPolynomial(int value = 0, int p = 2, int n = 8);
    GaloisPolynomial(const vector<Modular<int>> & v, int p = 2);
    GaloisPolynomial(const Polynomial & p);
    
    // Add two polynomials
    GaloisPolynomial & operator+=(const GaloisPolynomial & other);
    // Subtract two polynomials
    GaloisPolynomial & operator-=(const GaloisPolynomial & other);
    // Multiply two polynomials mod _modulus
    GaloisPolynomial & operator*=(const GaloisPolynomial & other);
    // Divide two polynomials mod _modulus
    GaloisPolynomial & operator/=(const GaloisPolynomial & other);
    
    // Add two polynomials
    GaloisPolynomial operator+(const GaloisPolynomial & other) const;
    // Subtract two polynomials
    GaloisPolynomial operator-(const GaloisPolynomial & other) const;
    // Multiply two polynomials mod _modulus
    GaloisPolynomial operator*(const GaloisPolynomial & other) const;
    // Divide two polynomials mod _modulus
    GaloisPolynomial operator/(const GaloisPolynomial & other) const;
    
    // Find the multiplicative inverse
    GaloisPolynomial inverse() const;
    
    // Grab a coefficient from the polynomial
    const Modular<int> operator[](int i) const;
    
    // Get degree of polynomial
    int size() const;
    
    // Returns the base 10 integer representation of the polynomial
    int toInt() const;
    // Returns detailed string representation of polynomial
    string toPoly() const;
    // Returns simple string representation of polynomial
    string toString() const;
    
private:
    Polynomial _polynomial;
    static Polynomial _modulus;
};

#endif
