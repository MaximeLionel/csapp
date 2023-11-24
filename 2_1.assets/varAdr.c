#include <stdio.h>

int main()
{
	int x = 100;
	printf("The address of x: %p\n",&x);
	printf("The size of var x: %ld bytes\n",sizeof(x));
	return 0;
}
