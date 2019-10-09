/*
 Daniel Kuzmin
 Class code: 1618
 September 24 2019
 Brief Description:
 This file contains the code for part A of the second assignment.
 This encrypts an array of 64 bits with the given algorithm.
 from the assignment.
 Tested on Athena, compiled like so:
 clang -g -O0 -std=c99 -Werror -Wall -Wextra -Wconversion -fsanitize=address hw1_perm152.c
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// This function takes a uint32_t and unsigned int i and rotates it left by i bits
static uint32_t rotl(uint32_t x, unsigned int i) {
    return (x << i) | (x >> (32-i));
}

// The mixing function as specified by the assignment
static void update(uint32_t *w, uint32_t *x, uint32_t *y, uint32_t *z) {
    *w = *w + *x; *z = *z ^ *w; *z = rotl(*z, 16);
    *y = *y + *z; *x = *x ^ *y; *x = rotl(*x, 12);
    *w = *w + *x; *z = *z ^ *w; *z = rotl(*z, 8);
    *y = *y + *z; *x = *x ^ *y; *x = rotl(*x, 7);
}

// Takes a pointer to an array in, copies it to a temp array, modifies it, then writes to out
void perm152(unsigned char in[64], unsigned char out[64]) {
    uint32_t a[16];

    memcpy(a, in, 64);
    for (int i = 0; i<10; i++) {
        update(&a[0], &a[4], &a[8], &a[12]);
        update(&a[1], &a[5], &a[9], &a[13]);
        update(&a[2], &a[6], &a[10], &a[14]);
        update(&a[3], &a[7], &a[11], &a[15]);

        update(&a[0], &a[5], &a[10], &a[15]);
        update(&a[1], &a[6], &a[11], &a[12]);
        update(&a[2], &a[7], &a[8], &a[13]);
        update(&a[3], &a[4], &a[9], &a[14]);
    }
    memcpy (out, a, 64);

}