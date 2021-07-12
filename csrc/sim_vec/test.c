/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */

/* 
 * referenced implementation @ https://github.com/edre/rvv-chacha-poly
 */
#include "test.h"

void println_hex(uint8_t* data, int size) {
  while (size > 0) {
    printf("%02x", *(data+0));
    data++;
    size--;
  }
  printf("\n");
}

bool test_chacha(const uint8_t* data, size_t len, const uint8_t key[32], const uint8_t nonce[12], bool verbose) {

  uint32_t counter[4]; 
  counter[0] = 0; 
  counter[1] = U8TO32_LITTLE(nonce + 0);
  counter[2] = U8TO32_LITTLE(nonce + 4);
  counter[3] = U8TO32_LITTLE(nonce + 8);

  uint8_t golden[4096];
  uint8_t isa_opt[4096];
  uint8_t vector1[4096];
  uint8_t vector2[4096];

  chacha20_isa(isa_opt, data, len, key, nonce, 0);
  chacha20_vec_v1(vector1, data, len, key, nonce, 0);
  chacha20_vec_v2(vector2, data, len, key, nonce, 0);

  uint64_t start = instruction_counter();
  chacha20_openssl(golden, data, len, (uint32_t*) key, counter);
  uint64_t end = instruction_counter();
  uint64_t golden_count = end - start;

  start = instruction_counter();
  chacha20_isa   (isa_opt, data, len, key, nonce, 0);
  end = instruction_counter();
  uint64_t isaopt_count = end - start;
  int  cmp  = memcmp(golden, isa_opt, len); 

  start = instruction_counter();
  chacha20_vec_v1(vector1, data, len, key, nonce, 0);
  end = instruction_counter();
  uint64_t vec_v1_count = end - start;
  cmp  = memcmp(golden, vector1, len); 


  start = instruction_counter();
  chacha20_vec_v2(vector2, data, len, key, nonce, 0);
  end = instruction_counter();
  uint64_t vec_v2_count = end - start;
  cmp  += memcmp(golden, vector2, len); 

  bool pass = cmp == 0;
  if (verbose || !pass) {
    printf("golden  : ");
    println_hex(golden, 64);
    printf("inst_count=%d, inst/byte=%.02f\n", golden_count, (float)(golden_count)/len);
    printf("isa_opt : ");
    println_hex(vector1, 64);
    printf("inst_count=%d, inst/byte=%.02f\n", isaopt_count, (float)(isaopt_count)/len);
    printf("vector 1: ");
    println_hex(vector1, 64);
    printf("inst_count=%d, inst/byte=%.02f\n", vec_v1_count, (float)(vec_v1_count)/len);
    printf("vector 2: ");
    println_hex(vector2, 64);
    printf("inst_count=%d, inst/byte=%.02f\n", vec_v2_count, (float)(vec_v2_count)/len);
  }
  
  if (pass) {
    printf("openssl: inst_count=%5d, inst/byte=%.02f\n", golden_count, (float)(golden_count)/len);
    printf("isa_opt: inst_count=%5d, inst/byte=%.02f\n", isaopt_count, (float)(isaopt_count)/len);
    printf("vector1: inst_count=%5d, inst/byte=%.02f\n", vec_v1_count, (float)(vec_v1_count)/len);
    printf("vector2: inst_count=%5d, inst/byte=%.02f\n", vec_v2_count, (float)(vec_v2_count)/len);
  }
  return pass;
}

int main(int argc, uint8_t *argv[]) {
  extern uint32_t vlmax_u32();
  printf("VLMAX in blocks: %d\n", vlmax_u32());

  int len[5] = {64, 128, 256, 512, 1024};
  uint8_t data[4096];
  uint32_t rand = 1;
  for (int i = 0; i < 4096; i++) {
    rand *= 101;
    rand %= 16777213; // random prime
    data[i] = (uint8_t)(rand); 
  }
  uint8_t key[32] = "Setec astronomy;too many secrets";
  uint8_t nonce[12] = "BurnAfterUse";

  bool pass=true;

  for (int i = 0; i < 5; i++) { 
    printf("Data length = %d\n", len[i]);
    pass &= test_chacha(data, len[i], key, nonce, false);
  }

  if (pass) {
    printf("chacha %s\n", pass_str);
  } else {
    printf("chacha %s\n", fail_str);
  }
  printf("\n");

}
