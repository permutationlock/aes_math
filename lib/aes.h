/*
 * aes.h
 * Author: Aven Bross
 * Date: 3/3/2015
 * 
 * Functions and constants to perform AES encryption
 * and decryption.
 */
 
#ifndef AES_H
#define AES_H

#include "galois_field.h"
#include "matrix.h"
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>

using std::string;
using std::pow;
using std::vector;

// Mod polynomial used in Rijndael field
extern const  Polynomial rijndael_Mod;
// Affine transformation A for S-Box
extern const vector<Modular<int>> rijndael_A;
// Affine transformation A for inverse S-Box
extern const vector<Modular<int>> rijndael_A_inverse;
// Polynomial b to add in S-Box
extern const GaloisPolynomial rijndael_b;
// Polynomial b to add in inverse S-Box
extern const GaloisPolynomial rijndael_b_inverse;
// Linear transformation M for mix columns
extern const QSMatrix<GaloisPolynomial> rijndael_M;
// Linear transformation M for inverse mix columns
extern const QSMatrix<GaloisPolynomial> rijndael_M_inverse;

// Perform p = A * p^(-1) + b
GaloisPolynomial & sBox(GaloisPolynomial & p);
// Perform p = (A_inverse * p + b_inverse)^(-1) (inverse S-Box)
GaloisPolynomial & sBox_inverse(GaloisPolynomial & p);

// Perform s(i,j) = A * s(i,j)^(-1) + b for all 0<=i,j<=3
QSMatrix<GaloisPolynomial> & subBytes(QSMatrix<GaloisPolynomial> & state);
// Perform s(i,j) = (A_inverse * p + b_inverse)^(-1) (inverse S-Box) for all 0<=i,j<=3
QSMatrix<GaloisPolynomial> & subBytes_inverse(QSMatrix<GaloisPolynomial> & state);
// Rotate each row right by its index
QSMatrix<GaloisPolynomial> & shiftRows(QSMatrix<GaloisPolynomial> & state);
// Rotate each row left by its index
QSMatrix<GaloisPolynomial> & shiftRows_inverse(QSMatrix<GaloisPolynomial> & state);
// Performs state = M * state
QSMatrix<GaloisPolynomial> & mixColumns(QSMatrix<GaloisPolynomial> & state);
// Performs state = M_inverse * state
QSMatrix<GaloisPolynomial> & mixColumns_inverse(QSMatrix<GaloisPolynomial> & state);
// Adds portion of the key into the computation
QSMatrix<GaloisPolynomial> & addRoundKey(QSMatrix<GaloisPolynomial> & state, const QSMatrix<GaloisPolynomial> & key);

// Takes a polynomial and returns a character
unsigned char polyToChar(const GaloisPolynomial & p);
// Takes out a byte of the key string and returns a polynomial
GaloisPolynomial extractPoly(vector<unsigned char> & key);

// Takes a 4 polynomial word and modifies it by Rijndael core operations
vector<GaloisPolynomial>& keyExpandCore(vector<GaloisPolynomial> & word, int iteration);
// Takes 16 byte (128 bit) key and returns rounds + 1 16 byte keys in matrix form
vector<QSMatrix<GaloisPolynomial>> expandKey(vector<unsigned char> key, int rounds);

// Does rounds of AES encryption on plaintext with key
string encrypt(string plaintext, string key, int rounds = 10);
// Undoes rounds of AES on ciphertext with key
string decrypt(string ciphertext, string key, int rounds = 10);

#endif
