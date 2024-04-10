#include <stdio.h>

/*
* Generate mask indicating leftmost 1 in x. Assume w=32.
* For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
* If x = 0, then return 0.
*/
int leftmost_one(unsigned x) {
	x |= x >> 16; // To make sure all right bits after leftmost 1 are 1s
	x |= x >> 8;  
	x |= x >> 4;  
	x |= x >> 2;
	x |= x >> 1;
	return (x >> 1) + 0x1;
}

int main()
{
	// 0x A = 0b 1010
	printf("Function leftmost_one returns 0x %x with input of 0xA.\n", leftmost_one(0xA));
	// 0x B = 0b 1011
	printf("Function leftmost_one returns 0x %x with input of 0xB.\n", leftmost_one(0xB));
	// 0x 4 = 0b 0100
	printf("Function leftmost_one returns 0x %x with input of 0x4.\n", leftmost_one(0x4));
	return 0;
}