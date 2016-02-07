
#include "xorshift.h"

// Implements the xorshift+ algorithm
// Reference: https://en.wikipedia.org/wiki/Xorshift

#include "stdint.h"

/* The state must be seeded so that it is not everywhere zero. */
uint64_t s[2];

uint64_t xorshiftplus_uniform(uint64_t limit) {
    uint64_t rnd = xorshiftplus();
    while (rnd > UINT64_MAX - (UINT64_MAX % limit)) {
        rnd = xorshiftplus();
    }
    return rnd % limit;
}

uint64_t xorshiftplus(void) {
    uint64_t x = s[0];
    uint64_t const y = s[1];
    s[0] = y;
    x ^= x << 23; // a
    s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
    return s[1] + y;
}

void seedxorshift(uint64_t s1, uint64_t s2) {
    s[0] = s1;
    s[1] = s2;
}