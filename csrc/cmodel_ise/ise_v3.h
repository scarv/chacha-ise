/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_V3_H__
#define __ISE_V3_H__

uint64_t chacha_add(uint64_t rs1, uint64_t rs2);

uint64_t chacha_xorroli(uint64_t rs1, uint64_t rs2,uint8_t ri);

#define CHACHA_HR_V3(A, B, C, D) {          \
    A = chacha_add(A,B);  D = chacha_xorroli(D,A,16); \
    C = chacha_add(C,D);  B = chacha_xorroli(B,C,12); \
    A = chacha_add(A,B);  D = chacha_xorroli(D,A,8); \
    C = chacha_add(C,D);  B = chacha_xorroli(B,C,7); \
}
#endif

