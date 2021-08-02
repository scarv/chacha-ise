/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_V2_H__
#define __ISE_V2_H__

uint64_t chacha_ad0(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc0(uint64_t rs1, uint64_t rs2);

uint64_t chacha_ad1(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc1(uint64_t rs1, uint64_t rs2);

//! A complete chacha quarter round function. out_rs* may equal in_rs*.
#define CHACHA_QR_V2(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs1= chacha_ad0(in_rs1 ,in_rs2); \
    out_rs2= chacha_bc0(out_rs1,in_rs2); \
    out_rs1= chacha_ad1(out_rs1,out_rs2); \
    out_rs2= chacha_bc1(out_rs1,out_rs2); \
}

#endif

