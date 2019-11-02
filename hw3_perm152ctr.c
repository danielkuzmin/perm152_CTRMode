/*
 Daniel Kuzmin
 Class code: 1618
 October 8 2019
 Brief Description:
 This file contains the code for part B of the third assignment.
 This performs CTR mode using perm152 as a block cipher.
 Tested on Athena, compiled like so:
 clang -g -O0 -std=c99 -Werror -Wall -Wextra -Wconversion -fsanitize=address hw3_perm152ctr.c
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
// REMOVE THE BELOW INCLUDE BEFORE TURNING IN
#include "hw2_perm152.c"
// in and out are pointers to 64-byte buffers
// implementation is in a separate file
void perm152(unsigned char *in, unsigned char *out);

// in and out are pointers to 64-byte buffers
// key points to 0-to-64-byte buffer, kbytes indicates it's length
// Computes: (perm152(in xor key) xor key) and writes 64-bytes to out
//           key has (64 - kbytes) zero bytes appended to its end
static void perm152_bc(unsigned char *in, unsigned char *out, unsigned char *key, int kbytes)
{
    unsigned char perm_in[64];
    unsigned char perm_out[64];

    // copy in to perm_in
    memcpy(perm_in, in, 64);

    // xor key to perm_in
    for (int i = 0; i<kbytes; i++)
    {
        perm_in[i] = perm_in[i]^key[i];
    }

    // perm152 perm_in to perm_out
    perm152(perm_in, perm_out);

    // xor key to perm_out
    for (int i = 0; i<kbytes; i++)
    {
        perm_out[i] = perm_out[i]^key[i];
    }

    // copy perm_out to out
    memcpy(out, perm_out, 64);
}

// Increment function as given in the assignment PDF
static void increment(unsigned char *block) {
    int i = 63;
    do {
        //block[i]+= 1;
        block[i]++;
        i-=1;
    } while (block[i+1] == 0);
}

void perm152ctr(unsigned char *in,    // Input buffer
                unsigned char *out,   // Output buffer
                int nbytes,           // Number of bytes to process
                unsigned char *block, // A 64-byte buffer holding IV+CTR
                unsigned char *key,   // Key to use. 16-32 bytes recommended
                int kbytes)           // Number of key bytes <= 64 to use.
{
    unsigned char buf[64];
    while (nbytes > 0) {

        // perm152_bc block to buf
        perm152_bc(block, buf, key, kbytes);

        // len = min(nbytes, 64)
        //int len = (nbytes < 64 ? nbytes : 64);
        int len;
        if (nbytes < 64) {
            len = nbytes;
        }
        else {
            len = 64;
        }

        // Past this point is wrong
        // xor len bytes of buf with in to out

        for (int i = 0; i < len; i++) {
            out[i] = buf[i]^in[i];
        }
        //uint64_t temp = *buf ^ *in;
        //memcpy(out, &temp, (unsigned long) len);


        // increment block
        // call increment function
        increment(block);
        in = in + len;
        out = out + len;
        nbytes = nbytes - len;
    }
}