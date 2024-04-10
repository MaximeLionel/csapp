// P106.c
#include<stdio.h>

typedef unsigned char* byte_pointer; 

void show_bytes(byte_pointer start, size_t len) {
	int i;
	for(i = 0; i < len; i++) printf(" %.2x", start[i]);
	printf("\n"); 
} 

int main()
{
  short sx = -12345;
  unsigned short usx = sx;
  int x = sx;			// signed extension
  unsigned ux = usx; 		// zero extension

  /* signed extension */
  printf("Before signed extension: sx = %d \t",sx);
  show_bytes((byte_pointer)&sx, sizeof(short));
  printf("After signed extension: x = %d \t",x);
  show_bytes((byte_pointer)&x, sizeof(int));
  printf("\n");

  /* zero extension */
  printf("Before zero extension: usx = %d \t",usx);
  show_bytes((byte_pointer)&usx, sizeof(short));
  printf("After zero extension: ux = %u \t",ux);
  show_bytes((byte_pointer)&ux, sizeof(int));

  return 0;
}
