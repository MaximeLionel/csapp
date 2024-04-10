#include<stdio.h>

int main()
{
	int i_inputNum;	// store the input number
	char alphabet[0x10] = "0123456789ABCDEF";	// alphabet table for hex
	char result[0x100];
	int length;	// the length of result array

	printf("This program will convert a decimal you input to a hex accordinglly.\n");
	printf("Please input a decimal number:");
	scanf("%d",&i_inputNum);

	if(i_inputNum == 0) printf("0x0");

	// Calculate the result and store into result array
	for(int i = 0;i_inputNum!=0;i++)
	{
		result[i] = alphabet[i_inputNum%0x10];// repeatly divide input number by 16
						      // record the remainder into result[i]
		i_inputNum /= 0x10;	// get the quotient
		length = i;
	}	

	// Print the result
	printf("The hex result: 0x");
	for(int i = length; i >= 0; i--) printf("%c",result[i]);
	printf("\n");

	return 0;
}
