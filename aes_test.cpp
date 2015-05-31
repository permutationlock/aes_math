/*
 * aes_test.cpp
 * Author: Aven Bross
 * Date: 3/4/2015
 * 
 * Testing AES functionality
 */
 
#include "lib/aes.h"
#include <iostream>

using std::cout;

int main(){
    GaloisPolynomial::globalSetModulus(rijndael_Mod);
    string plaintext = "0123456789abcdef";
    string key = "ohnoammyitisnogo";
    string ciphertext = encrypt(plaintext, key, 2);
    cout << ciphertext << "\n";
    string decrypted = decrypt(ciphertext, key, 2);
    cout << plaintext << "\n";
}
