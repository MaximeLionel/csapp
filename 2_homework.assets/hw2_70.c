#include<stdio.h>
/*
 * Return 1 when x can be represented as an n-bit, 2’s-complement
 * number; 0 otherwise
 * Assume 1 <= n <= w
 */
int fits_bits(int x, int n) {
	int test = x >> (n-1);
	return (test == 0) || (test == -1); // 2 cases: x >= 0 or x < 0;
}

/*
int fits_bits(int x, int n){
	int w = sizeof(int) << 3;
	int shift_count = w - n;
	return (x << shift_count >> shift_count) == x;
}*/

int main() {
	int x = 0b0100;
	int n = 3;
	if(fits_bits(x,n)) printf("0x%X can be represented as an %d - bit, 2’s - complement number\n",x,n);
	else printf("0x%X can NOT be represented as an %d - bit, 2’s - complement number\n", x, n);

	n = 4;
	if (fits_bits(x, n)) printf("0x%X can be represented as an %d - bit, 2’s - complement number\n", x, n);
	else printf("0x%X can NOT be represented as an %d - bit, 2’s - complement number\n", x, n);

	return 0;
}
