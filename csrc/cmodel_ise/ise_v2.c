
#include "ise_v2.h"

//
// Given rs1||rs2 = A,C||B,D
// Compute the intermediate values of B and D in the Quarter round
uint64_t chacha_bd_v2(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t c = rs2      ;
    uint32_t id = ROL32(((a+b )^d),16);
    uint32_t ib = ROL32(((c+id)^b),12);
    return ((uint64_t)ib) << 32 | id;
}

//
// Given rs1||rs2 = A,D||iB,iD
// Compute the output values of A and D in the Quarter round
uint64_t chacha_ad_v2(uint64_t rs1, uint64_t rs2) {
    uint32_t  a = rs1 >> 32;
    uint32_t ib = rs2 >> 32;
    uint32_t  d = rs1      ;
    uint32_t id = rs2      ;
    uint32_t na = ib + (d^ROL32(id,16));
    uint32_t nd = ROL32((id^na),8);
    return ((uint64_t)na) << 32 | nd;
}

//
// Given rs1||rs2 = nA,nD||B,C
// Compute the output values of B and C in the Quarter round
uint64_t chacha_bc_v2(uint64_t rs1, uint64_t rs2) {
    uint32_t na = rs1 >> 32;
    uint32_t  b = rs2 >> 32;
    uint32_t nd = rs1      ;
    uint32_t  c = rs2      ;

    uint32_t  t = c +(ROL32(nd,24)^na);
    uint32_t nc = t + nd;
    uint32_t nb = ROL32((nc ^ ROL32((b^t),12)),7);
    return ((uint64_t)nb) << 32 | nc;
}
