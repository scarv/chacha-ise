
#include <stdint.h>

#ifndef __CHACHA_REF_H__
#define __CHACHA_REF_H__

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) (			\
	a += b,  d ^= a,  d = ROTL(d,16),	\
	c += d,  b ^= c,  b = ROTL(b,12),	\
	a += b,  d ^= a,  d = ROTL(d, 8),	\
	c += d,  b ^= c,  b = ROTL(b, 7))

#define CHACHA20_ROUNDS 20

void chacha20_qr(uint32_t io[4]);
 
//! The chacha20 block function.
void chacha20_block(uint32_t out[16], uint32_t const in[16]);

#endif
