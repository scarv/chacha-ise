.section .text
.global instruction_counter
instruction_counter:
	rdinstret a0
	ret

.macro chacha_add rd, rs1, rs2
	.insn r CUSTOM_0, 7, 0b0011000, \rd, \rs1, \rs2
.endm

.macro chacha_xorrol16 rd, rs1, rs2
	.insn r CUSTOM_0, 7, 0b0011001, \rd, \rs1, \rs2
.endm
.macro chacha_xorrol12 rd, rs1, rs2
	.insn r CUSTOM_0, 7, 0b0011010, \rd, \rs1, \rs2
.endm
.macro chacha_xorrol08 rd, rs1, rs2
	.insn r CUSTOM_0, 7, 0b0011011, \rd, \rs1, \rs2
.endm
.macro chacha_xorrol07 rd, rs1, rs2
	.insn r CUSTOM_0, 7, 0b0011100, \rd, \rs1, \rs2
.endm

.macro rv64_packlh rd, rs1, rs2
	.insn r CUSTOM_0, 7, 0b0000111, \rd, \rs1, \rs2
.endm

.macro CHACHA_HR a b c d
	chacha_add      \a, \a, \b
	chacha_xorrol16 \d, \d, \a
	chacha_add      \c, \c, \d
	chacha_xorrol12 \b, \b, \c
	chacha_add      \a, \a, \b
	chacha_xorrol08 \d, \d, \a
	chacha_add      \c, \c, \d
	chacha_xorrol07 \b, \b, \c
.endm

#define tt2 s8
#define tt3 s9
#define tt6 s10
#define tt7 s11

.option push
.option norvc
.balign 4
# Using the same order as the boring chacha arguments:
# a0 = uint8_t *out
# a1 = uint8_t *in
# a2 = size_t in_len
# a3 = uint8_t key[32]
# a4 = uint8_t nonce[12]
# a5 = uint32_t counter
.global chacha20_ise_v4
chacha20_ise_v4:
    addi    sp, sp, -60                    // Allocate stack space.

    sw      s0 , 0(sp)
    sw      s1 , 4(sp)
    sw      s2 , 8(sp)
    sw      s3 ,12(sp)
    sw      s4 ,16(sp)
    sw      s5 ,20(sp)
    sw      s6 ,24(sp)
    sw      s7 ,28(sp)
    sw      s8 ,32(sp)
    sw      s9 ,36(sp)
    sw      s10,40(sp)
    sw      s11,44(sp)
    sw      a0 ,48(sp)
    sw      a1 ,52(sp)

	# initialize vector state
	# Load 4*32 bit constant        
#	li t0, 0x3320646e61707865 # "C1, C0: expand 3" little endian
#	li t1, 0x6b20657479622d32 # "C3, C2: 2-byte k" little endian
	la    a6, ChaChaConstant
	ld t0, 0(a6)
	ld t1, 8(a6)

	# Load 8* 32 key
    ld t2, 0(a3)
	ld t3, 8(a3)
	ld t4,16(a3)
	ld t5,24(a3)

	# Load nonce & counter=0
	lw t6, 0(a4)
	slli t6, t6, 32
    add t6, t6, a5  # add counter
    lw tt2, 4(a4)
    lw a7, 8(a4)
	slli a7, a7, 32
    xor  a7, a7, tt2
encrypt_blocks:

    mv s0, t0
	mv s1, t1
	mv s2, t2
	mv s3, t3
	mv s4, t4
	mv s5, t5
	mv s6, t6
	mv s7, a7

	li a6, 10 # loop counter
round_loop:
        CHACHA_HR s0,s2,s4,s6   # QR(x[1]||x[0], x[5]||x[4], x[ 9]||x[ 8], x[13]||x[12]) - column 1 & 0 
        CHACHA_HR s1,s3,s5,s7   # QR(x[3]||x[2], x[7]||x[6], x[11]||x[10], x[15]||x[14]) - column 3 & 2

        rv64_packlh tt2, s2, s3 #  6, 5 
        rv64_packlh tt3, s3, s2 #  4, 7 
        rv64_packlh tt6, s7, s6 # 12,15
        rv64_packlh tt7, s6, s7 # 14,13
        #                               A          B            C              D
        CHACHA_HR s0,tt2,s5,tt6  # QR(x[1]||x[0], x[6]||x[5], x[11]||x[10], x[12]||x[15]) - column 1 & 0 
        CHACHA_HR s1,tt3,s4,tt7  # QR(x[3]||x[2], x[4]||x[7], x[ 9]||x[ 8], x[14]||x[13]) - column 3 & 2

        rv64_packlh s2, tt3, tt2 #  5, 4 
        rv64_packlh s3, tt2, tt3 #  7, 6 
        rv64_packlh s6, tt6, tt7 #  13,12
        rv64_packlh s7, tt7, tt6 #  15,14
	
	addi a6, a6, -1
	bnez a6, round_loop

	# Add in initial block values.
	chacha_add s0, s0, t0
	chacha_add s1, s1, t1
	chacha_add s2, s2, t2
	chacha_add s3, s3, t3
	chacha_add s4, s4, t4
	chacha_add s5, s5, t5
	chacha_add s6, s6, t6
	chacha_add s7, s7, a7

	# xor in state
	ld  tt2, 0(a1)
	xor tt2, tt2, s0
	sd  tt2, 0(a0)
	ld  tt2, 8(a1)
	xor tt2, tt2, s1
	sd  tt2, 8(a0)
	ld  tt2,16(a1)
	xor tt2, tt2, s2
	sd  tt2,16(a0)
	ld  tt2,24(a1)
	xor tt2, tt2, s3
	sd  tt2,24(a0)
	ld  tt2,32(a1)
    xor tt2, tt2, s4
	sd  tt2,32(a0)
	ld  tt2,40(a1)
    xor tt2, tt2, s5
	sd  tt2,40(a0)
	ld  tt2,48(a1)
    xor tt2, tt2, s6
	sd  tt2,48(a0)
	ld  tt2,56(a1)
    xor tt2, tt2, s7
	sd  tt2,56(a0)

#	sd  t0, 0(a0)
#	sd  t1, 8(a0)
#	sd  t2,16(a0)
#	sd  t3,24(a0)
#	sd  t4,32(a0)
#	sd  t5,40(a0)
#	sd  t6,48(a0)
#	sd  a7,56(a0)
 
	# update counters/pointers
	addi a0, a0, 64 # advance output pointer
	addi a1, a1, 64 # advance input pointer
	addi t6, t6, 1 # increment counter; crash if counter overflows
	addi a2, a2, -64 # decrement remaining bytes

	# loop again if we have remaining blocks
	bnez a2, encrypt_blocks

    lw      s0 , 0(sp)
    lw      s1 , 4(sp)
    lw      s2 , 8(sp)
    lw      s3 ,12(sp)
    lw      s4 ,16(sp)
    lw      s5 ,20(sp)
    lw      s6 ,24(sp)
    lw      s7 ,28(sp)
    lw      s8 ,32(sp)
    lw      s9 ,36(sp)
    lw      s10,40(sp)
    lw      s11,44(sp)
    lw      a0 ,48(sp)
    lw      a1 ,52(sp)
    
    addi    sp, sp, 60                    // de-Allocate stack space.

	ret

.section .data
.balign 8                 # align to 4 bytes
ChaChaConstant:
.word   0x61707865, 0x3320646e, 0x79622d32, 0x6b206574




