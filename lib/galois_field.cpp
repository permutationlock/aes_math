/*
 * galois_field.cpp
 * Author: Aven Bross
 * Date: 2/20/2015
 * 
 * Implementation of galois field, allows construction of
 * objects in a galois field, addition, multiplication
 * and extended euclidean algorithm for finding inverses.
 */

#ifndef GALOIS_FIELD_CPP
#define GALOIS_FIELD_CPP

#include "galois_field.h"
 
/*
 * Polynomial
 * Represents a polynomial of the form a_n*x^n+...+a_1*x+a_0
 * Allows for add, subtract, multiply and modulus operations,
 * as well as pulling out coefficients by [] operators.
 */
 
 
Polynomial::Polynomial(int value, int p, int n): _p(p) {
    for(int i=0; i<n; i++){
        _a.push_back(0);
    }
    
    for(int i=n-1; i>=0; i--){
        int num = pow(_p, i);
        if(value >= num){
            int m = (value/num)%_p;
            _a[i] = Modular<int>(m);
            value -= m * num;
        }
        if(value==0) break;
    }
    
    reduce();
}

Polynomial::Polynomial(const vector<Modular<int>> & a, int p): _p(p) {
    for(int i=0; i<a.size(); i++){
        _a.push_back(a[i]);
    }
    reduce();
}


Polynomial::Polynomial(const Polynomial & other){
    _p = other._p;
    
    for(int i=0; i<other.size(); i++){
        _a.push_back(other[i]);
    }
    
    reduce();
}

// Adds a polynomial to this one modularly
Polynomial & Polynomial::operator+=(const Polynomial & other){
    if(_p != other._p) throw runtime_error("Mismatched prime.");
    
    while(_a.size()<other.size())
        _a.push_back(0);
    
    Modular<int>::globalSetModulus(_p);
    
    for(int i=0; i<other.size(); i++){
        _a[i] += other[i];
    }
    
    reduce();
    
    return *this;
}

// Subtracts a polynomial from this one modularly via additive inverses
Polynomial & Polynomial::operator-=(const Polynomial & other){
    if(_p != other._p) throw runtime_error("Mismatched prime.");
    
    while(_a.size()<other.size())
        _a.push_back(0);
        
    
    Modular<int>::globalSetModulus(_p);
    for(int i=0; i<other.size(); i++){
        _a[i] -= other[i];
    }
    
    reduce();
    
    return *this;
}

// Multiply two polynomials (results in higher degree n)
Polynomial & Polynomial::operator*=(const Polynomial & other){
    if(_p != other._p) throw runtime_error("Mismatched prime.");
    
    vector<Modular<int>> a;
    
    for(int i=0; i<other.size()+this->size(); i++){
        a.push_back(0);
    }
    
    Modular<int>::globalSetModulus(_p);
    // FOIL multiplication
    for(int i=0; i<this->size(); i++){
        for(int j=0; j<other.size(); j++){
            a[i+j] += _a[i]*other[j];
        }
    }
    
    // Swap in the product coefficients
    swap(_a,a);
    
    reduce();
    
    return *this;
}

// Take the modulus of two polynomials (results in lower degree n)
Polynomial & Polynomial::operator%=(const Polynomial & other){
    if(_p != other._p) throw runtime_error("Mismatched prime.");
    
    Modular<int>::globalSetModulus(_p);
    while(this->size() >= other.size()){
        Modular<int> c =  _a.back().mulInverse() * other._a.back();
        int diff = this->size()-other.size();
        vector<Modular<int>> v;
        for(int i=0; i< diff; i++)
            v.push_back(0);
        v.push_back(c);
        Polynomial p(v,_p);
        (*this) -= (other * p);
    }
    
    return *this;
}

// Take the quotient of two polynomials (results in lower degree n)
Polynomial & Polynomial::operator/=(const Polynomial & other){
    if(_p != other._p) throw runtime_error("Mismatched prime.");
    
    Polynomial quot(0,_p,1);
    
    Modular<int>::globalSetModulus(_p);
    while(this->size() >= other.size()){
        Modular<int> c =  other._a.back() / _a.back();
        int diff = this->size()-other.size();
        vector<Modular<int>> v;
        for(int i=0; i< diff; i++)
            v.push_back(0);
        v.push_back(c);
        Polynomial p(v,_p);
        quot += p;
        (*this) -= (other * p);
    }
    
    swap(_a,quot._a);
    
    return *this;
}

// Add two polynomials
Polynomial Polynomial::operator+(const Polynomial & other) const{
    return Polynomial(*this) += other;
}

// Subtract two polynomials
Polynomial Polynomial::operator-(const Polynomial & other) const{
    return Polynomial(*this) -= other;
}

// Multiply two polynomials (results in higher degree n)
Polynomial Polynomial::operator*(const Polynomial & other) const{
    return Polynomial(*this) *= other;
}

// Take the modulus of two polynomials (results in lower degree n)
Polynomial Polynomial::operator%(const Polynomial & other) const{
    return Polynomial(*this) %= other;
}

// Take the modulus of two polynomials (results in lower degree n)
Polynomial Polynomial::operator/(const Polynomial & other) const{
    return Polynomial(*this) /= other;
}

// Grab a coefficient from the polynomial
const Modular<int> & Polynomial::operator[](int i) const{
    return _a[i];
}

// Returns the base 10 integer representation of the polynomial
int Polynomial::toInt() const{
    int sum = 0;
    for(int i=0; i<_a.size(); i++){
        sum += _a[i].value()*pow(_p,i);
    }
    return sum;
}

// Returns the prime of the polynomial
int Polynomial::getPrime() const{
    return _p;
}


// Returns detailed string representation of polynomial
string Polynomial::toPoly() const{
    
    string s = "";
    if(_a.size()==0) return s;
    if(_a[_a.size()-1].value()!=0){
        s += to_string(_a[_a.size()-1].value());
        s += "*";
        s += to_string(_p);
        s += "^";
        s += to_string(_a.size()-1);
    }
    for(int i=_a.size()-2; i>=0; i--){
        if(_a[i].value()!=0){
            s += "+";
            s += to_string(_a[i].value());
            s += "*";
            s += to_string(_p);
            s += "^";
            s += to_string(i);
        }
    }

    return s;
}

// Returns simple string representation of polynomial
string Polynomial::toString() const{
    string s = "";
    for(int i=0; i<_a.size(); i++){
        s.insert(0,to_string(_a[i].value()));
    }
    return s;
}

// Returns the degree of the polynomial
int Polynomial::size() const{
    return _a.size();
}

// Remove excess 0's
void Polynomial::reduce(){
    while(_a.size()>0 && _a.back()==0){
        _a.pop_back();
    }
}


/*
 * GaloisPolynomial
 * Represents a polynomial in a galois field. Does addition, subtraction
 * and multiplication mod the modulus polynomial, keeping the degree of
 * the polynomial <= degree of the galois field.  Also allows for
 * calculating the multiplicative inverse mod the modulus polynomial.
 */
 
Polynomial GaloisPolynomial::_modulus(vector<Modular<int>>{
    1, 0, 0, 0, 1, 1, 0, 1, 1
});



GaloisPolynomial::GaloisPolynomial(int value, int p, int n): _polynomial(value, p, n) {
    _polynomial %= _modulus;
}

GaloisPolynomial::GaloisPolynomial(const vector<Modular<int>> & v, int p): _polynomial(v, p) {
    _polynomial %= _modulus;
}

GaloisPolynomial::GaloisPolynomial(const Polynomial & p): _polynomial(p) {
    _polynomial %= _modulus;
}

// Add two polynomials
GaloisPolynomial & GaloisPolynomial::operator+=(const GaloisPolynomial & other){
    _polynomial += other._polynomial;
    return *this;
}

// Subtract two polynomials
GaloisPolynomial & GaloisPolynomial::operator-=(const GaloisPolynomial & other){
    _polynomial -= other._polynomial;
    return *this;
}

// Multiply two polynomials mod _modulus
GaloisPolynomial & GaloisPolynomial::operator*=(const GaloisPolynomial & other){
    _polynomial *= other._polynomial;
    _polynomial %= _modulus;
    return *this;
}

// Multiply two polynomials mod _modulus
GaloisPolynomial & GaloisPolynomial::operator/=(const GaloisPolynomial & other){
    _polynomial *= other.inverse()._polynomial;
    _polynomial %= _modulus;
    return *this;
}

// Add two polynomials
GaloisPolynomial GaloisPolynomial::operator+(const GaloisPolynomial & other) const{
    return GaloisPolynomial(_polynomial + other._polynomial);
}

// Subtract two polynomials
GaloisPolynomial GaloisPolynomial::operator-(const GaloisPolynomial & other) const{
    return GaloisPolynomial(_polynomial - other._polynomial);
}

// Multiply two polynomials mod _modulus
GaloisPolynomial GaloisPolynomial::operator*(const GaloisPolynomial & other) const{
    return GaloisPolynomial((_polynomial * other._polynomial) % _modulus);
}

// Multiply two polynomials mod _modulus
GaloisPolynomial GaloisPolynomial::operator/(const GaloisPolynomial & other) const{
    return GaloisPolynomial((_polynomial * other.inverse()._polynomial) % _modulus);
}

// Find multiplicative inverse mod _modulus
GaloisPolynomial GaloisPolynomial::inverse() const{
    // Return 0 on 0
    if(_polynomial.size()<1) return GaloisPolynomial(Polynomial(0,_polynomial.getPrime(),1));
    Polynomial p1 = _modulus;
    Polynomial p2 = _polynomial;
    vector<Polynomial> aux;
    aux.push_back(Polynomial(0,_polynomial.getPrime(),1));
    aux.push_back(Polynomial(1,_polynomial.getPrime(),1));
    
    // Divide until we hit 1
    while(p1.size()>1 && p2.size()>1){
        Polynomial quot = p1/p2;
        Polynomial rem = p1%p2;
        Polynomial a_n = aux.back();
        a_n*=quot;
        a_n+=aux[aux.size()-2];
        aux.push_back(a_n);
        p1 = p2;
        p2 = rem;
    }
    
    return GaloisPolynomial(aux.back());
}

// Grab a coefficient from the polynomial
const Modular<int> GaloisPolynomial::operator[](int i) const{
    if(i>=_polynomial.size()) return 0;
    return _polynomial[i];
}

// Get degree of polynomial
int GaloisPolynomial::size() const{
    return _polynomial.size();
}

// Returns detailed string representation of the polynomial
string GaloisPolynomial::toPoly() const{
    return _polynomial.toPoly();
}

// Returns simple string representation of the polynomial
string GaloisPolynomial::toString() const{
    string s = _polynomial.toString();
    for(int i=0; i<_modulus.size()-_polynomial.size()-1; i++)
        s.insert(0, "0");
    return s;
}

// Returns the base 10 integer representation of the polynomial
int GaloisPolynomial::toInt() const{
    return _polynomial.toInt();
}

// Sets up the galois field for the polynomials
void GaloisPolynomial::globalSetModulus(const Polynomial & modulus){
    _modulus = modulus;
}

#endif

