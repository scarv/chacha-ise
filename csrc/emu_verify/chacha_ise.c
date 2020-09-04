#include <stdio.h>
#include "util.h"
#include "ise_pack.h"
#include "ise_v1.h"
#include "ise_v2.h"
#include "ise_v3.h"
#include "ise_v4.h"

extern uint64_t test_packll(uint64_t a, uint64_t b);
extern uint64_t test_packhh(uint64_t a, uint64_t b);
extern uint64_t test_packhl(uint64_t a, uint64_t b);
extern uint64_t test_packlh(uint64_t a, uint64_t b);

extern uint64_t test_chacha_ad_v1(uint64_t a, uint64_t b);
extern uint64_t test_chacha_bc_v1(uint64_t a, uint64_t b);

extern uint64_t test_chacha_ad_v2(uint64_t a, uint64_t b);
extern uint64_t test_chacha_bc_v2(uint64_t a, uint64_t b);
extern uint64_t test_chacha_bd_v2(uint64_t a, uint64_t b);

extern uint64_t test_chacha_ad0(  uint64_t a, uint64_t b);
extern uint64_t test_chacha_bc0(  uint64_t a, uint64_t b);
extern uint64_t test_chacha_ad1(  uint64_t a, uint64_t b);
extern uint64_t test_chacha_bc1(  uint64_t a, uint64_t b);

extern uint64_t test_chacha_add(     uint64_t a, uint64_t b);
extern uint64_t test_chacha_xorrol16(uint64_t a, uint64_t b);
extern uint64_t test_chacha_xorrol12(uint64_t a, uint64_t b);
extern uint64_t test_chacha_xorrol08(uint64_t a, uint64_t b);
extern uint64_t test_chacha_xorrol07(uint64_t a, uint64_t b);

void print_result_expectation(
    uint64_t lhs,
    uint64_t rhs,
    uint64_t result,
    uint64_t expected) {
    printf("Expected: %08lx, got: %08lx\n", expected, result);
}

int main() {
    
    int fail = 0;

    uint64_t lhs = 0xABCDEF0123456789;
    uint64_t rhs = 0xDEADBEADDEADBEAD;

    printf("# ChaCha ISE Unit Test \n");

    for (int i=0;i<10;i++){
    printf("lhs: %08lx, rhs: %08lx\n", lhs, rhs);

    // pack    
    uint64_t result = test_packll(lhs, rhs);
    uint64_t expect = rv64_pack(  lhs, rhs);

    if(result != expect) {
        printf("test_pack [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // packh
    result = test_packhh(lhs, rhs);
    expect = rv64_packh( lhs, rhs);

    if(result != expect) {
        printf("test_packh [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // packhl
    result = test_packhl(lhs, rhs);
    expect = rv64_packhl(lhs, rhs);

    if(result != expect) {
        printf("test_packhl [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    //packlh
    result = test_packlh(lhs, rhs);
    expect = rv64_packlh(lhs, rhs);

    if(result != expect) {
        printf("test_packlh [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_ad_v1
    result = test_chacha_ad_v1(lhs, rhs);
    expect =      chacha_ad_v1(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_ad_v1 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_bc_v1
    result = test_chacha_bc_v1(lhs, rhs);
    expect =      chacha_bc_v1(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_bc_v1 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_ad_v2
    result = test_chacha_ad_v2(lhs, rhs);
    expect =      chacha_ad_v2(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_ad_v2 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_bc_v2
    result = test_chacha_bc_v2(lhs, rhs);
    expect =      chacha_bc_v2(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_bc_v2 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }
    
    // chacha_bd_v2
    result = test_chacha_bd_v2(lhs, rhs);
    expect =      chacha_bd_v2(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_bd_v2 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_ad0
    result = test_chacha_ad0(lhs, rhs);
    expect =      chacha_ad0(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_ad0 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_bc0
    result = test_chacha_bc0(lhs, rhs);
    expect =      chacha_bc0(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_bc0 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }
    // chacha_ad1
    result = test_chacha_ad1(lhs, rhs);
    expect =      chacha_ad1(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_ad1 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_bc1
    result = test_chacha_bc1(lhs, rhs);
    expect =      chacha_bc1(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_bc1 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_add
    result = test_chacha_add(lhs, rhs);
    expect =      chacha_add(lhs, rhs);

    if(result != expect) {
        printf("test_chacha_add [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_xorrol16
    result = test_chacha_xorrol16(lhs, rhs);
    expect =      chacha_xorroli (lhs, rhs, 16);

    if(result != expect) {
        printf("test_chacha_xorrol16 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_xorrol12
    result = test_chacha_xorrol12(lhs, rhs);
    expect =      chacha_xorroli (lhs, rhs, 12);

    if(result != expect) {
        printf("test_chacha_xorrol12 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_xorrol08
    result = test_chacha_xorrol08(lhs, rhs);
    expect =      chacha_xorroli (lhs, rhs, 8);

    if(result != expect) {
        printf("test_chacha_xorrol08 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    // chacha_xorrol07
    result = test_chacha_xorrol07(lhs, rhs);
    expect =      chacha_xorroli (lhs, rhs, 7);

    if(result != expect) {
        printf("test_chacha_xorrol07 [FAIL]\n");
        printf("Expected: %08lx, got: %08lx\n", expect, result);
        fail = 1;
    }

    rhs = lfsr(lhs);
    lhs = lfsr(rhs);
    }
    return fail;

}

