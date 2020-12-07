#define chacha_add(rd, rs1, rs2) asm volatile ( \
  ".insn r CUSTOM_0, 7, 0b11000, %0,%1,%2\n\t" :"=r"(rd):"r"(rs1),"r"(rs2));

#define chacha_xorrol16(rd, rs1, rs2) asm volatile ( \
  ".insn r CUSTOM_0, 7, 0b11001, %0,%1,%2\n\t" :"=r"(rd):"r"(rs1),"r"(rs2));

#define chacha_xorrol12(rd, rs1, rs2) asm volatile ( \
  ".insn r CUSTOM_0, 7, 0b11010, %0,%1,%2\n\t" :"=r"(rd):"r"(rs1),"r"(rs2));

#define chacha_xorrol08(rd, rs1, rs2) asm volatile ( \
  ".insn r CUSTOM_0, 7, 0b11011, %0,%1,%2\n\t" :"=r"(rd):"r"(rs1),"r"(rs2));

#define chacha_xorrol07(rd, rs1, rs2) asm volatile ( \
  ".insn r CUSTOM_0, 7, 0b11100, %0,%1,%2\n\t" :"=r"(rd):"r"(rs1),"r"(rs2));

#define HALF_ROUND(A, B, C, D) {          \
  chacha_add(A,A,B);  chacha_xorrol16(D,D,A); \
  chacha_add(C,C,D);  chacha_xorrol12(B,B,C); \
  chacha_add(A,A,B);  chacha_xorrol08(D,D,A); \
  chacha_add(C,C,D);  chacha_xorrol07(B,B,C); \
}
#define REPACK_ROW(r0,r1,s0,s1) { \
  rv64_packlh(r0,s0,s1); \
  rv64_packlh(r1,s1,s0); \
}
void chacha20_block(uint32_t out[16], uint32_t in[16])
{
  uint64_t * pin  = (uint64_t *)in;
  uint64_t * pout  = (uint64_t *)out;
  uint64_t t2,t3,t6,t7;

  uint64_t a0 = pin[0];      // x[ 1], x[ 0]
  uint64_t a1 = pin[1];      // x[ 3], x[ 2]  
  uint64_t a2 = pin[2];      // x[ 5], x[ 4]  
  uint64_t a3 = pin[3];      // x[ 7], x[ 6]
  uint64_t a4 = pin[4];      // x[ 9], x[ 8]  
  uint64_t a5 = pin[5];      // x[11], x[10]
  uint64_t a6 = pin[6];      // x[13], x[12]  
  uint64_t a7 = pin[7];      // x[15], x[14]

  for(int i = 0; i < CHACHA20_ROUNDS; i += 2) {                  
    HALF_ROUND(a0,a2,a4,a6); // column 1 & 0 
    HALF_ROUND(a1,a3,a5,a7); // column 3 & 2
    REPACK_ROW(t2,t3,a2,a3); //  5, 4, 7, 6-> 6, 5, 4, 7
    REPACK_ROW(t6,t7,a7,a6); // 13,12,15,14->12,15,14,13

    HALF_ROUND(a0,t2,a5,t6); // column 1 & 0 
    HALF_ROUND(a1,t3,a4,t7); // column 3 & 2
    REPACK_ROW(a2,a3,t3,t2); //  6, 5, 4, 7-> 5, 4, 7, 6
    REPACK_ROW(a6,a7,t6,t7); // 12,15,14,13->13,12,15,14
  }
  chacha_add(pout[0], a0, pin[0]);
  chacha_add(pout[1], a1, pin[1]);
  chacha_add(pout[2], a2, pin[2]);
  chacha_add(pout[3], a3, pin[3]);
  chacha_add(pout[4], a4, pin[4]);
  chacha_add(pout[5], a5, pin[5]);
  chacha_add(pout[6], a6, pin[6]);
  chacha_add(pout[7], a7, pin[7]);
}

