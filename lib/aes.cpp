/*
 * aes.cpp
 * Author: Aven Bross
 * Date: 3/3/2015
 * 
 * Functions and constants to perform AES encryption
 * and decryption.
 */
 
#ifndef AES_CPP
#define AES_CPP

#include "aes.h"

// Mod polynomial used in Rijndael field
const Polynomial rijndael_Mod(vector<Modular<int>>{
    1, 0, 0, 0, 1, 1, 0, 1, 1
});

// Affine transformation A for S-Box
const vector<Modular<int>> rijndael_A{
    1, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 1, 1, 1, 1, 1
};

// Affine transformation A for inverse S-Box
const vector<Modular<int>> rijndael_A_inverse{
    0, 0, 1, 0, 0, 1, 0, 1,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 0, 1, 0, 0, 1, 0,
    0, 0, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 0,
    0, 1, 0, 0, 1, 0, 1, 0
};

// Polynomial b to add in S-Box
const GaloisPolynomial rijndael_b(vector<Modular<int>>{ 1, 1, 0, 0, 0, 1, 1, 0 });

// Polynomial b to add in inverse S-Box
const GaloisPolynomial rijndael_b_inverse(vector<Modular<int>>{ 1, 0, 1, 0, 0, 0, 0, 0 });

// Linear transformation M for mix columns
const QSMatrix<GaloisPolynomial> rijndael_M(4, 4, vector<GaloisPolynomial>{
    2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2
});

// Linear transformation M for inverse mix columns
const QSMatrix<GaloisPolynomial> rijndael_M_inverse(4, 4, vector<GaloisPolynomial>{
    14, 11, 13,  9,
     9, 14, 11, 13,
    13,  9, 14, 11,
    11, 13,  9, 14
});

// Perform p = A * p^(-1) + b
GaloisPolynomial & sBox(GaloisPolynomial & p){
    // Invert each element
    p = p.inverse();
    
    // Calculate affine transformation
    vector<Modular<int>> coef;
    for(int r=0; r<8; r++){
        Modular<int> sum(0);
        for(int c=0; c<8; c++){
            sum += p[c] * rijndael_A[r*8+c];
        }
        coef.push_back(sum);
    }
    p = GaloisPolynomial(coef);
    
    // Add polynomial b
    p += rijndael_b;
    
    return p;
}

// Perform p = (A_inverse * p + b_inverse)^(-1) (inverse S-Box)
GaloisPolynomial & sBox_inverse(GaloisPolynomial & p){
    // Calculate affine transformation
    vector<Modular<int>> coef;
    for(int r=0; r<8; r++){
        Modular<int> sum(0);
        for(int c=0; c<8; c++){
            sum += p[c] * rijndael_A_inverse[r*8+c];
        }
        coef.push_back(sum);
    }
    p = GaloisPolynomial(coef);
    
    // Add polynomial b inverse
    p += rijndael_b_inverse;
    
    // Invert each element
    p = p.inverse();
    
    return p;
}

// Perform s(i,j) = A * s(i,j)^(-1) + b for all 0<=i,j<=3
QSMatrix<GaloisPolynomial> & subBytes(QSMatrix<GaloisPolynomial> & state){
    Modular<int>::globalSetModulus(2);
    for(int i=0; i<state.getRows(); i++){
        for(int j=0; j<state.getCols(); j++){
            sBox(state(i,j));
        }
    }
    
    return state;
}

// Perform s(i,j) = (A_inverse * p + b_inverse)^(-1) (inverse S-Box) for all 0<=i,j<=3
QSMatrix<GaloisPolynomial> & subBytes_inverse(QSMatrix<GaloisPolynomial> & state){
    Modular<int>::globalSetModulus(2);
    for(int i=0; i<state.getRows(); i++){
        for(int j=0; j<state.getCols(); j++){
            sBox_inverse(state(i,j));
        }
    }
    
    return state;
}

// Rotate each row right by its index
QSMatrix<GaloisPolynomial> & shiftRows(QSMatrix<GaloisPolynomial> & state){
    for(int i=0; i<state.getRows(); i++){
        vector<GaloisPolynomial> temp;  // Save first as temp since it is overwritten
        for(int j=0; j<state.getCols(); j++){
            int c = (j+i)%state.getCols();
            temp.push_back(state(i,c));
        }
        for(int j=0; j<state.getCols(); j++){
            state(i,j) = temp[j];
        }
    }
    
    return state;
}

// Rotate each row left by its index
QSMatrix<GaloisPolynomial> & shiftRows_inverse(QSMatrix<GaloisPolynomial> & state){
    for(int i=0; i<state.getRows(); i++){
        vector<GaloisPolynomial> temp;  // Save first as temp since it is overwritten
        for(int j=0; j<state.getCols(); j++){
            int c = (j-i);
            if(c<0) c = state.getCols() + c;
            temp.push_back(state(i,c));
        }
        for(int j=0; j<state.getCols(); j++){
            state(i,j) = temp[j];
        }
    }
    
    return state;
}

// Performs state = M * state
QSMatrix<GaloisPolynomial> & mixColumns(QSMatrix<GaloisPolynomial> & state){
    state = rijndael_M * state;
    
    return state;
}

// Performs state = M_inverse * state
QSMatrix<GaloisPolynomial> & mixColumns_inverse(QSMatrix<GaloisPolynomial> & state){
    state = rijndael_M_inverse * state;
    
    return state;
}

// Adds portion of the key into the computation
QSMatrix<GaloisPolynomial> & addRoundKey(QSMatrix<GaloisPolynomial> & state, const QSMatrix<GaloisPolynomial> & key){
    state += key;
    
    return state;
}

// Takes a polynomial and returns a character
unsigned char polyToChar(const GaloisPolynomial & p){
    return (unsigned char) p.toInt();
}

// Takes out a byte of the string and returns a polynomial
GaloisPolynomial extractPoly(vector<unsigned char> & text){
    if(text.size() == 0) return GaloisPolynomial(0);
    int n = (int) text[0];
    text.erase(text.begin());
    return GaloisPolynomial(n);
}

// Takes a 4 polynomial word and modifies it by Rijndael core operations
vector<GaloisPolynomial>& keyExpandCore(vector<GaloisPolynomial> & word, int iteration){
    // Do a simple left rotate of bytes
    GaloisPolynomial temp(word[0]);
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
    
    // Apply S-Box to each byte
    for(int i=0; i<4; i++){
        sBox(word[i]);
    }
    
    // Apply Rcon operation (add 2^(iteration-1) to word[0])
    vector<Modular<int>> v;
    for(int i=0; i<iteration-2; i++){
        v.push_back(0);
    }
    v.push_back(1);
    word[0] += GaloisPolynomial(v);
    
    return word;
}

// Takes 16 byte (128 bit) key and returns rounds + 1 16 byte keys in matrix form
vector<QSMatrix<GaloisPolynomial>> expandKey(vector<unsigned char> key, int rounds){
    vector<GaloisPolynomial> keyBytes;
    keyBytes.reserve(16*(rounds+1));    // Reserve space so inserts are faster
    
    // Just copy in key for first 16 bytes
    for(int i=0; i<16; i++){
        keyBytes.push_back(extractPoly(key));
    }
    
    int iteration = 1;
    
    // Continue until we have a 128 bit key for each round + an initial key
    while(keyBytes.size()<16*(rounds+1)){
        for(int w=0; w<4; w++){
            // Copy last 4 bytes
            vector<GaloisPolynomial> word;
            for(int i=0; i<4; i++){
                word.push_back(keyBytes[keyBytes.size()-5+i]);
            }
            
            // Apply core operations if first word
            if(w==0) keyExpandCore(word, iteration);
            
            // Add each byte with that 16 bytes back
            for(int i=0; i<4; i++){
                word[i] += keyBytes[keyBytes.size()-16+i];
            }
            
            // Append
            keyBytes.insert(keyBytes.end(), word.begin(), word.end());
        }
        
        iteration += 1;
    }
    
    // Convert to vector of matrices
    vector<QSMatrix<GaloisPolynomial>> keyMatrices;
    
    for(int i=0; i<rounds+1; i++){
        vector<GaloisPolynomial> v;
        for(int b=0; b<16; b++){
            v.push_back(keyBytes[i*16+b]);
        }
        keyMatrices.push_back(QSMatrix<GaloisPolynomial>(4, 4, v));
    }
    
    return keyMatrices;
}

// Does rounds of AES encryption on plaintext with key
string encrypt(string plaintext, string key, int rounds){
    // Set up Galois Field
    GaloisPolynomial::globalSetModulus(rijndael_Mod);
    
    // Convert strings to unsigned characters
    vector<unsigned char> vplaintext(plaintext.begin(), plaintext.end());
    vector<unsigned char> vkey(key.begin(), key.end());
    
    // Expand key
    vector<QSMatrix<GaloisPolynomial>> keyMatrices = expandKey(vkey, rounds);
    
    // Convert plaintext to matrix representation
    vector<GaloisPolynomial> sVec;
    sVec.reserve(16);
    for(int i=0; i<16; i++){
        sVec.push_back(extractPoly(vplaintext));
    }
    QSMatrix<GaloisPolynomial> state(4, 4, sVec);
    
    // Perform rounds on plaintext
    addRoundKey(state, keyMatrices[0]);
    for(int i=1; i<rounds; i++){
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, keyMatrices[i]);
    }
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, keyMatrices[rounds]);
    
    // Convert back to string
    vector<unsigned char> vciphertext;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vciphertext.push_back(polyToChar(state(i,j)));
        }
    }
    string ciphertext(vciphertext.begin(), vciphertext.end());
    
    return ciphertext;
}

// Undoes rounds of AES on ciphertext with key
string decrypt(string ciphertext, string key, int rounds){
    // Set up Galois Field
    GaloisPolynomial::globalSetModulus(rijndael_Mod);
    
    // Convert strings to unsigned characters
    vector<unsigned char> vciphertext(ciphertext.begin(), ciphertext.end());
    vector<unsigned char> vkey(key.begin(), key.end());
    
    // Expand key
    vector<QSMatrix<GaloisPolynomial>> keyMatrices = expandKey(vkey, rounds);
    
    // Convert ciphertext to matrix representation
    vector<GaloisPolynomial> sVec;
    sVec.reserve(16);
    for(int i=0; i<16; i++){
        sVec.push_back(extractPoly(vciphertext));
    }
    QSMatrix<GaloisPolynomial> state(4, 4, sVec);
    
    // Perform rounds on ciphertext
    addRoundKey(state, keyMatrices[rounds]);
    shiftRows_inverse(state);
    subBytes_inverse(state);
    for(int i=rounds-1; i>0; i--){
        addRoundKey(state, keyMatrices[i]);
        mixColumns_inverse(state);
        shiftRows_inverse(state);
        subBytes_inverse(state);
    }
    addRoundKey(state, keyMatrices[0]);
    
    // Convert back to string
    vector<unsigned char> vplaintext;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            vplaintext.push_back(polyToChar(state(i,j)));
        }
    }
    string plaintext(vplaintext.begin(), vplaintext.end());
    
    return plaintext;
}

#endif
