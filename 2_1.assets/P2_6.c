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
	char str_type[0x10] = {0};
	printf("Please choose the kind of conversion (int/float/pointer): ");
	scanf("%s",str_type);
	if(!strcmp(str_type,"int"))
	{
		int tgt_int;
		printf("Please input the target integer:");
		scanf("%d",&tgt_int);
		show_int(tgt_int);
	}
	else if(!strcmp(str_type,"float"))
	{
		float tgt_float;
		printf("Please input the target float:");
		scanf("%f",&tgt_float);
		show_float(tgt_float);
	}
	else if(!strcmp(str_type,"pointer"))
	{
		int val = 0;
		printf("Please input a number then its pointer will be calculated:");
		scanf("%d",&val);
		void* tgt_pointer = (void*)&val;
		show_pointer(tgt_pointer);
	}
	else
	{
		printf("Wrong inputs!\n");
	}
	return 0;
}