
#include <stdint.h>
#include "ise_pack.h"

#ifndef __ISE_H__
#define __ISE_H__

uint64_t chacha_ad(uint64_t rs1, uint64_t rs2);

uint64_t chacha_bc(uint64_t rs1, uint64_t rs2);

#define CHACHA_QR(out_rs1, out_rs2, in_rs1,in_rs2) {          \
    out_rs1= chacha_ad(in_rs1 ,in_rs2); \
    out_rs2= chacha_bc(out_rs1,in_rs2); \
}

#endif

