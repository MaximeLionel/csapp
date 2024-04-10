
// P129.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<stdint.h>
 void* copy_elements(void *ele_src[], int ele_cnt, uint32_t ele_size) {
     /*
      * Allocate buffer for ele_cnt objects, each of ele_size bytes
      * and copy from locations designated by ele_src
      */
     uint64_t asize = ele_cnt*ele_size;
     size_t   st_size = (size_t)ele_cnt*ele_size;
     if(asize != st_size)
     {
	printf("Multiiplication overflow happens!\n");
	return NULL;
     }
     printf("No overflow happens.\n");
     void *result = malloc(asize); // overflow
     printf("The value of ele_cnt*ele_size: %u",ele_cnt*ele_size);
     if (result == NULL)
         /* malloc failed */
         return NULL;
     void *next = result;
     int i;
     for(i = 0; i < ele_cnt; i++)
     {
          /* Copy object i to destination */ 
          memcpy(next, ele_src[i], ele_size);
          /* Move pointer to next memory region */ 
          next += ele_size;
     }
     return result;
 }

int main()
{
  int cnt = 1048577; // 2^20 + 1
  uint32_t size = 4096; // 2^12
  int test_array[0x100] = {0};
  void* res = copy_elements((void*)test_array, cnt, size);
  
  return 0;
}
