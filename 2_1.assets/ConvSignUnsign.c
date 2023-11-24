#include<stdio.h>
#include<limits.h>

int main()
{
	int x = -12345;
	unsigned int y = x;
	printf("INT_MAX value in decimal: %d	in hex: %x\n",INT_MAX,INT_MAX);
	printf("UINT_MAX value in decimal: %d	in hex: %x\n",UINT_MAX,UINT_MAX);
	printf("%d converts to unsigned: %x\n",y);

	return 0;
}
