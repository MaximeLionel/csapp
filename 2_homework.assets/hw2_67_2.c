	#include<stdio.h>
	
	int int_size_is_32() 
	{ 
		int set_msb = 1 << 31; 
		int beyond_msb = set_msb << 1; 
		return set_msb && !beyond_msb; 
	}

	int main()
	{
		if(int_size_is_32())
		{
			printf("An int is 32 bits on this machine\n");
		}
		else 
		{
			printf("An int is NOT 32 bits on this machine\n");
		}
		return 0;
	}
