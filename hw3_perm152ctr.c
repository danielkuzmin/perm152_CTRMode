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
    kbytes = 0;
    if (kbytes)
        printf("you shouldn't be seeing this");

    unsigned char perm_in[64];
    unsigned char perm_out[64];

    // copy in to perm_in
    memcpy(perm_in, in, 64);

    // xor key to perm_in
    uint64_t temp = *perm_in ^ *key;
    memcpy(perm_in, &temp, 64);

    // perm152 perm_in to perm_out
    perm152(perm_in, perm_out);

    // xor key to perm_out
    uint64_t temp2 = *perm_out ^ *key;
    memcpy(perm_out, &temp2, 64);

    // copy perm_out to out
    memcpy(out, perm_out, 64);
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
        perm152_bc(block, block, key, kbytes);
        memcpy(buf, block, 64);

        // len = min(nbytes, 64)
        int len = nbytes;
        if (64 < nbytes)
            len = 64;

        // xor len bytes of buf with in to out
        uint64_t temp = *buf ^ *in;
        memcpy(out, &temp, (unsigned long) len);

        // increment block
        in = in + len;
        out = out + len;
        nbytes = nbytes - len;
    }
}