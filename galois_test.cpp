#include "lib/galois_field.h"
#include <iostream>

int main(){
    vector<Modular<int>> mod_vec;
    
    // Set up Rijndael Mod
    for(int i=0; i<9; i++){
        if(i==8 || i==4 || i==3 || i==1 || i==0) mod_vec.push_back(1);
        else mod_vec.push_back(0);
    }
    GaloisPolynomial::globalSetModulus(Polynomial(mod_vec));
    
    // Find multiplicative inverses
    std::cout<<"Finding and testing inverses of the poly representations of 1-15:\n";
    for(int i=0; i<16; i++){
        GaloisPolynomial p1(i, 2, 8);
        GaloisPolynomial p2 = p1.inverse();
        std::cout << "(" << p1.toPoly() << ")^(-1) = (" << p2.toPoly() << ")\n";
        std::cout << "(" << p1.toPoly() << ") * (" << p2.toPoly() << ") = (" << (p1*p2).toPoly() << ") \n\n";
    }
}
