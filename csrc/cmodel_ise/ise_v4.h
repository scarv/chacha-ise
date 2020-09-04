
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_V4_H__
#define __ISE_V4_H__

uint64_t chacha_add(uint64_t rs1, uint64_t rs2);

uint64_t chacha_xorroli(uint64_t rs1, uint64_t rs2,uint8_t ri);

#define CHACHA_QR_V4(A, B, C, D) {          \
    A = chacha_add(A,B);  D = chacha_xorroli(D,A,16); \
    C = chacha_add(C,D);  B = chacha_xorroli(B,C,12); \
    A = chacha_add(A,B);  D = chacha_xorroli(D,A,8); \
    C = chacha_add(C,D);  B = chacha_xorroli(B,C,7); \
}
#endif

