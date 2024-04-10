

#include <stdio.h>

int lower_one_mask(int n) {
	int w = sizeof(int) << 3;
	int mask = (unsigned)-1 >> (w - n);
	return mask;
}

/*
 * Do rotating left shift.  Assume 0 <= n < w
 * Examples when x = 0x12345678 and w = 32:
 *    n=4 -> 0x23456781, n=20 -> 0x67812345
 */
unsigned rotate_left(unsigned x, int n)
{
	int w = sizeof(int) << 3;
	if (n >= w) return 0;

	int low_bits = lower_one_mask(n) & ((unsigned)x>> (w-n));
	int high_bits = x << n;
	return high_bits | low_bits;
}

int main()
{
	int x = 0x12345678;
	int n = 4;
	printf("When x = 0x%X, it will be 0x%X after rotating left shift by %d.\n", x, rotate_left(x, n), n);

	n = 0;
	printf("When x = 0x%X, it will be 0x%X after rotating left shift by %d.\n", x, rotate_left(x, n), n);

	n = 20;
	printf("When x = 0x%X, it will be 0x%X after rotating left shift by %d.\n", x, rotate_left(x, n), n);
	return 0;
}
