
#include <stdint.h>

#ifndef __ISE_H__
#define __ISE_H__

//! Pack [low  32-bits of rs2, low  32-bits of rs1].
uint64_t rv64_pack(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, high 32-bits of rs1].
uint64_t rv64_packh(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, low 32-bits of rs1].
uint64_t rv64_packm(uint64_t rs1, uint64_t rs2);

uint64_t chacha_ad0(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc0(uint64_t rs1, uint64_t rs2);

uint64_t chacha_ad1(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc1(uint64_t rs1, uint64_t rs2);

//! A complete chacha quarter round function. out_rs* may equal in_rs*.
#define CHACHA_QR(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs1= chacha_ad0(in_rs1 ,in_rs2); \
    out_rs2= chacha_bc0(out_rs1,in_rs2); \
    out_rs1= chacha_ad1(out_rs1,out_rs2); \
    out_rs2= chacha_bc1(out_rs1,out_rs2); \
}

#endif

