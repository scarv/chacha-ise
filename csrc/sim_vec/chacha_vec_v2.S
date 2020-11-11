.section .text
.macro ROTLV a r
	vslidedown.vi  v31, \a, \r
	vslideup.vi    v31, \a, 4-\r
	vmv.v.v         \a,  v31
.endm

.macro ROTLV1 a
	vrgather.vv  v31, \a, v30
	vmv.v.v         \a,  v31
.endm
.macro ROTLV2 a
	vrgather.vv  v31, \a, v29
	vmv.v.v         \a,  v31
.endm
.macro ROTLV3 a
	vrgather.vv  v31, \a, v28
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
#touched v0,v1,v2,v3,  v4,v5,v6,v7, v28,v29,v30,v31
.global chacha20_vec_v2
chacha20_vec_v2:
	# a2 = initial length in bytes

	li t3, -1
	vsetvli a6, t3, e32
    addi    a6, a6, -4

    bnez    a6, init_VL256
init_VL128:
	la t0, V128_ROTL_Ind
	# load vector v30, v29, v28 for rotation left 1,2,3 elements
	vle32.v v30, (t0)  
	addi t0, t0, 16
        vle32.v v29, (t0)  
	addi t0, t0, 16   
        vle32.v v28, (t0) 

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
    j encrypt_blocks

init_VL256:
	li t3, 8
	vsetvli t1, t3, e32
	la t0, V256_ROTL_Ind
	# load vector v30, v29, v28 for rotation left 1,2,3 elements
	vle32.v v30, (t0)  
	addi t0, t0, 32
	vle32.v v29, (t0)  
	addi t0, t0, 32   
	vle32.v v28, (t0) 

	la t0, V256_Ind
	# load vector v26, v27 for vector extension and loading input data
	vle32.v v26, (t0)  
	addi t0, t0, 32
	vle32.v v27, (t0)  

	# initialize vector state
	# Load 128 bit constant
        la t0, ChaChaConstant
	vle32.v v4, (t0)

	# Load key
	vle32.v v1, (a3)
	vrgather.vv   v5, v1, v26
	vslidedown.vi v2, v1, 4
	vrgather.vv   v6, v2, v26
	# Load nonce
	vle32.v v8, (a4)
      
encrypt_blocks:

    bnez    a6, updatecounter_VL256
updatecounter_VL128:
	vslide1up.vx  v7, v8, a5
	j           reload_states

updatecounter_VL256:
	addi t1, a5, 1
	vslide1up.vx  v1, v8, t1
	vrgather.vv   v7, v1, v26
	vmv.s.x       v7, a5

reload_states:
	vmv4r.v v0, v4

	li t2, 10 # loop counter
round_loop:

	# Mix columns.
	CHACHA_FR v0, v1, v2, v3   
  
	# element rotation
	ROTLV1 v1
	ROTLV2 v2
	ROTLV3 v3

	# Mix diagonals.
	CHACHA_FR v0, v1, v2, v3

	# element rotation
	ROTLV3 v1
	ROTLV2 v2
	ROTLV1 v3
	
	addi t2, t2, -1
	bnez t2, round_loop

	# Add in initial block values.
	vadd.vv v0, v0, v4
	vadd.vv v1, v1, v5
	vadd.vv v2, v2, v6
	vadd.vv v3, v3, v7

    bnez    a6, enc_VL256
enc_VL128:
	# loading input data
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
	addi a5, a5, 1  # increment counter
	li   t0, 64     # current VL in bytes
    j    update_pointer

enc_VL256:
	# loading input data
	vlxei32.v v16, (a1), v27
	addi    t0,   a1,   16
	vlxei32.v v17, (t0), v27
	addi    t0,   t0,   16
	vlxei32.v v18, (t0), v27
	addi    t0,   t0,   16
	vlxei32.v v19, (t0), v27

	# xor in state
	vxor.vv v16, v16, v0
	vxor.vv v17, v17, v1
	vxor.vv v18, v18, v2
	vxor.vv v19, v19, v3

	vsxei32.v v16, (a0), v27
	addi    t0,   a0,   16
	vsxei32.v v17, (t0), v27
	addi    t0,   t0,   16
	vsxei32.v v18, (t0), v27
	addi    t0,   t0,   16
	vsxei32.v v19, (t0), v27

	# update counters/pointers
	addi a5, a5, 2  # increment counter
	li   t0, 128    # current VL in bytes 

update_pointer:
	add  a0, a0, t0 # advance output pointer
	add  a1, a1, t0 # advance input pointer
	sub  a2, a2, t0 # decrement remaining bytes


	bgt a2, x0, encrypt_blocks

return:
	ret

.section .data
.balign 8                 # align to 4 bytes
ChaChaConstant:
.word   0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
.word   0x61707865, 0x3320646e, 0x79622d32, 0x6b206574

V128_ROTL_Ind:
.word   1, 2, 3, 0   
.word   2, 3, 0, 1
.word   3, 0, 1, 2

V256_ROTL_Ind:
.word   1, 2, 3, 0, 5, 6, 7, 4  
.word   2, 3, 0, 1, 6, 7, 4, 5   
.word   3, 0, 1, 2, 7, 4, 5, 6

V256_Ind: 
.word   0, 1, 2, 3, 0, 1, 2, 3
.word   0, 4, 8,12,64,68,72,76



