// Copyright (c) 2021, Dimitris Alexopoulos All rights reserved.

// libreusedc - A personal collection of commonly used functions in C
// reusedc.h - Main library header

// NOTE: To compile without the use of libgmp define NOGMP

#ifndef REUSEDC_H
#define REUSEDC_H

// Macro definitions

// Header includes
#ifndef NO_GMP
#include <gmp.h>
#endif

// Function prototypes

// Number handling functions (numbers.c)

// Returns the number of digits in a long
int get_length(long number);
// Returns the requested digit (starting from the left) of a long
int get_digit(long number, int digit);
// Converts a long to an array of its digits (only works for positive integers)
int *long_to_array(long number, int length);
// Converts an array of digits into a long
long array_to_long(const int array[], int length);
// Sums all the elements in an array of integers
long array_sum(const int array[], int length);
#ifndef NO_GMP
// Returns the number of digits in a long
unsigned long long mpz_get_length(const mpz_t number);
// Returns the requested digit (starting from the left) of a mpz_t
int mpz_get_digit(const mpz_t number, unsigned long long digit);
// Converts an array of digits into a mpz_t
void array_to_mpz(const int array[], unsigned long long length, mpz_t number);
#endif

#endif
