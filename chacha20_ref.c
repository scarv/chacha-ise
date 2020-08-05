
#include "chacha20.h"


void chacha20_qr(uint32_t g[4]) {
    uint32_t a = g[0];
    uint32_t b = g[1];
    uint32_t c = g[2];
    uint32_t d = g[3];
	a += b,  d ^= a,  d = ROTL(d,16);
	c += d,  b ^= c,  b = ROTL(b,12);
	a += b,  d ^= a,  d = ROTL(d, 8);
	c += d,  b ^= c,  b = ROTL(b, 7);
    g[0] = a;
    g[1] = b;
    g[2] = c;
    g[3] = d;
}

//! A vanilla implementation of the ChaCha20 block function.
void chacha20_block(uint32_t out[16], uint32_t const in[16])
{
	int i;
	uint32_t x[16];

	for (i = 0; i < 16; ++i)	
		x[i] = in[i];
	// 10 loops × 2 rounds/loop = 20 rounds
	//for (i = 0; i < CHACHA20_ROUNDS; i += 2) 
    {
        printf("---\n");
        printf("%08X %08X %08X %08X\n",x[0], x[4], x[ 8], x[12]);
        printf("%08X %08X %08X %08X\n",x[1], x[5], x[ 9], x[13]);
        printf("%08X %08X %08X %08X\n",x[2], x[6], x[10], x[14]);
        printf("%08X %08X %08X %08X\n",x[3], x[7], x[11], x[15]);
		// Odd round
		QR(x[0], x[4], x[ 8], x[12]); // column 0
		QR(x[1], x[5], x[ 9], x[13]); // column 1
		QR(x[2], x[6], x[10], x[14]); // column 2
		QR(x[3], x[7], x[11], x[15]); // column 3
        
        printf("---\n");
        printf("%08X %08X %08X %08X\n",x[0], x[4], x[ 8], x[12]);
        printf("%08X %08X %08X %08X\n",x[1], x[5], x[ 9], x[13]);
        printf("%08X %08X %08X %08X\n",x[2], x[6], x[10], x[14]);
        printf("%08X %08X %08X %08X\n",x[3], x[7], x[11], x[15]);
        
        printf("---\n");
        printf("%08X %08X %08X %08X\n",x[0], x[5], x[10], x[15]);
        printf("%08X %08X %08X %08X\n",x[1], x[6], x[11], x[12]);
        printf("%08X %08X %08X %08X\n",x[2], x[7], x[ 8], x[13]);
        printf("%08X %08X %08X %08X\n",x[3], x[4], x[ 9], x[14]);

		// Even round
		QR(x[0], x[5], x[10], x[15]); // diagonal 1 (main diagonal)
		//QR(x[1], x[6], x[11], x[12]); // diagonal 2
		//QR(x[2], x[7], x[ 8], x[13]); // diagonal 3
		//QR(x[3], x[4], x[ 9], x[14]); // diagonal 4
        
        printf("---\n");
        printf("%08X %08X %08X %08X\n",x[0], x[5], x[10], x[15]);
        printf("%08X %08X %08X %08X\n",x[1], x[6], x[11], x[12]);
        printf("%08X %08X %08X %08X\n",x[2], x[7], x[ 8], x[13]);
        printf("%08X %08X %08X %08X\n",x[3], x[4], x[ 9], x[14]);
	}
	for (i = 0; i < 16; ++i) {
		out[i] = x[i] ;//+ in[i];
    }
}
