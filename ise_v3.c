
#include "ise.h"

//
// Given rs1||rs2 = AD,||B,C
// Compute the upper Quarter round output values of A and D
uint64_t chacha_ad0(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((na ^ d), 16);
    return ((uint64_t)na) << 32 | nd;
}

//
// Given rs1||rs2 = AD,||B,C
// Compute the upper Quarter round output values of B and C
uint64_t chacha_bc0(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs2 >> 32;
    uint32_t c = rs2      ;
    uint32_t d = rs1      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b),12);
    return ((uint64_t)nb) << 32 | nc;
}

//
// Given rs1||rs2 = AD,||B,C
// Compute the lower Quarter round output values of A and D
uint64_t chacha_ad1(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((na ^ d),  8);
    return ((uint64_t)na) << 32 | nd;
}

//
// Given rs1||rs2 = AD,||B,C
// Compute the lower Quarter round output values of B and C
uint64_t chacha_bc1(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs2 >> 32;
    uint32_t c = rs2      ;
    uint32_t d = rs1      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b), 7);
    return ((uint64_t)nb) << 32 | nc;
}

