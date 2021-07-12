/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "chacha_openssl.h"

const char* pass_str = "\x1b[32mPASS\x1b[0m";
const char* fail_str = "\x1b[31mFAIL\x1b[0m";

extern void chacha20_isa   (uint8_t *out, const uint8_t *in,
			    size_t in_len, const uint8_t key[32],
			    const uint8_t nonce[12], uint32_t counter);
extern void chacha20_vec_v1(uint8_t *out, const uint8_t *in,
			    size_t in_len, const uint8_t key[32],
			    const uint8_t nonce[12], uint32_t counter);
extern void chacha20_vec_v2(uint8_t *out, const uint8_t *in,
			    size_t in_len, const uint8_t key[32],
			    const uint8_t nonce[12], uint32_t counter);
extern uint64_t instruction_counter();

void println_hex(uint8_t* data, int size);

bool test_chacha(const uint8_t* data, size_t len, 
                 const uint8_t key[32], const uint8_t nonce[12], 
                 bool verbose); 
