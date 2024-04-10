#include<stdio.h>
#include<math.h>
int main()
{
	int a = pow(2,24) + 1;
	float b;
	b = a;
	printf("%d casting from integer to float is %lf\n",a,b);
	a = pow(2,25) + 1;
	b = a;
	printf("%d casting from integer to float is %lf\n",a,b);
	return 0;
}
