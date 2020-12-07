#include "chacha20_ise.h"
#include "ise_v4.h"

//! A ISE-assisted implementation of the ChaCha20 block function.
void chacha20_block_ise4(uint32_t out[16], uint32_t const in[16])
{
    uint64_t * pin  = (uint64_t *)in;
    uint64_t * pout  = (uint64_t *)out;
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
        CHACHA_QR_V4(a0,a2,a4,a6); // QR(x[1]||x[0], x[5]||x[4], x[ 9]||x[ 8], x[13]||x[12]) - column 1 & 0 
        CHACHA_QR_V4(a1,a3,a5,a7); // QR(x[3]||x[2], x[7]||x[6], x[11]||x[10], x[15]||x[14]) - column 3 & 2
        REPACK_STATE(t2,t3,a2,a3); //  5, 4, 7, 6-> 6, 5, 4, 7
        REPACK_STATE(t6,t7,a7,a6); // 13,12,15,14->12,15,14,13
//        rv64_packlh(t2, a2, a3); //  6, 5 
//        rv64_packlh(t3, a3, a2); //  4, 7 
//        rv64_packlh(t6, a7, a6); // 12,15
//        rv64_packlh(t7, a6, a7); // 14,13
        //                                A          B            C              D
        CHACHA_QR_V4(a0,t2,a5,t6); // QR(x[1]||x[0], x[6]||x[5], x[11]||x[10], x[12]||x[15]) - column 1 & 0 
        CHACHA_QR_V4(a1,t3,a4,t7); // QR(x[3]||x[2], x[4]||x[7], x[ 9]||x[ 8], x[14]||x[13]) - column 3 & 2
        REPACK_STATE(a2,a3,t3,t2); //  6, 5, 4, 7-> 5, 4, 7, 6
        REPACK_STATE(a6,a7,t6,t7); // 12,15,14,13->13,12,15,14
//        rv64_packlh(a2, t3, t2); //  5, 4 
//        rv64_packlh(a3, t2, t3); //  7, 6 
//        rv64_packlh(a6, t6, t7); // 13,12
//        rv64_packlh(a7, t7, t6); // 15,14
    }

    // 3 instructions each: load + add + store - 24 total.
    chacha_add(pout[0], a0, pin[0]);
    chacha_add(pout[1], a1, pin[1]);
    chacha_add(pout[2], a2, pin[2]);
    chacha_add(pout[3], a3, pin[3]);
    chacha_add(pout[4], a4, pin[4]);
    chacha_add(pout[5], a5, pin[5]);
    chacha_add(pout[6], a6, pin[6]);
    chacha_add(pout[7], a7, pin[7]);

    // 432 instructions total.
}

