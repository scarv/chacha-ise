
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_V3_H__
#define __ISE_V3_H__

uint64_t chacha_ad0(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc0(uint64_t rs1, uint64_t rs2);

uint64_t chacha_ad1(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc1(uint64_t rs1, uint64_t rs2);

//! A complete chacha quarter round function. out_rs* may equal in_rs*.
#define CHACHA_QR_V3(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs1= chacha_ad0(in_rs1 ,in_rs2); \
    out_rs2= chacha_bc0(out_rs1,in_rs2); \
    out_rs1= chacha_ad1(out_rs1,out_rs2); \
    out_rs2= chacha_bc1(out_rs1,out_rs2); \
}

#endif

