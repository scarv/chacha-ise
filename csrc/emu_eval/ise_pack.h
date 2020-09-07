
#include <stdint.h>

#ifndef __IPACK_H__
#define __IPACK_H__

#define ROL32(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROR32(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

//! Pack [low  32-bits of rs2, low  32-bits of rs1].
//uint64_t rv64_pack(uint64_t rs1, uint64_t rs2);
#define rv64_pack(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0000100, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//! Pack [high 32-bits of rs2, high 32-bits of rs1].
//uint64_t rv64_packh(uint64_t rs1, uint64_t rs2);
#define rv64_packh(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0000101, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//! Pack [high 32-bits of rs2, low 32-bits of rs1].
//uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2);
#define rv64_packhl(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0000110, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

//! Pack [low 32-bits of rs2, high 32-bits of rs1].
//uint64_t rv64_packlh(uint64_t rs1, uint64_t rs2);
#define rv64_packlh(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0000111, %0, %1, %2\n\t" :"=r" (rd) : "r" (rs1), "r" (rs2));

#define rv64_packtest(rd, rs1, rs2) asm volatile ( \
    ".insn r CUSTOM_0, 7, 0b0000100, t1, %2, %3\n\t" \
    ".insn r CUSTOM_0, 7, 0b0000101, t2, %2, %3\n\t" \
    ".insn r CUSTOM_0, 7, 0b0000110, t3, %2, %3\n\t" \
    ".insn r CUSTOM_0, 7, 0b0000111, t4, %2, %3\n\t" \
    ".insn r CUSTOM_0, 7, 0b0000111, %0, %3, %2\n\t" \
    :"=r" (rd) : "r" (rd), "r" (rs1), "r" (rs2)      \
    :"t1", "t2", "t3", "t4");
#endif

/*    "nop \n\t nop \n\t nop \n\t nop\n\t" \*/
