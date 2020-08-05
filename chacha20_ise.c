
#include "chacha20.h"

#define ROL32(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROR32(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

uint64_t rv64_pack(uint64_t rs1, uint64_t rs2) {
    return ((rs1 << 32)>>32) | (rs2 << 32);
}

uint64_t rv64_packh(uint64_t rs1, uint64_t rs2) {
    return (rs1 >> 32) | ((rs2 >> 32)<<32);
}

// [high half of rs1, low half of rs2]
uint64_t rv64_packm(uint64_t rs1, uint64_t rs2) {
    return ((rs1 >> 32)<<32) | ((rs2 << 32) >> 32);
}

uint64_t chacha20_ad0(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((d ^ na), 16);
    return ((uint64_t)na) << 32 | nd;
}

uint64_t chacha20_bc0(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs2 >> 32;
    uint32_t c = rs2      ;
    uint32_t d = rs1      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b),12);
    return ((uint64_t)nb) << 32 | nc;
}

uint64_t chacha20_ad1(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((d ^ na),  8);
    return ((uint64_t)na) << 32 | nd;
}

uint64_t chacha20_bc1(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs2 >> 32;
    uint32_t c = rs2      ;
    uint32_t d = rs1      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b), 7);
    return ((uint64_t)nb) << 32 | nc;
}

#define FQR(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs1= chacha20_ad0(in_rs1 ,in_rs2); \
    out_rs2= chacha20_bc0(out_rs1,in_rs2); \
    out_rs1= chacha20_ad1(out_rs1,out_rs2); \
    out_rs2= chacha20_bc1(out_rs1,out_rs2); \
}


void chacha20_qr(uint32_t g[4]) {
    uint64_t tmp;
    uint32_t a = g[0];
    uint32_t b = g[1];
    uint32_t c = g[2];
    uint32_t d = g[3];

    uint64_t ad = rv64_pack(d,a); 
    uint64_t bc = rv64_pack(c,b); 

    FQR(ad, bc, ad,bc)

    a = ad >> 32;
    b = bc >> 32;
    c = bc;
    d = ad;

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

    uint64_t t0 = rv64_pack (a6, a0); //  0, 12  Column 0
    uint64_t t1 = rv64_pack (a4, a2); //  4,  8
    uint64_t t2 = rv64_packh(a6, a0); //  1, 13  Column 1
    uint64_t t3 = rv64_packh(a4, a2); //  5,  9
    uint64_t t4 = rv64_pack (a7, a1); //  2, 14  Column 2
    uint64_t t5 = rv64_pack (a5, a3); //  6, 10
    uint64_t t6 = rv64_packh(a7, a1); //  3, 15  Column 3
    uint64_t t7 = rv64_packh(a5, a3); //  7, 11

    for(int i = 0; i < CHACHA20_ROUNDS; i += 2)
    {
        printf("--- %d \n", i);
        printf("%016llX %016llX\n", t0, t1);
        printf("%016llX %016llX\n", t2, t3);
        printf("%016llX %016llX\n", t4, t5);
        printf("%016llX %016llX\n", t6, t7);

        //  AD  BC, AD  BC         A     B     C      D
        FQR(a0, a1, t0, t1); // QR(x[0], x[4], x[ 8], x[12]) - column 0
        FQR(a2, a3, t2, t3); // QR(x[1], x[5], x[ 9], x[13]) - column 1
        FQR(a4, a5, t4, t5); // QR(x[2], x[6], x[10], x[14]) - column 2
        FQR(a6, a7, t6, t7); // QR(x[3], x[7], x[11], x[15]) - column 3
        
        printf("---\n");
        printf("%016llX %016llX\n", a0, a1);
        printf("%016llX %016llX\n", a2, a3);
        printf("%016llX %016llX\n", a4, a5);
        printf("%016llX %016llX\n", a6, a7);
        
        //              A    D        B     C
        // t0 || t1 = x[0], x[12] || x[4], x[ 8]
        // t2 || t3 = x[1], x[13] || x[5], x[ 9]
        // t4 || t5 = x[2], x[14] || x[6], x[10]
        // t6 || t7 = x[3], x[15] || x[7], x[11]

        t0 = rv64_packm(a0, a6); //  0, 15 | A, D   |  Column 0
        t1 = rv64_packm(a3, a5); //  5, 10 | B, C   |
        t2 = rv64_packm(a2, a0); //  1, 12 |        |  Column 1
        t3 = rv64_packm(a5, a7); //  6, 11 |        |
        t4 = rv64_packm(a4, a2); //  2, 13 |        |  Column 2
        t5 = rv64_packm(a7, a1); //  7,  8 |        |
        t6 = rv64_packm(a6, a4); //  3, 14 |        |  Column 3
        t7 = rv64_packm(a1, a3); //  4,  9 |        |
        
        printf("---\n");
        printf("%016llX %016llX\n", t0, t1);
        printf("%016llX %016llX\n", t2, t3);
        printf("%016llX %016llX\n", t4, t5);
        printf("%016llX %016llX\n", t6, t7);
        
        //  AD  BC, AD, BC          A     B     C      D
        FQR(a0, a1, t0, t1); // QR(x[0], x[5], x[10], x[15]) - diagonal 0
        FQR(a2, a3, t2, t3); // QR(x[1], x[6], x[11], x[12]) - diagonal 1
        FQR(a4, a5, t4, t5); // QR(x[2], x[7], x[ 8], x[13]) - diagonal 2
        FQR(a6, a7, t6, t7); // QR(x[3], x[4], x[ 9], x[14]) - diagonal 3
        
        printf("---\n");
        printf("%016llX %016llX\n", a0, a1);
        printf("%016llX %016llX\n", a2, a3);
        printf("%016llX %016llX\n", a4, a5);
        printf("%016llX %016llX\n", a6, a7);
        
        //              A    D        B      C   
        // a0 || a1 = x[0], x[15] || x[ 5], x[10]
        // a2 || a3 = x[1], x[12] || x[ 6], x[11]
        // a4 || a5 = x[2], x[13] || x[ 7], x[ 8]
        // a6 || a7 = x[3], x[14] || x[ 4], x[ 9]
        
        t0 = rv64_packm(a0, a2); //  0, 12 | A, D   |  Column 0
        t1 = rv64_packm(a7, a5); //  4,  8 | B, C   |
        t2 = rv64_packm(a2, a4); //  1, 13 |        |  Column 1
        t3 = rv64_packm(a1, a7); //  5,  9 |        |
        t4 = rv64_packm(a4, a6); //  2, 14 |        |  Column 2
        t5 = rv64_packm(a3, a1); //  6, 10 |        |
        t6 = rv64_packm(a6, a0); //  3, 15 |        |  Column 3
        t7 = rv64_packm(a5, a3); //  7, 11 |        |
    }


    out[ 0] = (a0>>32) + in[ 0];
    out[ 1] = (a2>>32) + in[ 1];
    out[ 2] = (a4>>32) + in[ 2];
    out[ 3] = (a6>>32) + in[ 3];
    out[ 4] = (a7>>32) + in[ 4];
    out[ 5] = (a1>>32) + in[ 5];
    out[ 6] = (a3>>32) + in[ 6];
    out[ 7] = (a5>>32) + in[ 7];
    out[ 8] = (a5    ) + in[ 8];
    out[ 9] = (a7    ) + in[ 9];
    out[10] = (a1    ) + in[10];
    out[11] = (a3    ) + in[11];
    out[12] = (a2    ) + in[12];
    out[13] = (a4    ) + in[13];
    out[14] = (a6    ) + in[14];
    out[15] = (a0    ) + in[15];
}

