#include<stdio.h>
int main()
{
  int x = -1;
  unsigned u = 2147483648;

  printf("x = %u in unsigned format\nx = %d in signed format\n\n", x, x);
  printf("u = %u in unsigned format\nu = %d in signed format\n\n", u, u);
  
  return 0;
}