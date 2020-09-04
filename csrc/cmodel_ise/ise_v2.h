
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_V2_H__
#define __ISE_V2_H__

uint64_t chacha_bd_v2(uint64_t rs1, uint64_t rs2);

uint64_t chacha_ad_v2(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc_v2(uint64_t rs1, uint64_t rs2);

#define CHACHA_QR_V2(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs2= chacha_bd_v2(in_rs1 ,in_rs2); \
    out_rs1= chacha_ad_v2(in_rs1 ,out_rs2); \
    out_rs2= chacha_bc_v2(out_rs1,in_rs2); \
}

#endif

