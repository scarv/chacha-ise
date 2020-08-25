
#include <stdint.h>

#ifndef __ISE_H__
#define __ISE_H__

//! Pack [low  32-bits of rs2, low  32-bits of rs1].
uint64_t rv64_pack(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, high 32-bits of rs1].
uint64_t rv64_packh(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, low 32-bits of rs1].
uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2);


uint64_t chacha_bd(uint64_t rs1, uint64_t rs2);

uint64_t chacha_ad(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc(uint64_t rs1, uint64_t rs2);

#define CHACHA_QR(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs2= chacha_bd(in_rs1 ,in_rs2); \
    out_rs1= chacha_ad(in_rs1 ,out_rs2); \
    out_rs2= chacha_bc(out_rs1,in_rs2); \
}

#endif

