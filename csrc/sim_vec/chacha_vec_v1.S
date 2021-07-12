# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

#referenced implementation @ https://github.com/edre/rvv-chacha-poly


.global instruction_counter
.global chacha20_vec_v1
.global vlmax_u32

instruction_counter:
	rdinstret a0
	ret

vlmax_u32:
	li a0, -1
	vsetvli a0, a0, e32
	ret

.macro VROTL a r
	vsll.vi v28, \a, \r
	vsrl.vi \a, \a, 32-\r
	vor.vv \a, \a, v28
.endm

.macro CHACHA_QR a b c d
	# a += b; d ^= a; d <<<= 16;
	vadd.vv \a, \a, \b
	vxor.vv \d, \d, \a
	VROTL   \d, 16
	# c += d; b ^= c; b <<<= 12;
	vadd.vv \c, \c, \d
	vxor.vv \b, \b, \c
	VROTL   \b, 12
	# a += b; d ^= a; d <<<= 8;
	vadd.vv \a, \a, \b
	vxor.vv \d, \d, \a
	VROTL   \d, 8
	# c += d; b ^= c; b <<<= 7;
	vadd.vv \c, \c, \d
	vxor.vv \b, \b, \c
	VROTL   \b, 7
.endm

# Cell-based implementation strategy:
# v0-v15: Cell vectors. Each element is from a different block

## Function initialization
# Using the same order as the boring chacha arguments:
# a0 = uint8_t *out
# a1 = uint8_t *in
# a2 = size_t in_len
# a3 = uint8_t key[32]
# a4 = uint8_t nonce[12]
# a5 = uint32_t counter
chacha20_vec_v1:
	# a2 = initial length in bytes
	# t3 = remaining 64-byte blocks to mix
	# t4 = remaining full blocks to read/write
	#  (if t3 and t4 are different by one, there is a partial block to manually xor)
	# t1 = vl in 64-byte blocks
	srli t4, a2, 6
	addi t0, a2, 63
	srli t3, t0, 6

	# initialize vector state
	vsetvli t1, t3, e32

	# Load 128 bit constant
	li t0, 0x61707865 # "expa" little endian
	vmv.v.x v16, t0
	li t0, 0x3320646e # "nd 3" little endian
	vmv.v.x v17, t0
	li t0, 0x79622d32 # "2-by" little endian
	vmv.v.x v18, t0
	li t0, 0x6b206574 # "te k" little endian
	vmv.v.x v19, t0
	# Load key
	lw t0, 0(a3)
	vmv.v.x v20, t0
	lw t0, 4(a3)
	vmv.v.x v21, t0
	lw t0, 8(a3)
	vmv.v.x v22, t0
	lw t0, 12(a3)
	vmv.v.x v23, t0
	lw t0, 16(a3)
	vmv.v.x v24, t0
	lw t0, 20(a3)
	vmv.v.x v25, t0
	lw t0, 24(a3)
	vmv.v.x v26, t0
	lw t0, 28(a3)
	vmv.v.x v27, t0

	# Load nonce
	ld t0, 0(a4)
	vmv.v.x v29, t0
	ld t0, 4(a4)
	vmv.v.x v30, t0
	ld t0, 8(a4)
	vmv.v.x v31, t0


encrypt_blocks:
	# Load counter, and increment for each element
	vid.v v28
	vadd.vx v28, v28, a5

	vmv.v.v v0 , v16
	vmv.v.v v1 , v17
	vmv.v.v v2 , v18
	vmv.v.v v3 , v19
	vmv.v.v v4 , v20
	vmv.v.v v5 , v21
	vmv.v.v v6 , v22
	vmv.v.v v7 , v23
	vmv.v.v v8 , v24
	vmv.v.v v9 , v25
	vmv.v.v v10, v26
	vmv.v.v v11, v27
	vmv.v.v v12, v28
	vmv.v.v v13, v29
	vmv.v.v v14, v30
	vmv.v.v v15, v31

	li t2, 10 # loop counter
round_loop:

	# Mix columns.
	CHACHA_QR v0, v4, v8, v12
	CHACHA_QR v1, v5, v9, v13
	CHACHA_QR v2, v6, v10, v14
	CHACHA_QR v3, v7, v11, v15
	# Mix diagonals.
	CHACHA_QR v0, v5, v10, v15
	CHACHA_QR v1, v6, v11, v12
	CHACHA_QR v2, v7, v8, v13
	CHACHA_QR v3, v4, v9, v14
	
	addi t2, t2, -1
	bnez t2, round_loop

	# take back value of v28
	vid.v v28
	vadd.vx v28, v28, a5

	# Add in initial block values.
	vadd.vv v0 , v16, v0
	vadd.vv v1 , v17, v1
	vadd.vv v2 , v18, v2
	vadd.vv v3 , v19, v3
	vadd.vv v4 , v20, v4
	vadd.vv v5 , v21, v5
	vadd.vv v6 , v22, v6
	vadd.vv v7 , v23, v7
	vadd.vv v8 , v24, v8
	vadd.vv v9 , v25, v9
	vadd.vv v10, v26, v10
	vadd.vv v11, v27, v11
	vadd.vv v12, v28, v12
	vadd.vv v13, v29, v13
	vadd.vv v14, v30, v14
	vadd.vv v15, v31, v15

	vsetvli t5, t4, e32
	li t0, 64
	vlse32.v v28, (a1), t0
	vxor.vv  v0,   v0,  v28
	vsse32.v v0,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v1,   v1,  v28
	vsse32.v v1,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v2,   v2,  v28
	vsse32.v v2,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v3,   v3,  v28
	vsse32.v v3,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v4,   v4,  v28
	vsse32.v v4,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v5,   v5,  v28
	vsse32.v v5,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v6,   v6,  v28
	vsse32.v v6,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v7,   v7,  v28
	vsse32.v v7,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4      
	vlse32.v v28, (a1), t0
	vxor.vv  v8,   v8,  v28
	vsse32.v v8,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v9,   v9,  v28
	vsse32.v v9,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v10,  v10, v28
	vsse32.v v10, (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v11,  v11, v28
	vsse32.v v11, (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v12,  v12, v28
	vsse32.v v12, (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v13,  v13, v28
	vsse32.v v13, (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v14,  v14, v28
	vsse32.v v14,  (a0), t0
	addi a1, a1, 4
	addi a0, a0, 4
	vlse32.v v28, (a1), t0
	vxor.vv  v15,  v15, v28
	vsse32.v v15, (a0), t0
	addi a1, a1, -60
	addi a0, a0, -60
  
#   sw t0, (a0)
#	sw t1, 4(a0)
#	sw t3, 8(a0)
#	sw t4, 12(a0)
#	sw t5, 16(a0)
      
	# update counters/pointers
	slli t0, t5, 6 # current VL in bytes
	add a0, a0, t0 # advance output pointer
	add a1, a1, t0 # advance input pointer
	sub a2, a2, t0 # decrement remaining bytes
	sub t3, t3, t1 # decrement remaining blocks
	sub t4, t4, t1 # decrement remaining blocks
	# TODO: crash if counter overflows
	add a5, a5, t1 # increment counter

	# loop again if we have remaining blocks
	bne x0, t3, encrypt_blocks
return:
	ret
