
#include "ise_pack.h"

uint64_t rv64_pack(uint64_t rs1, uint64_t rs2) {
    return ((rs1 << 32)>>32) | (rs2 << 32);
}

uint64_t rv64_packh(uint64_t rs1, uint64_t rs2) {
    return (rs1 >> 32) | ((rs2 >> 32)<<32);
}

// [high half of rs2, low half of rs1]
uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2) {
    return ((rs2 >> 32)<<32) | ((rs1 << 32) >> 32);
}

// [low half of rs2, high half of rs1]
uint64_t rv64_packlh(uint64_t rs1, uint64_t rs2) {
    return (rs2<<32) | (rs1 >> 32);
}


