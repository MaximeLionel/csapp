#include <stdio.h>

/* Return 1 when any odd bit of x equals 1; 0 otherwise. Assume w=32 */
int any_odd_one(unsigned x)
{
	// 0b 1010 1010 1010 1010 1010 1010 1010 1010
	unsigned mask = 0xAAAAAAAA;
	return (mask & x)&&1;
}

int main()
{
	printf("Function any_odd_one returns %d with input of 0xA.\n", any_odd_one(0xA));
	printf("Function any_odd_one returns %d with input of 0xB.\n", any_odd_one(0xB));
	printf("Function any_odd_one returns %d with input of 0x4.\n", any_odd_one(0x4));
	return 0;
}

