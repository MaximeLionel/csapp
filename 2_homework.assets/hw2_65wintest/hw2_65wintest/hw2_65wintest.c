#include <stdio.h>

/* Return 1 when x contains an odd number of 1s; 0 otherwise. Assume w=32 */
/*
0 ^ 0 = 0,
0 ^ 1 = 1,
1 ^ 1 = 0
*/
//��λ����������(1��)1��ʱ��, ��λ�����ֵΪ1.Ҳ�����������������һ��λѹ���˽���������λ��1�ĸ���.
//���Ƿ���������λһ��ֻ������ÿ�ε�0λ�������, ���Ż��ռ�.
//���ǿ���ͬʱ���ö�������, ������Щ������������λ�����ص�.(��Ϊ�����Ҫ��һλ����0~w - 1λ��1�ĸ���, �����ص��ͻᵼ��ĳһλ�����һ��).
//��ô����Ҳ��ص������ð취����һ������һ��, ���������"����"һ��.��ʱ���������0~w / 2 - 1λ��ÿһλ�ֱ�����iλ�͵�i + w / 2 - 1λ��1�ĸ����Ƿ�Ϊ����.
//ÿ������һ��ֱ��ֻʣһλ, ����ֻ�����$log_2w$��������������.�������ͷ.
int odd_ones(unsigned x)
{
	x ^= x >> 16; 
	x ^= x >> 8;  
	x ^= x >> 4;  
	x ^= x >> 2;  
	x ^= x >> 1;	  
	return x & 0x1;
}

int main()
{
	// 0x A = 0b 1010
	printf("Function odd_ones returns %d with input of 0x0A000000.\n", odd_ones(0x0a000000));
	// 0x B = 0b 1011
	printf("Function odd_ones returns %d with input of 0xB.\n", odd_ones(0xB));
	// 0x 4 = 0b 0100
	printf("Function odd_ones returns %d with input of 0x4.\n", odd_ones(0x4));
	return 0;
}