// P136.c
#include <stdio.h>

int div16(int x)
{
  // (x<0? x+(1<<4)-1 : x)>>4
  int signed_bit = x >> 31;
  int bias = signed_bit & 0xF;
  return (x + bias)>>4;
}

int main()
{
  int x0 = 16, x1 = 31, x2 = 33;
  int y0 = -16, y1 = -31; 
  printf("%d/16=%d\n",x0,div16(x0));
  printf("%d/16=%d\n",x1,div16(x1));
  printf("%d/16=%d\n",x2,div16(x2));
  printf("%d/16=%d\n",y0,div16(y0));
  printf("%d/16=%d\n",y1,div16(y1));
  
  return 0;
}
