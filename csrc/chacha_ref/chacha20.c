#include "chacha20.h"

//! A vanilla implementation of the ChaCha20 block function.
void chacha20_block(uint32_t out[16], uint32_t const in[16])
{
	int i;
	uint32_t x[16];

    // 16 instructions to load inputs.
	for (i = 0; i < 16; ++i) {
		x[i] = in[i];
    }

	//  10 loops × 2 rounds/loop = 20 rounds
    //  12 instructions per Quarter Round
    //  96 instructions per Double Round.
    // 960 instructions total in the loop.
	for (i = 0; i < CHACHA20_ROUNDS; i += 2) 
    {
		// Odd round
		QR(x[0], x[4], x[ 8], x[12]); // column 0
		QR(x[1], x[5], x[ 9], x[13]); // column 1
		QR(x[2], x[6], x[10], x[14]); // column 2
		QR(x[3], x[7], x[11], x[15]); // column 3

		// Even round
		QR(x[0], x[5], x[10], x[15]); // diagonal 1
		QR(x[1], x[6], x[11], x[12]); // diagonal 2
		QR(x[2], x[7], x[ 8], x[13]); // diagonal 3
		QR(x[3], x[4], x[ 9], x[14]); // diagonal 4
	}

    //
    //  3 instructions per loop iteration: Load + add + store.
    // 48 instructions total.
	for (i = 0; i < 16; ++i) {
		out[i] = x[i] + in[i];
    }
    
    // 1024 instructions total.
}
