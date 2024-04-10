// P118.c
#include<stdio.h>
#include<limits.h>

/*Determine whether 2 arguments can be added without overflow.*/
/*If can add without overflow, return 1. Else return 0.*/
int uadd_ok(unsigned x, unsigned y)
{
  unsigned s = x + y;
  if(s < x) return 0;
  return 1;
}

int main()
{
  unsigned x0 = UINT_MAX;
  unsigned y0 = 0x10;
  printf("Test0:");
  printf("x0 = %u\ty0 = %u\n",x0,y0);
  printf("The return of uadd_ok:%d\n",uadd_ok(x0,y0));
  printf("\n");

  unsigned x1 = 0x10;
  unsigned y1 = 0x20;
  printf("Test1:");
  printf("x1 = %u\ty1 = %u\n",x1,y1);
  printf("The return of uadd_ok:%d\n",uadd_ok(x1,y1));
  return 0;
}
