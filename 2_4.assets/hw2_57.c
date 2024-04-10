     // hw2_57.c
     #include<stdio.h> 
     
     typedef unsigned char* byte_pointer; 
     
     void show_bytes(byte_pointer start, size_t len) {
     	int i;
     	for(i = 0; i < len; i++) printf(" %.2x", start[i]);
     	printf("\n"); 
     } 
     
     void show_int(int x) {
     	show_bytes((byte_pointer)&x, sizeof(int)); 
     } 
     	
     void show_float(float x) {
     	show_bytes((byte_pointer)&x, sizeof(float)); 
     } 
     	
     void show_pointer(void *x) {
     	show_bytes((byte_pointer)&x, sizeof(void *)); 
     }

	 void show_short(short x) {
     	show_bytes((byte_pointer)&x, sizeof(short)); 
     }

	 void show_long(long x) {
     	show_bytes((byte_pointer)&x, sizeof(long)); 
     }

	 void show_double(double x) {
     	show_bytes((byte_pointer)&x, sizeof(double)); 
     }
     
     int main()
     {
	     short  s = 100;
	     long   l = 100;
	     double d = 100.0;

		 printf("Show short of %d:",s);
		 show_short(s);
		 printf("Show long of %ld:",l);
		 show_long(l);
		 printf("Show double of %lf:",d);
		 show_double(d);
		 return 0;
     }
