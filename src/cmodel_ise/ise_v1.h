/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_V1_H__
#define __ISE_V1_H__

uint64_t chacha_ad_v1(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc_v1(uint64_t rs1, uint64_t rs2);

#define CHACHA_QR_V1(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs1= chacha_ad_v1(in_rs1 ,in_rs2); \
    out_rs2= chacha_bc_v1(out_rs1,in_rs2); \
}

#endif

