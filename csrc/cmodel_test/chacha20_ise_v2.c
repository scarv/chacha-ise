
#include "chacha20.h"
#include "ise_v2.h"

//! A vanilla implementation of the ChaCha20 block function.
void chacha20_block(uint32_t out[16], uint32_t const in[16])
{
    uint64_t * pin  = (uint64_t *)in;

    // 8 instructions to load inputs.
    uint64_t a0 = pin[0];  //  1,  0
    uint64_t a1 = pin[1];  //  3,  2
    uint64_t a2 = pin[2];  //  5,  4
    uint64_t a3 = pin[3];  //  7,  6
    uint64_t a4 = pin[4];  //  9,  8
    uint64_t a5 = pin[5];  // 11, 10
    uint64_t a6 = pin[6];  // 13, 12
    uint64_t a7 = pin[7];  // 15, 14

    // 8 instructions to pack into correct form.
    uint64_t t0 = rv64_pack (a6, a0); //  0, 12  Column 0
    uint64_t t1 = rv64_pack (a4, a2); //  4,  8
    uint64_t t2 = rv64_packh(a6, a0); //  1, 13  Column 1
    uint64_t t3 = rv64_packh(a4, a2); //  5,  9
    uint64_t t4 = rv64_pack (a7, a1); //  2, 14  Column 2
    uint64_t t5 = rv64_pack (a5, a3); //  6, 10
    uint64_t t6 = rv64_packh(a7, a1); //  3, 15  Column 3
    uint64_t t7 = rv64_packh(a5, a3); //  7, 11

	//  10 loops × 2 rounds/loop = 20 rounds
    //   4 instructions per Quarter Round
    //  24 instructions per Round / 48 per double round.
    // 480 instructions total in the loop.
    for(int i = 0; i < CHACHA20_ROUNDS; i += 2)
    {
        //        AD BC,AD BC         A     B     C      D
        CHACHA_QR_V2(a0,a1,t0,t1); // QR(x[0], x[4], x[ 8], x[12]) - column 0
        CHACHA_QR_V2(a2,a3,t2,t3); // QR(x[1], x[5], x[ 9], x[13]) - column 1
        CHACHA_QR_V2(a4,a5,t4,t5); // QR(x[2], x[6], x[10], x[14]) - column 2
        CHACHA_QR_V2(a6,a7,t6,t7); // QR(x[3], x[7], x[11], x[15]) - column 3

        t0 = rv64_packhl(a6, a0); //  0, 15 | A, D   |  Column 0
        t1 = rv64_packhl(a5, a3); //  5, 10 | B, C   |
        t2 = rv64_packhl(a0, a2); //  1, 12 |        |  Column 1
        t3 = rv64_packhl(a7, a5); //  6, 11 |        |
        t4 = rv64_packhl(a2, a4); //  2, 13 |        |  Column 2
        t5 = rv64_packhl(a1, a7); //  7,  8 |        |
        t6 = rv64_packhl(a4, a6); //  3, 14 |        |  Column 3
        t7 = rv64_packhl(a3, a1); //  4,  9 |        |
        
        //        AD BC,AD BC         A     B     C      D
        CHACHA_QR_V2(a0,a1,t0,t1); // QR(x[0], x[5], x[10], x[15]) - diagonal 0
        CHACHA_QR_V2(a2,a3,t2,t3); // QR(x[1], x[6], x[11], x[12]) - diagonal 1
        CHACHA_QR_V2(a4,a5,t4,t5); // QR(x[2], x[7], x[ 8], x[13]) - diagonal 2
        CHACHA_QR_V2(a6,a7,t6,t7); // QR(x[3], x[4], x[ 9], x[14]) - diagonal 3
        
        t0 = rv64_packhl(a2, a0); //  0, 12 | A, D   |  Column 0
        t1 = rv64_packhl(a5, a7); //  4,  8 | B, C   |
        t2 = rv64_packhl(a4, a2); //  1, 13 |        |  Column 1
        t3 = rv64_packhl(a7, a1); //  5,  9 |        |
        t4 = rv64_packhl(a6, a4); //  2, 14 |        |  Column 2
        t5 = rv64_packhl(a1, a3); //  6, 10 |        |
        t6 = rv64_packhl(a0, a6); //  3, 15 |        |  Column 3
        t7 = rv64_packhl(a3, a5); //  7, 11 |        |
    }

    // 4 instructions each: load + shift + add + store - 32 total
    out[ 0] = (a0>>32) + in[ 0];
    out[ 1] = (a2>>32) + in[ 1];
    out[ 2] = (a4>>32) + in[ 2];
    out[ 3] = (a6>>32) + in[ 3];
    out[ 4] = (a7>>32) + in[ 4];
    out[ 5] = (a1>>32) + in[ 5];
    out[ 6] = (a3>>32) + in[ 6];
    out[ 7] = (a5>>32) + in[ 7];

    // 3 instructions each: load + add + store - 24 total.
    out[ 8] = (a5    ) + in[ 8];
    out[ 9] = (a7    ) + in[ 9];
    out[10] = (a1    ) + in[10];
    out[11] = (a3    ) + in[11];
    out[12] = (a2    ) + in[12];
    out[13] = (a4    ) + in[13];
    out[14] = (a6    ) + in[14];
    out[15] = (a0    ) + in[15];

    // 552 instructions total.
}

