/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>

#ifndef __ISE_V2_H__
#define __ISE_V2_H__

//! A complete chacha quarter round function. out_rs* may equal in_rs*.
#define CHACHA_QR_V2(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    chacha_ad0(out_rs1, in_rs1 ,in_rs2); \
    chacha_bc0(out_rs2, out_rs1,in_rs2); \
    chacha_ad1(out_rs1, out_rs1,out_rs2); \
    chacha_bc1(out_rs2, out_rs1,out_rs2); \
}

//
// Given rs1||rs2 = AD,||B,C
// Compute the upper Quarter round output values of A and D
//uint64_t chacha_ad0(uint64_t rs1, uint64_t rs2);
#define chacha_ad0(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0010000, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//
// Given rs1||rs2 = AD,||B,C
// Compute the upper Quarter round output values of B and C
//uint64_t chacha_bc0(uint64_t rs1, uint64_t rs2);
#define chacha_bc0(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0010001, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//
// Given rs1||rs2 = AD,||B,C
// Compute the lower Quarter round output values of A and D
//uint64_t chacha_ad1(uint64_t rs1, uint64_t rs2);
#define chacha_ad1(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0010010, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//
// Given rs1||rs2 = AD,||B,C
// Compute the lower Quarter round output values of B and C
//uint64_t chacha_bc1(uint64_t rs1, uint64_t rs2);
#define chacha_bc1(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0010011, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

#endif

