#include <stdio.h>

/* Return 1 when x contains an odd number of 1s; 0 otherwise. Assume w=32 */
/*
0 ^ 0 = 0,
0 ^ 1 = 1,
1 ^ 1 = 0
*/
//两位中有奇数个(1个)1的时候, 该位的异或值为1.也就是我们用异或结果的一个位压缩了进行异或的两位的1的个数.
//我们发现这样移位一次只利用了每次第0位的异或结果, 有优化空间.
//我们可以同时利用多个异或结果, 并且这些异或结果所代表的位不能重叠.(因为最后需要有一位代表0~w - 1位中1的个数, 若有重叠就会导致某一位多计算一次).
//那么最大化且不重叠的利用办法就是一次右移一半, 像将这个数字"对折"一样.此时的异或结果中0~w / 2 - 1位中每一位分别代表第i位和第i + w / 2 - 1位中1的个数是否为奇数.
//每次右移一半直到只剩一位, 我们只需进行$log_2w$次异或和右移运算.代码见开头.
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