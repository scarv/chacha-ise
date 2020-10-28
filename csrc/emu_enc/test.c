/* 
 * referenced implementation @ https://github.com/edre/rvv-chacha-poly
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "boring.h"

extern void chacha20_ise_v4(uint8_t *out, const uint8_t *in, size_t in_len, const uint8_t key[32], const uint8_t nonce[12], uint32_t counter);

void println_hex(uint8_t* data, int size) {
  while (size > 0) {
    printf("%02x", *data);
    data++;
    size--;
  } 
  printf("\n");
}

int datacmp(uint8_t* data1, uint8_t* data2, int size) {
  int cnt=0;
  while (size > 0) {
    if (data1[size-1] != data2[size-1])  cnt++;
    size--;
  }
  return cnt;
}

extern uint64_t instruction_counter();

const char* pass_str = "\x1b[32mPASS\x1b[0m";
const char* fail_str = "\x1b[31mFAIL\x1b[0m";

bool test_chacha(const uint8_t* data, size_t len, const uint8_t key[32], const uint8_t nonce[12], bool verbose) {
  uint8_t golden[1024];
  uint64_t start = instruction_counter();
  boring_chacha20(golden, data, len, key, nonce, 0);
  uint64_t end = instruction_counter();
  uint64_t boring_count = end - start;

  uint8_t test_res[1024]; 
  start = instruction_counter();
  chacha20_ise_v4(test_res, data, len, key, nonce, 0);
  end = instruction_counter();

  int  cmp  = datacmp(golden, test_res, len); 
  bool pass = cmp == 0;

  if (verbose || !pass) {
    printf("golden  : ");
    println_hex(golden, 64);
    printf("inst_count=%ld \n", boring_count);
    printf("test_res: ");
    println_hex(test_res, 64);
    printf("inst_count=%ld\n", end - start);
  }

  if (pass) {
    printf("Ref = %d\n", boring_count);
    printf("V_4 = %d\n", end - start);
  }

  return pass;
}

int main(int argc, uint8_t *argv[]) {
  int len[5] = {64, 128, 256, 512, 1024};
  uint8_t data[1024];
  uint32_t rand = 1;
  for (int i = 0; i < 1024; i++) {
    rand *= 101;
    rand %= 16777213; // random prime
    data[i] = (uint8_t)(rand); 
//    data[i] = 0;
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
