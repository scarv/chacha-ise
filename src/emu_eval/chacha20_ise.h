/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdint.h>
#include "ise_pack.h"

#ifndef __CHACHA20_ISE_H__
#define __CHACHA20_ISE_H__

#ifndef CHACHA20_ROUNDS
#define CHACHA20_ROUNDS 20
#endif

void chacha20_block_ise1(uint32_t out[16], uint32_t const in[16]);
void chacha20_block_ise2(uint32_t out[16], uint32_t const in[16]);
void chacha20_block_ise3(uint32_t out[16], uint32_t const in[16]);

#endif
