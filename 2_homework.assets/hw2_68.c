#include<stdio.h>

int lower_one_mask(int n)
{
	int w = sizeof(int) << 3;
	if (n >= w) return 0;

	int mask = 1;
	for (int i = 1; i < n; i++)
	{
		mask = mask << 1;
		mask += 1;
	}
	return mask;
}

/*
int lower_one_mask(int n) {
	int w = sizeof(int) << 3;
	int mask = (unsigned)-1 >> (w - n);
	return mask;
}*/

int main()
{
	int n = 10;
	printf("Mask with least signficant %d bits set to 1 is 0x%X\n",n, lower_one_mask(n));
	return 0;
}