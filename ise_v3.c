
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
// Given rs1||rs2 = A,C||B,D
// Compute the intermediate values of B and D in the Quarter round
uint64_t chacha_bd(uint64_t rs1, uint64_t rs2) {
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
uint64_t chacha_ad(uint64_t rs1, uint64_t rs2) {
    uint32_t  a = rs1 >> 32;
    uint32_t ib = rs2 >> 32;
    uint32_t  d = rs1      ;
    uint32_t id = rs2      ;
    uint32_t na = ib + (ROL32(id,16)^d);
    uint32_t nd = ROL32((id^na),8);
    return ((uint64_t)na) << 32 | nd;
}

//
// Given rs1||rs2 = B,C||nA,nD
// Compute the output values of B and C in the Quarter round
uint64_t chacha_bc(uint64_t rs1, uint64_t rs2) {
    uint32_t  b = rs1 >> 32;
    uint32_t na = rs2 >> 32;
    uint32_t  c = rs1      ;
    uint32_t nd = rs2      ;
    uint32_t  t = c +(ROL32(nd,24)^na);
    uint32_t nc = t + nd;
    uint32_t nb = ROL32((nc ^ ROL32((b^t),12)),7);
    return ((uint64_t)nb) << 32 | nc;
}
