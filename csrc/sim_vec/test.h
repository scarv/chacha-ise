/* 
 * referenced implementation @ https://github.com/edre/rvv-chacha-poly
 *
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
