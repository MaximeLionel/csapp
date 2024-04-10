#include <stdio.h>
#include <string.h>

typedef unsigned char* byte_pointer; 

void show_bytes(byte_pointer start, size_t len) {
	int i;
	printf("Show %ld bytes from address: %p\n",len,start);
	for(i = 0; i < len; i++) 
		printf("\t%p: %.2x\n",start+i, start[i]);
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

int main()
{
	show_bytes("12345",6);

	return 0;
}
