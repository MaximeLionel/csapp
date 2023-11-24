#include <stdio.h>
#include <stdint.h>

int main()
{
	uint32_t x = 0x01234567;
	uint32_t* x_adr = &x;
	printf("The value of variable x is: 0x%08x \n",x);
	printf("The address of variable x is: %p\n",x_adr);
	printf("The 1st byte value of x address is: 0x%02x at address: %p\n",
			*(char*)x_adr,(char*)x_adr);
	printf("The 2nd byte value of x address is: 0x%02x at address: %p\n",
			*((char*)x_adr+1),(char*)x_adr+1);
	printf("The 3rd byte value of x address is: 0x%02x at address: %p\n",
			*((char*)x_adr+2),(char*)x_adr+2);
	printf("The 4th byte value of x address is: 0x%02x at address: %p\n",
			*((char*)x_adr+3),(char*)x_adr+3);
	return 0;
}
