
#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include "chacha20.h"
#include "chacha20_ise.h"

#define MEASURE(ncyc, nins, stmt) {                         \
    asm volatile( "rdinstret %0" : "=r" (instr_pre)  );     \
    asm volatile( "rdcycle   %0" : "=r" (cycle_pre)  );     \
    stmt;                                                   \
    asm volatile( "rdcycle   %0" : "=r" (cycle_post) );     \
    asm volatile( "rdinstret %0" : "=r" (instr_post) );     \
                                                            \
    ncyc =   cycle_post - cycle_pre;                        \
    nins =   instr_post - instr_pre;                        \
  }

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
uint32_t output1[16];
uint32_t output2[16];
uint32_t output3[16];
uint32_t output4[16];

uint32_t check_result(uint32_t n, uint32_t expect, uint32_t res){
    if(expect != res)  {
        printf("Error at V%d: %02d Expected %08lx, got %08lx\n", n, expect, res);
        return 1; 
    }
    return 0;
}

int main(int argc, char ** argv) {
    uint64_t cycle_pre, cycle_post;                         
    uint64_t instr_pre, instr_post;                         
    uint64_t ncyc0, ncyc1, ncyc2, ncyc3, ncyc4;
    uint64_t nins0, nins1, nins2, nins3, nins4; 
    printf("Test\n");

    int mismatches = 0;	
/*
    long int i, j, r1, e1;
    i =0xAAAAAAAABBBBBBBB;
    j =0x5555555577777777;
    e1=0xBBBBBBBB55555555; 

    rv64_packtest(r1,i,j); 
     
    if(e1 != r1) {  
        printf("test_pack [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", e1, r1);
        mismatches = 1;
    } 
 
*/  

    MEASURE( ncyc0, nins0, chacha20_block(     output,  input) );
    MEASURE( ncyc1, nins1, chacha20_block_ise1(output1, input) );
    MEASURE( ncyc2, nins2, chacha20_block_ise2(output2, input) );
    MEASURE( ncyc3, nins3, chacha20_block_ise3(output3, input) );
    MEASURE( ncyc4, nins4, chacha20_block_ise4(output4, input) );
 
    for(int i = 0; i < 16; i ++) {

        mismatches += check_result(0,expect[i], output[i]);
        mismatches += check_result(1,expect[i], output1[i]);
        mismatches += check_result(2,expect[i], output2[i]);
        mismatches += check_result(3,expect[i], output3[i]);
        mismatches += check_result(4,expect[i], output4[i]);
    }

    if(mismatches == 0) {
        printf("PASS\n");
        printf("N_cyc0: %d, N_ins0: %d\n", ncyc0, nins0);
        printf("N_cyc1: %d, N_ins1: %d\n", ncyc1, nins1);
        printf("N_cyc2: %d, N_ins2: %d\n", ncyc2, nins2);
        printf("N_cyc3: %d, N_ins3: %d\n", ncyc3, nins3);
        printf("N_cyc4: %d, N_ins4: %d\n", ncyc4, nins4);
    }
    
    return (mismatches > 0);
}
