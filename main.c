#include <stdio.h>
#include "hw3_perm152ctr.c"

int main() {
    printf("Hello, World!\n");
    uint32_t in[64];
    uint32_t out[64];
    perm152ctr(in, out, 64, , ,16);
    return 0;
}