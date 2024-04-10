// P112.c
#include<stdio.h>

float sum_elements(float a[], unsigned length)
{
  int i;
  float result = 0;
  
  for(i = 0; i < length; i++) result += a[i];
  
  return result;
}

int main()
{
  float s[2] = {0,1};
  float sum = sum_elements(s, 0);
  
  return 0;
}
