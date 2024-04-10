// P123.c
#include<stdio.h>
#include<limits.h>

int tadd_ok(int x, int y)
{
  int s = x + y;
  // If x>0,y>0 but s<=0, POSITIVE OVERFLOW
  if(x>0 && y>0 && s<=0) return 0;
  // If x<0,y<0 but s>=0, NEGATIVE OVERFLOW
  if(x<0 && y<0 && s>=0) return 0;
  return 1;
}

int main()
{
  int x0 = 0x7FFFFFFE;
  int y0 = 0x10;
  printf("Test0:");
  printf("x0 = %d\ty0 = %d\n",x0,y0);
  printf("The return of tadd_ok:%d\n\n",tadd_ok(x0,y0));
  
  int x1 = 0x10;
  int y1 = 0x20;
  printf("Test1:");
  printf("x1 = %d\ty1 = %d\n",x1,y1);
  printf("The return of tadd_ok:%d\n\n",tadd_ok(x1,y1));
  return 0;
}
