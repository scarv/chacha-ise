
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

uint64_t chacha20_odd_ad0(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((d ^ na), 16);
    return ((uint64_t)na) << 32 | nd;
}

uint64_t chacha20_odd_bc0(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs2 >> 32;
    uint32_t c = rs2      ;
    uint32_t d = rs1      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b),12);
    return ((uint64_t)nb) << 32 | nc;
}

uint64_t chacha20_odd_ad1(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((d ^ na),  8);
    return ((uint64_t)na) << 32 | nd;
}

uint64_t chacha20_odd_bc1(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs2 >> 32;
    uint32_t c = rs2      ;
    uint32_t d = rs1      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b), 7);
    return ((uint64_t)nb) << 32 | nc;
}


#define ODD_QR(rs1,rs2) {          \
    rs1= chacha20_odd_ad0(rs1,rs2); \
    rs2= chacha20_odd_bc0(rs1,rs2); \
    rs1= chacha20_odd_ad1(rs1,rs2); \
    rs2= chacha20_odd_bc1(rs1,rs2); \
}

//                                  AB            ?D
uint64_t chacha20_even_bd0(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs1      ;
    uint32_t d = rs2      ;
    uint32_t nb = b  ;
    uint32_t nd = ROL32((d ^ (a+b)), 16);
    return ((uint64_t)nb) << 32 | nd;
}

//                                  BD            ?C
uint64_t chacha20_even_bc0(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs1 >> 32;
    uint32_t d = rs1      ;
    uint32_t c = rs2      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b),12);
    return ((uint64_t)nb) << 32 | nc;
}
//                                  AD ,          BD
uint64_t chacha20_even_ad1(uint64_t rs1, uint64_t rs2) {
    uint32_t a = rs1 >> 32;
    uint32_t b = rs2 >> 32;
    uint32_t d = rs1      ;
    uint32_t na = a+b;
    uint32_t nd = ROL32((d ^ na),  8);
    return ((uint64_t)na) << 32 | nd;
}

//                                  ?B            CD
uint64_t chacha20_even_bc1(uint64_t rs1, uint64_t rs2) {
    uint32_t b = rs1      ;
    uint32_t c = rs2 >> 32;
    uint32_t d = rs2      ;
    uint32_t nc = c + d;
    uint32_t nb = ROL32((nc ^ b), 7);
    return ((uint64_t)nb) << 32 | nc;
}

//              AD , BC
#define EVEN_QR(rs1,rs2) {           \
    uint64_t bd, bc;                 \
    bd = chacha20_even_bd0(rs1,rs2); \
    bc = chacha20_even_bc0(bd ,rs2); \
    rs1= chacha20_even_ad1(rs1,bd );  \
    rs2= chacha20_even_bc1(rs1,bc );  \
}

void chacha20_qr(uint32_t g[4]) {
    uint64_t tmp;
    uint32_t a = g[0];
    uint32_t b = g[1];
    uint32_t c = g[2];
    uint32_t d = g[3];

    uint64_t ad = rv64_pack(d,a); 
    uint64_t bc = rv64_pack(c,b); 

    ODD_QR(ad,bc)

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

    uint64_t t0 = rv64_pack (a6, a0); //  0,  4  Column 0
    uint64_t t1 = rv64_pack (a4, a2); //  8, 12
    uint64_t t2 = rv64_packh(a6, a0); //  1,  5  Column 1
    uint64_t t3 = rv64_packh(a4, a2); //  9, 13
    uint64_t t4 = rv64_pack (a7, a1); //  2,  6  Column 2
    uint64_t t5 = rv64_pack (a5, a3); // 10, 14
    uint64_t t6 = rv64_packh(a7, a1); //  3,  7  Column 3
    uint64_t t7 = rv64_packh(a5, a3); // 11, 15

    //for(int i = 0; i < CHACHA20_ROUNDS; i += 2) {
        printf("---\n");
        printf("%016llX %016llX\n", t0, t1);
        printf("%016llX %016llX\n", t2, t3);
        printf("%016llX %016llX\n", t4, t5);
        printf("%016llX %016llX\n", t6, t7);

                      //    A     B     C      D
        ODD_QR(t0, t1); // QR(x[0], x[4], x[ 8], x[12]) - column 0
        ODD_QR(t2, t3); // QR(x[1], x[5], x[ 9], x[13]) - column 1
        ODD_QR(t4, t5); // QR(x[2], x[6], x[10], x[14]) - column 2
        ODD_QR(t6, t7); // QR(x[3], x[7], x[11], x[15]) - column 3
        
        printf("---\n");
        printf("%016llX %016llX\n", t0, t1);
        printf("%016llX %016llX\n", t2, t3);
        printf("%016llX %016llX\n", t4, t5);
        printf("%016llX %016llX\n", t6, t7);
        
        //              A    D        B     C
        // t0 || t1 = x[0], x[12] || x[4], x[ 8]
        // t2 || t3 = x[1], x[13] || x[5], x[ 9]
        // t4 || t5 = x[2], x[14] || x[6], x[10]
        // t6 || t7 = x[3], x[15] || x[7], x[11]

        a0 = rv64_packh(t3, t0); //  0,  5 | A, B   |  Column 0
        a1 = rv64_pack (t6, t5); // 10, 15 | C, D   |
        a2 = rv64_packh(t5, t2); //  1,  6 |        |  Column 1
        a3 = rv64_pack (t0, t7); // 11, 12 |        |
        a4 = rv64_packh(t7, t4); //  2,  7 |        |  Column 2
        a5 = rv64_pack (t2, t1); //  8, 13 |        |
        a6 = rv64_packh(t1, t6); //  3,  4 |        |  Column 3
        a7 = rv64_pack (t4, t3); //  9, 14 |        |
        
        printf("---\n");
        printf("%016llX %016llX\n", a0, a1);
        printf("%016llX %016llX\n", a2, a3);
        printf("%016llX %016llX\n", a4, a5);
        printf("%016llX %016llX\n", a6, a7);
        
        EVEN_QR(a0, a1); // QR(x[0], x[5], x[10], x[15]); // diagonal 1
        //EVEN_QR(a2, a3); // QR(x[1], x[6], x[11], x[12]); // diagonal 2
        //EVEN_QR(a4, a5); // QR(x[2], x[7], x[ 8], x[13]); // diagonal 3
        //EVEN_QR(a6, a7); // QR(x[3], x[4], x[ 9], x[14]); // diagonal 4
        
        printf("---\n");
        printf("%016llX %016llX\n", a0, a1);
        printf("%016llX %016llX\n", a2, a3);
        printf("%016llX %016llX\n", a4, a5);
        printf("%016llX %016llX\n", a6, a7);
    //}


    out[ 0] = t0     ;
    out[ 1] = t0>>32 ;
    out[ 2] = t1     ;
    out[ 3] = t1>>32 ;
    out[ 4] = t2     ;
    out[ 5] = t2>>32 ;
    out[ 6] = t3     ;
    out[ 7] = t3>>32 ;
    out[ 8] = t4     ;
    out[ 9] = t4>>32 ;
    out[10] = t5     ;
    out[11] = t5>>32 ;
    out[12] = t6     ;
    out[13] = t6>>32 ;
    out[14] = t7     ;
    out[15] = t7>>32 ;
}

