
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
void chacha20_block_ise4(uint32_t out[16], uint32_t const in[16]);

#endif
