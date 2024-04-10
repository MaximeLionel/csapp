#include<stdio.h>

	/* 指针法检测 */
	int checkLitEndMachByPointer()
	{
		int a = 1;
		char* p = (char*)&a;

		if(1 == *p) return 1;
		return 0;
	}

	/* 联合体法检测 */
	int checkLitEndMachByUnion()
	{
		union{
			char c;
			int  i;
		}u;
		
		u.i = 1;

		if(1 == u.c) return 1;
		return 0;
	}

	int main()
	{
		if(checkLitEndMachByPointer())
		{
			printf("Using pointer check little endian, this machine is little endian type.\n");
		}
		else 
		{
			printf("Using pointer check little endian, this machine is big endian type.\n");
		}

		if(checkLitEndMachByUnion())
		{
			printf("Using union check little endian, this machine is little endian type.\n");
		}
		else 
		{
			printf("Using union check little endian, this machine is big endian type.\n");
		}
		return 0;
	}

