
#include "ise_v4.h"

#define ROL32(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROR32(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

uint64_t rv64_pack(uint64_t rs1, uint64_t rs2) {
    return ((rs1 << 32)>>32) | (rs2 << 32);
}

uint64_t rv64_packh(uint64_t rs1, uint64_t rs2) {
    return (rs1 >> 32) | ((rs2 >> 32)<<32);
}

// [high half of rs2, low half of rs1]
uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2) {
    return ((rs2 >> 32)<<32) | ((rs1 << 32) >> 32);
}

// [low half of rs2, high half of rs1]
uint64_t rv64_packlh(uint64_t rs1, uint64_t rs2) {
    return (rs2<<32) | (rs1 >> 32);
}

//
// Given rs1||rs2 = A[i],A[i+1]||B[i],B[i+1]
// Compute A[i]=A[i] + B[i] and A[i+1]=A[i+1] + B[i+1]
uint64_t chacha_add(uint64_t rs1, uint64_t rs2) {
    uint32_t  a1 = rs1 >> 32;
    uint32_t  a0 = rs1      ;
    uint32_t  b1 = rs2 >> 32;
    uint32_t  b0 = rs2      ;
    uint32_t na0 = a0+b0;
    uint32_t na1 = a1+b1;
    return ((uint64_t)na1) << 32 | na0;
}

//
// Given rs1||rs2 = D[i],D[i+1]||A[i],A[i+1]
// Compute D[i]= (D[i]^A[i])<<<ri and D[i+1]=(A[i+1]^D[i+1])<<<ri
uint64_t chacha_xorroli(uint64_t rs1, uint64_t rs2,uint8_t ri) {
    uint32_t  d1 = rs1 >> 32;
    uint32_t  d0 = rs1      ;
    uint32_t  a1 = rs2 >> 32;
    uint32_t  a0 = rs2      ;
    uint32_t nd0 = ROL32((d0^a0),ri);
    uint32_t nd1 = ROL32((d1^a1),ri);
    return ((uint64_t)nd1) << 32 | nd0;
}


