/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>

#ifndef __ISE_V3_H__
#define __ISE_V3_H__

//! A complete chacha quarter round function
#define CHACHA_HR_V3(A, B, C, D) {          \
    chacha_add(A,A,B);  chacha_xorrol16(D,D,A); \
    chacha_add(C,C,D);  chacha_xorrol12(B,B,C); \
    chacha_add(A,A,B);  chacha_xorrol08(D,D,A); \
    chacha_add(C,C,D);  chacha_xorrol07(B,B,C); \
}

#define REPACK_STATE(r0, r1, s0, s1) { \
        rv64_packlh(r0, s0, s1); \
        rv64_packlh(r1, s1, s0); \
}

//uint64_t chacha_add(uint64_t rs1, uint64_t rs2);
#define chacha_add(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0011000, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//uint64_t chacha_xorroli(uint64_t rs1, uint64_t rs2,uint8_t ri);
#define chacha_xorrol16(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0011001, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

#define chacha_xorrol12(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0011010, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

#define chacha_xorrol08(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0011011, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

#define chacha_xorrol07(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0011100, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

#endif

