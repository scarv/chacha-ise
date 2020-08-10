
#include <stdint.h>

#ifndef __ISE_H__
#define __ISE_H__

//! Pack [low  32-bits of rs2, low  32-bits of rs1].
uint64_t rv64_pack(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, high 32-bits of rs1].
uint64_t rv64_packh(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, low 32-bits of rs1].
uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2);

//! Pack [low 32-bits of rs2, high 32-bits of rs1].
uint64_t rv64_packlh(uint64_t rs1, uint64_t rs2);

uint64_t chacha_add(uint64_t rs1, uint64_t rs2);

uint64_t chacha_xorroli(uint64_t rs1, uint64_t rs2,uint8_t ri);

#define CHACHA_QR(A, B, C, D) {          \
    A = chacha_add(A,B);  D = chacha_xorroli(D,A,16); \
    C = chacha_add(C,D);  B = chacha_xorroli(B,C,12); \
    A = chacha_add(A,B);  D = chacha_xorroli(D,A,8); \
    C = chacha_add(C,D);  B = chacha_xorroli(B,C,7); \
}
#endif

