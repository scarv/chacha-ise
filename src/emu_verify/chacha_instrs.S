
.text

.global test_packll
.func   test_packll
test_packll:
    .insn r CUSTOM_0, 7, 0b0000100, a0, a0, a1
    ret
.endfunc


.global test_packhh
.func   test_packhh
test_packhh:
    .insn r CUSTOM_0, 7, 0b0000101, a0, a0, a1
    ret
.endfunc

.global test_packhl
.func   test_packhl
test_packhl:
    .insn r CUSTOM_0, 7, 0b0000110, a0, a0, a1
    ret
.endfunc

.global test_packlh
.func   test_packlh
test_packlh:
    .insn r CUSTOM_0, 7, 0b0000111, a0, a0, a1
    ret
.endfunc

.global test_chacha_ad_v1
.func   test_chacha_ad_v1
test_chacha_ad_v1:
    .insn r CUSTOM_0, 7, 0b0000000, a0, a0, a1
    ret
.endfunc

.global test_chacha_bc_v1
.func   test_chacha_bc_v1
test_chacha_bc_v1:
    .insn r CUSTOM_0, 7, 0b0000001, a0, a0, a1
    ret
.endfunc

.global test_chacha_ad_v2
.func   test_chacha_ad_v2
test_chacha_ad_v2:
    .insn r CUSTOM_0, 7, 0b0001000, a0, a0, a1
    ret
.endfunc

.global test_chacha_bc_v2
.func   test_chacha_bc_v2
test_chacha_bc_v2:
    .insn r CUSTOM_0, 7, 0b0001010, a0, a0, a1
    ret
.endfunc

.global test_chacha_bd_v2
.func   test_chacha_bd_v2
test_chacha_bd_v2:
    .insn r CUSTOM_0, 7, 0b0001001, a0, a0, a1
    ret
.endfunc

.global test_chacha_ad0
.func   test_chacha_ad0
test_chacha_ad0:
    .insn r CUSTOM_0, 7, 0b0010000, a0, a0, a1
    ret
.endfunc

.global test_chacha_bc0
.func   test_chacha_bc0
test_chacha_bc0:
    .insn r CUSTOM_0, 7, 0b0010001, a0, a0, a1
    ret
.endfunc

.global test_chacha_ad1
.func   test_chacha_ad1
test_chacha_ad1:
    .insn r CUSTOM_0, 7, 0b0010010, a0, a0, a1
    ret
.endfunc

.global test_chacha_bc1
.func   test_chacha_bc1
test_chacha_bc1:
    .insn r CUSTOM_0, 7, 0b0010011, a0, a0, a1
    ret
.endfunc

.global test_chacha_add
.func   test_chacha_add
test_chacha_add:
    .insn r CUSTOM_0, 7, 0b0011000, a0, a0, a1
    ret
.endfunc

.global test_chacha_xorrol16
.func   test_chacha_xorrol16
test_chacha_xorrol16:
    .insn r CUSTOM_0, 7, 0b0011001, a0, a0, a1
    ret
.endfunc

.global test_chacha_xorrol12
.func   test_chacha_xorrol12
test_chacha_xorrol12:
    .insn r CUSTOM_0, 7, 0b0011010, a0, a0, a1
    ret
.endfunc

.global test_chacha_xorrol08
.func   test_chacha_xorrol08
test_chacha_xorrol08:
    .insn r CUSTOM_0, 7, 0b0011011, a0, a0, a1
    ret
.endfunc

.global test_chacha_xorrol07
.func   test_chacha_xorrol07
test_chacha_xorrol07:
    .insn r CUSTOM_0, 7, 0b0011100, a0, a0, a1
    ret
.endfunc
