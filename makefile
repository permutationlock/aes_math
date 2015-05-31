all: aes_test

aes_test: galois_field.o matrix.o modular_arithmetic.o aes.o aes_test.o
	g++ galois_field.o matrix.o modular_arithmetic.o aes.o aes_test.o -std=c++11 -o aes_test

aes_test.o: aes_test.cpp
	g++ -c aes_test.cpp -std=c++11

aes.o: lib/aes.cpp
	g++ -c lib/aes.cpp -std=c++11

modular_arithmetic.o: lib/modular_arithmetic.cpp
	g++ -c lib/modular_arithmetic.cpp -std=c++11
    
matrix.o: lib/matrix.cpp
	g++ -c lib/matrix.cpp -std=c++11
    
galois_field.o: lib/galois_field.cpp
	g++ -c lib/galois_field.cpp -std=c++11

clean:
	rm *.o aes_test

