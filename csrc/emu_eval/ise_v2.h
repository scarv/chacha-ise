
#include <stdint.h>


#ifndef __ISE_V2_H__
#define __ISE_V2_H__

//! A complete chacha quarter round function. out_rs* may equal in_rs*.
#define CHACHA_QR_V2(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    chacha_bd_v2(out_rs2, in_rs1 ,in_rs2); \
    chacha_ad_v2(out_rs1, in_rs1 ,out_rs2); \
    chacha_bc_v2(out_rs2, out_rs1,in_rs2); \
}

//uint64_t chacha_bd(uint64_t rs1, uint64_t rs2);
#define chacha_bd_v2(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0001001, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//uint64_t chacha_ad(uint64_t rs1, uint64_t rs2);
#define chacha_ad_v2(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0001000, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//uint64_t chacha_bc(uint64_t rs1, uint64_t rs2);
#define chacha_bc_v2(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0001010, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));
#endif

