#include<stdio.h>

void inplace_swap(int* x, int* y)
{
	*y = *x ^ *y;
	*x = *x ^ *y;
	*y = *x ^ *y;
}

void reverse_array(int a[], int cnt)
{
	int first, last;
	for(first = 0, last = cnt - 1; first < last; first++, last--)
		inplace_swap(&a[first],&a[last]);
}


int main()
{
	int test[] = {0,1,2,3,4};
	reverse_array(test,5);
	for(int i = 0; i < sizeof(test)/sizeof(test[0]);i++)
		printf("%d ",test[i]);
	return 0;
}
