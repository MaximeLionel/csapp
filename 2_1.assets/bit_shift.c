#include <stdio.h>

void print_CharToBin(char num)
{
	char chars_list[0x2] = "01";
	char result[0x8] = "00000000";
	int length;	// the length of result array
			
	if(num == 0) printf("0b 0\n");

	// Calculate the result and store into result array
	for(int i = 0;num!=0;i++)
	{
		result[i] = chars_list[num%0x2];// repeatly divide input number by 16
						// record the remainder into result[i]
		num /= 0x2;	// get the quotient
	}

	printf("0b ");
	for(int i = 8; i >= 0; i--)
	{
		printf("%c",result[i]);
	}
	printf("\r\n");
}


void print_UIntToBin(unsigned int num)
{
	char chars_list[0x2] = "01";
	char result[0x20] = "00000000000000000000000000000000";
	int length;	// the length of result array
			
	if(num == 0) printf("0b 0\n");

	// Calculate the result and store into result array
	for(int i = 0;num!=0;i++)
	{
		result[i] = chars_list[num%0x2];// repeatly divide input number by 16
						// record the remainder into result[i]
		num /= 0x2;	// get the quotient
	}

	printf("0b ");
	for(int i = 0x20; i >= 0; i--)
	{
		printf("%c",result[i]);
	}
	printf("\r\n");
}



int main()
{
	char x = 0b10010101;
	printf("=== CHAR TEST ===\n");
	printf("The original number: ");
	print_CharToBin(x);
	printf("x << 4: ");
	print_CharToBin(x<<4);
	printf("x >> 4: ");
	print_CharToBin(x>>4);

	printf("\n");
	printf("=== INT TEST ===\n");
	unsigned int y = 0b10010101000000001111111100001111;
	printf("The original number: ");
	print_UIntToBin(y);
	printf("y << 4: ");
	print_UIntToBin(y<<4);
	printf("y >> 4: ");
	print_UIntToBin(y>>4);
	return 0;
}
