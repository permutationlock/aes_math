# Specify compiler
COMP = clang++ -std=c++1y -O2

# Specify target
all: aes_test

# Build executable
aes_test: galois_field.o matrix.o modular_arithmetic.o aes.o aes_test.o
	$(COMP) galois_field.o matrix.o modular_arithmetic.o aes.o aes_test.o -o aes_test

# Build test file object
aes_test.o: aes_test.cpp
	$(COMP) -c aes_test.cpp

# Build aes library object
aes.o: lib/aes.cpp
	$(COMP) -c lib/aes.cpp

# Build modulare arithmetic library object
modular_arithmetic.o: lib/modular_arithmetic.cpp
	$(COMP) -c lib/modular_arithmetic.cpp

# Build matrix library object
matrix.o: lib/matrix.cpp
	$(COMP) -c lib/matrix.cpp

# Build galois field library object
galois_field.o: lib/galois_field.cpp
	$(COMP) -c lib/galois_field.cpp

# Clean build
clean:
	rm *.o aes_test

