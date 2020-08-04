
#include "chacha20.h"

#define ROL32(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROR32(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

uint64_t rv64_pack(uint64_t rs1, uint64_t rs2) {
    return ((rs1 << 32)>>32) | (rs2 << 32);
}

uint64_t rv64_packh(uint64_t rs1, uint64_t rs2) {
    return (rs1 >> 32) | ((rs2 >> 32)<<32);
}

#define UNPACK_ABCD(rs1,rs2) \
    uint32_t a = rs1 >> 32;  \
    uint32_t d = rs1      ;  \
    uint32_t b = rs2 >> 32;  \
    uint32_t c = rs2      ;  \

uint64_t chacha_i1_hi (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t rd_0 = c;
    uint64_t rd_1 = ROL32((a+b) ^ d, 16);
    return (rd_1 << 32) | rd_0;
}

uint64_t chacha_i1_lo (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t rd_0 = c;
    uint64_t rd_1 = ROL32((a+b) ^ d,  8);
    return (rd_1 << 32) | rd_0;
}

uint64_t chacha_i2_hi (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t rd_0 = c + d;
    uint64_t rd_1 = ROL32((c+d) ^ b, 12);
    return (rd_1 << 32) | rd_0;
}

uint64_t chacha_i2_lo (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t rd_0 = c + d;
    uint64_t rd_1 = ROL32((c+d) ^ b,  7);
    return (rd_1 << 32) | rd_0;
}

uint64_t chacha_i3_hi (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t rd_0 = a + (ROR32(b,12) ^ c);
    uint64_t rd_1 = d;
    return (rd_1 << 32) | rd_0;
}

uint64_t chacha_i3_lo (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint64_t rd_0 = a + (ROR32(b, 7) ^ c);
    uint64_t rd_1 = d;
    return (rd_1 << 32) | rd_0;
}

void chacha20_qr(uint32_t g[4]) {
    uint64_t tmp;
    uint32_t a = g[0];
    uint32_t b = g[1];
    uint32_t c = g[2];
    uint32_t d = g[3];

    tmp = chacha_i1_hi(a,b,c,d);
    c = tmp;
    d = tmp >> 32;
    
    tmp = chacha_i2_hi(a,b,c,d);
    c = tmp;
    b = tmp >> 32;
    
    tmp = chacha_i3_hi(a,b,c,d);
    a = tmp;
    d = tmp >> 32;
    
    tmp = chacha_i1_lo(a,b,c,d);
    c = tmp;
    d = tmp >> 32;
    
    tmp = chacha_i2_lo(a,b,c,d);
    c = tmp;
    b = tmp >> 32;
    
    tmp = chacha_i3_lo(a,b,c,d);
    a = tmp;
    d = tmp >> 32;

    g[0] = a;
    g[1] = b;
    g[2] = c;
    g[3] = d;
}


//! A vanilla implementation of the ChaCha20 block function.
void chacha20_block(uint32_t out[16], uint32_t const in[16])
{
    uint64_t * pin  = (uint64_t *)in;
    uint64_t * pout = (uint64_t *)out;

    uint64_t a0 = pin[0];  //  1,  0
    uint64_t a1 = pin[1];  //  3,  2
    uint64_t a2 = pin[2];  //  5,  4
    uint64_t a3 = pin[3];  //  7,  6
    uint64_t a4 = pin[4];  //  9,  8
    uint64_t a5 = pin[5];  // 11, 10
    uint64_t a6 = pin[6];  // 13, 12
    uint64_t a7 = pin[7];  // 15, 14

    uint64_t t0 = rv64_pack (a2, a0); //  0,  4  Column 0
    uint64_t t1 = rv64_pack (a6, a4); //  8, 12
    uint64_t t2 = rv64_packh(a2, a0); //  1,  5  Column 1
    uint64_t t3 = rv64_packh(a6, a4); //  9, 13
    uint64_t t4 = rv64_pack (a3, a1); //  2,  6  Column 2
    uint64_t t5 = rv64_pack (a7, a5); // 10, 14
    uint64_t t6 = rv64_packh(a3, a1); //  3,  7  Column 3
    uint64_t t7 = rv64_packh(a7, a5); // 11, 15
}

