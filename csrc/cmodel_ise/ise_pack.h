/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>

#ifndef __ISE_PACK_H__
#define __ISE_PACK_H__

#define ROL32(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROR32(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

//! Pack [low  32-bits of rs2, low  32-bits of rs1].
uint64_t rv64_pack(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, high 32-bits of rs1].
uint64_t rv64_packh(uint64_t rs1, uint64_t rs2);

//! Pack [high 32-bits of rs2, low 32-bits of rs1].
uint64_t rv64_packhl(uint64_t rs1, uint64_t rs2);

//! Pack [low 32-bits of rs2, high 32-bits of rs1].
uint64_t rv64_packlh(uint64_t rs1, uint64_t rs2);

#endif

