	#include<stdio.h>
	
	int bad_int_size_is_32() {
		/* Set most significant bit (msb) of 32-bit machine */
		int set_msb = 1 << 31;
		/* Shift past msb of 32-bit word */
		int beyond_msb = 1 << 32;
		/* set_msb is nonzero when word size >= 32
		   beyond_msb is zero when word size <= 32  */
	    return set_msb && !beyond_msb;
	}

	int main()
	{
		if(bad_int_size_is_32())
		{
			printf("An int is 32 bits on this machine\n");
		}
		else 
		{
			printf("An int is NOT 32 bits on this machine\n");
		}
		return 0;
	}
