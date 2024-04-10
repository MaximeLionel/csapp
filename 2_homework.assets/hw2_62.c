#include<stdio.h>
#include<stdbool.h>

bool int_shifts_are_arithmetic()
{
	int sample = 0x80000000;// if arithmetic, 0x80000000>>31 will be 0xFFFFFFFF
	printf("%X\n",sample>>31);
	return !~(sample >> 31);
}

int main()
{
	if(int_shifts_are_arithmetic())
		printf("This is a machine using arithmetic right shift.\n");
	else
		printf("This is NOT a machine using arithmetic right shift.\n");
	return 0;
}
