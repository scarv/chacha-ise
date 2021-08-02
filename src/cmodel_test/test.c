/* Copyright (C) 2021 SCARV project <info@scarv.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found at https://opensource.org/licenses/MIT (or should be included 
 * as LICENSE.txt within the associated archive or repository).
 */
#include <stdio.h>
#include <stdint.h>

#include "chacha20.h"

uint32_t input[16] = {
    0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
    0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
    0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c,
    0x00000001, 0x09000000, 0x4a000000, 0x00000000
};

uint32_t expect[16] = {
    0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3,
    0xc7f4d1c7, 0x0368c033, 0x9aaa2204, 0x4e6cd4c3,
    0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
    0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2
};

uint32_t output[16];

int main(int argc, char ** argv) {

    chacha20_block(output, input);
    
    int mismatches = 0;
    for(int i = 0; i < 16; i ++) {
        if(expect[i] != output[i])  {
            printf("Error: %02d Expected %08X, got %08X\n",
                i, expect[i], output[i]);
            mismatches ++;
        }
    }

    if(mismatches == 0) {
        printf("PASS\n\n");
    }
    
    return (mismatches > 0);
}