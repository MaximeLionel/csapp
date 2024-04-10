#include <stdio.h>

/* Return 1 when x contains an odd number of 1s; 0 otherwise. Assume w=32 */
int odd_ones(unsigned x)
{
    x ^= x >> 1;
	// odd number of 1s for i and i+1 bit of x

	x ^= x >> 2;
	// odd number of 1s for i and i+2 bit of x
	// Combined above, odd number of 1s for i,i+1,i+2,i+3 bit of x

	x ^= x >> 4;
	// odd number of 1s for i and i+4 bit of x
	// Combined above, odd number of 1s for i,i+1,i+2,i+3,i+4,i+5,i+6,i+7 bit of x

	x ^= x >> 8;
	// odd number of 1s for i and i+8 bit of x
	// Combined above, odd number of 1s for i,i+1,...,i+15 bit of x

	x ^= x >> 16;
	// odd number of 1s for i and i+16 bit of x
	// Combined above, odd number of 1s for i,i+1,...,i+31 bit of x

	return x&0x1;
}

int main()
{
	// 0x A = 0b 1010
	printf("Function odd_ones returns %d with input of 0xA.\n", odd_ones(0xA));
	// 0x B = 0b 1011
	printf("Function odd_ones returns %d with input of 0xB.\n", odd_ones(0xB));
	// 0x 4 = 0b 0100
	printf("Function odd_ones returns %d with input of 0x4.\n", odd_ones(0x4));
	return 0;
}
