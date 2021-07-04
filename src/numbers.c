// Copyright (c) 2021, Dimitris Alexopoulos All rights reserved.

// libreusedc - A personal collection of commonly used functions in C
// numbers.c - Useful functions for handling numbers

// NOTE: To compile without the use of libgmp define NOGMP

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Optional build for GMP functions
#ifndef NOGMP
#include <gmp.h>
#endif

// Returns the number of digits in a long
int get_length(long input_number)
{
    int length = 0;
    input_number = labs(input_number);
    while (input_number != 0)
    {
        input_number = input_number / 10;
        length++;
    }
    return length;
}

// Returns the requested digit (starting from the left) of a long
int get_digit(long input_number, int digit_requested)
{
    int digits_cleared = 0;
    int length = get_length(input_number);
    input_number = labs(input_number);
    assert(digit_requested <= length && digit_requested > 0);
    while (digits_cleared < (length - digit_requested)) //removes digits from the back of a number
    {
        input_number -= input_number % 10;
        input_number /= 10;
        digits_cleared++;
    }
    return (input_number % 10); //returns the last digit of a number (the digit requested)
}

// Converts a long to an array of its digits (only works for positive integers)
int *long_to_array(long input_number, int length)
{
    assert(input_number >= 0);
    int *digit_array;
    do
    {
        digit_array = malloc(sizeof(int) * length);
    } while (digit_array == NULL);

    for (int i = 1; i <= length; i++)
    {
        digit_array[length - i] = input_number % 10;
        input_number /= 10;
    }
    return digit_array;
}

// Converts an array of digits into a long
long array_to_long(const int array[], int length)
{
    long output_number = 0;
    for (int i = 0; i < length; i++)
    {
        output_number += array[i] * pow(10, length - i - 1);
    }
    return output_number;
}

long array_sum(const int array[], int length)
{
    long sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum += array[i];
    }
    return sum;
}

// Optional GMP functions
#ifndef NOGMP
// Returns the number of digits in a long
unsigned long long mpz_get_length(const mpz_t input_number)
{
    unsigned long long length = 0;
    mpz_t number;
    mpz_t tmp;
    mpz_init(number);
    mpz_init(tmp);
    mpz_abs(number, input_number);
    while (mpz_sgn(number) != 0)
    {
        mpz_set(tmp, number);
        mpz_tdiv_q_ui(number, tmp, 10);
        length++;
    }
    return length;
}

// Returns the requested digit (starting from the left) of a mpz_t
int mpz_get_digit(const mpz_t input_number, unsigned long long digit_requested)
{
    mpz_t number;
    mpz_t tmp;
    mpz_init(number);
    mpz_init(tmp);
    unsigned long long digits_cleared = 0;
    unsigned long long length = mpz_get_length(input_number);
    mpz_abs(number, input_number);
    // assert(digit_requested <= length && digit_requested > 0); TODO: Fix assert or remove
    while (digits_cleared < (length - digit_requested)) //removes digits from the back of a number
    {
        mpz_tdiv_r_ui(tmp, number, 10);
        mpz_sub(number, number, tmp);
        mpz_tdiv_q_ui(number, number, 10);
        digits_cleared++;
    }
    int return_digit = mpz_get_si(number) % 10;
    mpz_clears(number, tmp, NULL);
    return return_digit; //returns the last digit of a number (the digit requested)
}

// Converts an array of digits into a mpz_t
void array_to_mpz(const int array[], unsigned long long length, mpz_t output_number)
{
    mpz_t tmp;
    mpz_t place_value;
    mpz_t raised_int; // TODO: Rename
    mpz_init(tmp);
    mpz_init(place_value);
    mpz_init(raised_int);
    mpz_set_ui(output_number, 0);
    for (unsigned long long i = 0; i < length; i++)
    {
        mpz_set(tmp, output_number);
        mpz_ui_pow_ui(place_value, 10, length - i - 1);
        mpz_mul_ui(raised_int, place_value, array[i]);
        mpz_add(output_number, tmp, raised_int);
    }
    mpz_clears(tmp, place_value, raised_int, NULL);
}
#endif

int main()
{
    printf("11234352 has %i digits\n", get_length(11234352));
    printf("The second digit of 5421 is %i\n", get_digit(5421, 2));
    long length1 = get_length(123456789);
    int *int_array1 = long_to_array(123456789, length1);
    printf("The digits of 123456789 are: ");
    for (int i = 0; i < length1 - 1; i++)
    {
        printf("%i, ", int_array1[i]);
    }
    printf("%i\n", int_array1[length1 - 1]);

    printf("The number with digits [1, 2, 3, 5, 6, 7, 8, 9] is: %li\n", array_to_long((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 9));

    printf("The sum of the digits of 123456789 is %li\n", array_sum((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 9));

// For optional GMP functions
#ifndef NOGMP
    mpz_t gmpint3;
    mpz_init(gmpint3);
    array_to_mpz((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 9, gmpint3);
    gmp_printf("The number with digits [1, 2, 3, 5, 6, 7, 8, 9] is: %Zd\n", gmpint3);
#endif
    printf("Beginning test suite...\n");

    assert(get_length(11234352) == 8);

    assert(get_digit(5421, 2) == 4);

    const int int_array2[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(memcmp(int_array1, int_array2, sizeof(int) * 9) == 0);

    assert(array_to_long(int_array2, 9) == 123456789);

    assert(array_sum((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 9) == 45);

#ifndef NOGMP
    mpz_t gmpint1;
    mpz_t gmpint2;
    mpz_init(gmpint1);
    mpz_init_set_ui(gmpint2, 123456789);
    array_to_mpz((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 9, gmpint1);
    assert(mpz_cmp(gmpint1, gmpint2) == 0);
    assert(mpz_get_digit(gmpint2, 3) == 3 && mpz_get_digit(gmpint2, 7) == 7);
#endif

    printf("Test suite finished successfully\n");
}
