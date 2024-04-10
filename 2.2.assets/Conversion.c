#include<stdio.h>

int main()
{
	int x0 = -100;
	unsigned int y0 = (unsigned int)x0;
	printf("\nConvert int x0: %d to unsigned: y0:%u", x0,y0);
	printf("\nx0 in hex: %x",x0);
	printf("\ny0 in hex: %x",y0);

	unsigned int x1 = 0xFFFFFFDD;
	int y1 = (int)x1;
	printf("\nConvert unsigned x1: %u to int: y1:%d", x1,y1);
	printf("\nx1 in hex: %x",x1);
	printf("\ny1 in hex: %x",y1);

	short int v = -12345;
	unsigned short uv = (unsigned short)v;
	printf("\n\nv = %d, uv = %u\n",v,uv);
	printf("\nv in hex: %x",v);
	printf("\nuv in hex: %x\n",uv);

	unsigned u1 = 4294967295u;
	int tu1 = (int)u;
	printf("\n\nu1 = %u, tu1 = %d\n",u1,tu1);
	printf("\nu1 in hex: %x",u1);
	printf("\ntu1 in hex: %x\n",tu1);

	return 0;
}
