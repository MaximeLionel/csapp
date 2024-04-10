// cast.c
#include <stdio.h> 
#include <string.h>

typedef unsigned char* byte_pointer; 

void show_bytes(byte_pointer start, size_t len) {
	int i;
	for(i = 0; i < len; i++) printf(" %.2x", start[i]);
	printf("\n"); 
} 

void show_int(int x) {
	show_bytes((byte_pointer) &x, sizeof(int)); 
} 
	
void show_float(float x) {
	show_bytes((byte_pointer) &x, sizeof(float)); 
} 
	
void show_pointer(void *x) {
	show_bytes((byte_pointer) &x, sizeof(void *)); 
}

void test_show_bytes(int val) { 
  	int ival 	= val; 
  	float fval 	= (float) ival;
	int*pval	= &ival;
	printf("Show int:");
	show_int(ival); 
	printf("Show float:");
  	show_float(fval); 
	printf("Show pointer:");
  	show_pointer(pval);
}


int main()
{
	show_bytes("mnopqr",strlen("mnopqr"));
	return 0;
}
