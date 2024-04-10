#include<stdio.h>

unsigned replace_byte (unsigned x, int n, unsigned char b)
{
	char* p = (char*)&x;
	*(p+n)=b;
	return x;
}

int main()
{
	// replace_byte(0x12345678, 2, 0xAB) --> 0x12AB5678
	printf("Replace the 3rd byte of 0x12345678 with 0xAB is: 0x%X\n",replace_byte(0x12345678, 2, 0xAB));
	// replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB
	printf("Replace the 1st byte of 0x12345678 with 0xAB is: 0x%X\n",replace_byte(0x12345678, 0, 0xAB));
	return 0;
}
