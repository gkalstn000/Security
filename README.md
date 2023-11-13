# Mini RSA Implementation in C
## Overview
This repository contains a C implementation of a mini RSA algorithm, a widely-known asymmetric cryptographic technique. This implementation is designed for educational purposes, providing a basic understanding of RSA mechanics.

## Features
* RSA Key Generation: Efficiently generates RSA keys (p, q, e, d, n).
* Encryption and Decryption: Implements RSA encryption and decryption using generated keys.
* Modular Arithmetic Operations: Includes custom functions for modular addition, multiplication, exponentiation, and inversion.
* Prime Number Verification: Utilizes the Miller-Rabin primality test for verifying prime numbers.
* Extended Euclidean Algorithm: Implements the algorithm for finding modular inverses.
## Prerequisites
* A C compiler (e.g., GCC, Clang)
* Basic understanding of RSA algorithm and C programming
## Compilation and Execution
1. Clone this repository.
2. Compile the code using a C compiler. Example: gcc rsa.c -o rsa
3. Run the executable. Example: ./rsa
## Functions Overview
* Mod: Performs modular reduction.
* ModAdd: Executes modular addition or subtraction.
* ModMul: Carries out modular multiplication, considering overflow conditions.
* div_: Implements division without using the division operator.
* ModPow: Conducts modular exponentiation using the "square and multiply" method.
* IsPrime: Determines if a number is prime using Miller-Rabin test.
* Miller: The Miller-Rabin primality test logic.
* ModInv: Calculates modular inverse using the Extended Euclidean Algorithm.
* miniRSAKeygen: Generates RSA keys.
* miniRSA: Encrypts and decrypts data using RSA.
* GCD: Computes the greatest common divisor.
## Usage Example
1. Generate RSA keys using miniRSAKeygen.
2. Encrypt a message using miniRSA with the public key.
3. Decrypt the message using miniRSA with the private key.
## Disclaimer
This implementation is for educational purposes and might not be suitable for production use due to potential security vulnerabilities.

## Author
Min Su Ha
