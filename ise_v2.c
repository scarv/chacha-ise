
#include "ise_v3.h"

#define ROL32(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROR32(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

uint64_t rv64_pack(uint64_t rs1, uint64_t rs2) {
    return ((rs1 << 32)>>32) | (rs2 << 32);
}

uint64_t rv64_packh(uint64_t rs1, uint64_t rs2) {
    return (rs1 >> 32) | ((rs2 >> 32)<<32);
}

// [high half of rs1, low half of rs2]
uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2) {
    return ((rs2 >> 32)<<32) | ((rs1 << 32) >> 32);
}


//
// Given rs1||rs2 = A,D|| B,C
// Compute the output values of A and D in the Quarter round
uint64_t chacha_ad(uint64_t rs1, uint64_t rs2) {
    uint32_t  a = rs1 >> 32;
    uint32_t  b = rs2 >> 32;
    uint32_t  d = rs1      ;
    uint32_t  c = rs2      ;
    uint32_t ia = a + b;
    uint32_t id = ROL32((ia^d),16);
    uint32_t ic = c +id;
    uint32_t ib = ROL32((ic^b),12);
    uint32_t na = ia+ib;
    uint32_t nd = ROL32((id^na),8);
    return ((uint64_t)na) << 32 | nd;
}

//
// Given rs1||rs2 = nA,nD|| B,C
// Compute the output values of B and C in the Quarter round
uint64_t chacha_bc(uint64_t rs1, uint64_t rs2) {
    uint32_t na = rs1 >> 32;
    uint32_t  b = rs2 >> 32;
    uint32_t nd = rs1      ;
    uint32_t  c = rs2      ;
    uint32_t id = ROL32(nd,24) ^ na;
    uint32_t ic = c +id;
    uint32_t ib = ROL32((b ^ic),12);
    uint32_t nc = ic+nd;
    uint32_t nb = ROL32((ib^nc),7);
    return ((uint64_t)nb) << 32 | nc;
}
