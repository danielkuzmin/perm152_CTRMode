#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw3_perm152ctr.c"

void perm152ctr(unsigned char *in,    // Input buffer
                unsigned char *out,   // Output buffer
                int nbytes,           // Number of bytes to process
                unsigned char *block, // A 64-byte buffer holding IV+CTR
                unsigned char *key,   // Key to use. 16-32 bytes recommended
                int kbytes);          // Number of key bytes <= 64 to use.

// Poly hash using modulus 2^19 - 1. Each two-bytes (16-bits) is an element.
// Doesn't handle odd length inputs. Not endian-interoperable.
uint64_t poly_hash(uint16_t *m, int nbytes, uint32_t key) {
    uint64_t acc = key;   // (0 + 1) * key.  Implicit "1" element handles length
    while (nbytes >= 2) {
        acc = acc + *m;
        acc = acc * key;
        acc = (acc >> 19) + (acc & 0x7FFFF);  // Divisionless mod
        m += 1;
        nbytes -= 2;
    }
    return acc % 0x7FFFF;                     // Real mod
}

int main() {
    int len[] = {0,2,62,64,66,126,128,130,1000000};
    int res[] = {0x1FFFF,0x7B76,0x757C2,0x3FBDB,0x1A979,0x7A039,0x10509,0x7EDAF,0x6303};
    unsigned char block[64], block_orig[64] = {0};
    unsigned char key[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    unsigned char *pt = malloc(1000000); // 1 million bytes
    unsigned char *ct = malloc(1000000); // 1 million bytes

    // Initialize
    block_orig[0] = block_orig[63] = block_orig[62] = block_orig[61] = 255;
    for (int i=0; i<1000000; i++)
        pt[i] = (unsigned char)((i+1)*(i+3)%251);

    for (unsigned i=0; i<sizeof(len)/sizeof(len[0]); i++) {
        memcpy(block, block_orig, 64);
        // encrypt last len[i] bytes of pt buffer into ct buffer to catch bad mem accesses
        perm152ctr(pt+1000000-len[i], ct+1000000-len[i], len[i], block, key, 16);
        //printf("0x%X\n", poly_hash((uint16_t *)(ct+1000000-len[i]), len[i], 0x1FFFFu));
        // Poly hash the output to see if it matches expected results
        if (poly_hash((uint16_t *)(ct+1000000-len[i]), len[i], 0x1FFFFu) != (unsigned long)res[i]) {
            printf("No Pass");
            free(ct);
            free(pt);
            return 0;
        }
    }

    printf("Pass\n");
    free(ct);
    free(pt);
    return 0;
}