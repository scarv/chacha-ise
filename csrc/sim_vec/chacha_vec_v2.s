.section .text
.macro ROTLV a r
        vslidedown.vi  v31, \a, \r
        vslideup.vi    v31, \a, 4-\r
	vmv.v.v         \a,  v31
.endm

.macro VROTL a r
	vsll.vi v31, \a, \r
	vsrl.vi \a, \a, 32-\r
	vor.vv \a, \a, v31
.endm

.macro CHACHA_FR a b c d
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

# block-based implementation strategy:
# v0-v3: Block vectors. Each processing block is kept in the vectors.

## Function initialization
# Using the same order as the boring chacha arguments:
# a0 = uint8_t *out
# a1 = uint8_t *in
# a2 = size_t in_len
# a3 = uint8_t key[32]
# a4 = uint8_t nonce[12]
# a5 = uint32_t counter
.global chacha20_vec_v2
chacha20_vec_v2:
	# a2 = initial length in bytes
        li t3, -1
	vsetvli t1, t3, e32

	# initialize vector state
	# Load 128 bit constant
        la t0, ChaChaConstant
	vle32.v v4, (t0)
	# Load key
	vle32.v v5, (a3)
        addi t0, a3, 16    
	vle32.v v6, (t0)
	# Load nonce
	vle32.v v8, (a4)
      
encrypt_blocks:
	# update counter
	vslide1up.vx  v7, v8, a5

	vmv4r.v v0, v4
#	vmv.v.v v0, v4
#	vmv.v.v v1, v5
#	vmv.v.v v2, v6
#	vmv.v.v v3, v7

	li t2, 10 # loop counter
round_loop:

	# Mix columns.
	CHACHA_FR v0, v1, v2, v3   
  
        # element rotation
        ROTLV v1, 1
        ROTLV v2, 2
	ROTLV v3, 3

	# Mix diagonals.
	CHACHA_FR v0, v1, v2, v3

        # element rotation
        ROTLV v1, 3
        ROTLV v2, 2
	ROTLV v3, 1
	
	addi t2, t2, -1
	bnez t2, round_loop

	# Add in initial block values.

	vadd.vv v0, v0, v4
	vadd.vv v1, v1, v5
	vadd.vv v2, v2, v6
	vadd.vv v3, v3, v7


	vle32.v v16, (a1)
	addi    t0,   a1,   16
	vle32.v v17, (t0)
	addi    t0,   t0,   16
	vle32.v v18, (t0)
	addi    t0,   t0,   16
	vle32.v v19, (t0)

	# xor in state
	vxor.vv v16, v16, v0
	vxor.vv v17, v17, v1
	vxor.vv v18, v18, v2
	vxor.vv v19, v19, v3

	vse32.v v16, (a0)
	addi    t0,   a0,   16
	vse32.v v17, (t0)
	addi    t0,   t0,   16
	vse32.v v18, (t0)
	addi    t0,   t0,   16
	vse32.v v19, (t0)

	# update counters/pointers
	li   t0, 64 # current VL in bytes
	add  a0, a0, t0 # advance output pointer
	add  a1, a1, t0 # advance input pointer
	sub  a2, a2, t0 # decrement remaining bytes
	addi a5, a5, 1 # increment counter

	bnez a2, encrypt_blocks

return:
	ret

.section .data
.balign 8                 # align to 4 bytes
ChaChaConstant:
.word   0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
