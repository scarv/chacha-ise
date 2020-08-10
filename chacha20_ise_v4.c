
#include "chacha20.h"
#include "ise_v4.h"

//! A vanilla implementation of the ChaCha20 block function.
void chacha20_block(uint32_t out[16], uint32_t const in[16])
{
    uint64_t * pin  = (uint64_t *)in;

    // 8 instructions to load inputs.
    uint64_t a0 = pin[0];  //  1,  0  //A
    uint64_t a1 = pin[1];  //  3,  2  
    uint64_t a2 = pin[2];  //  5,  4  //B
    uint64_t a3 = pin[3];  //  7,  6
    uint64_t a4 = pin[4];  //  9,  8  //C
    uint64_t a5 = pin[5];  // 11, 10
    uint64_t a6 = pin[6];  // 13, 12  //D
    uint64_t a7 = pin[7];  // 15, 14

    uint64_t t2,t3,t6,t7;
    //  10 loops × 2 rounds/loop = 20 rounds
    //   8 instructions per Quarter Round
    //  20 instructions per Round / 40 per double round.
    // 400 instructions total in the loop.
    for(int i = 0; i < CHACHA20_ROUNDS; i += 2)
    {
        //                                A          B            C              D
        CHACHA_QR(a0,a2,a4,a6); // QR(x[1]||x[0], x[5]||x[4], x[ 9]||x[ 8], x[13]||x[12]) - column 1 & 0 
        CHACHA_QR(a1,a3,a5,a7); // QR(x[3]||x[2], x[7]||x[6], x[11]||x[10], x[15]||x[14]) - column 3 & 2

        t2 = rv64_packlh(a2, a3); //  6, 5 
        t3 = rv64_packlh(a3, a2); //  4, 7 
        t6 = rv64_packlh(a7, a6); // 12,15
        t7 = rv64_packlh(a6, a7); // 14,13
        //                                A          B            C              D
        CHACHA_QR(a0,t2,a5,t6); // QR(x[1]||x[0], x[6]||x[5], x[11]||x[10], x[12]||x[15]) - column 1 & 0 
        CHACHA_QR(a1,t3,a4,t7); // QR(x[3]||x[2], x[4]||x[7], x[ 9]||x[ 8], x[14]||x[13]) - column 3 & 2

        a2 = rv64_packlh(t3, t2); //  5, 4 
        a3 = rv64_packlh(t2, t3); //  7, 6 
        a6 = rv64_packlh(t6, t7); // 13,12
        a7 = rv64_packlh(t7, t6); // 15,14
    }

    // 4 instructions each: load + shift + add + store - 32 total
    out[ 1] = (a0>>32) + in[ 1];
    out[ 3] = (a1>>32) + in[ 3];
    out[ 5] = (a2>>32) + in[ 5];
    out[ 7] = (a3>>32) + in[ 7];
    out[ 9] = (a4>>32) + in[ 9];
    out[11] = (a5>>32) + in[11];
    out[13] = (a6>>32) + in[13];
    out[15] = (a7>>32) + in[15];

    // 3 instructions each: load + add + store - 24 total.
    out[ 0] = (a0    ) + in[ 0];
    out[ 2] = (a1    ) + in[ 2];
    out[ 4] = (a2    ) + in[ 4];
    out[ 6] = (a3    ) + in[ 6];
    out[ 8] = (a4    ) + in[ 8];
    out[10] = (a5    ) + in[10];
    out[12] = (a6    ) + in[12];
    out[14] = (a7    ) + in[14];

    // 464 instructions total.
}

