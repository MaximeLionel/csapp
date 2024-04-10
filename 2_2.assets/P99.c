#include <stdio.h>
#include <limits.h>

int main()
{
	short int v = -12345;
	unsigned short uv = (unsigned short)v;
	printf("v = %d, uv = %u\n", v, uv);

	
	unsigned u = UINT_MAX; 
	int tu = (int)u;
	printf("u = %u, tu = %d\n", u, tu);
}
