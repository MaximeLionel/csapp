## 2.55
Compile and run the sample code that uses show_bytes (file show-bytes.c) on different machines to which you have access. Determine the byte orderings used by these machines.
**Solution**: do it by yourself!

## 2.56
Try running the code for show_bytes for different sample values.
**Solution**: do it by yourself!

## 2.57
Write procedures show_short, show_long, and show_double that print the byte representations of C objects of types short, long, and double, respectively. Try these out on several machines.
Solution:
```c
     // hw2_57.c
     #include<stdio.h> 
     
     typedef unsigned char* byte_pointer; 
     
     void show_bytes(byte_pointer start, size_t len) {
     	int i;
     	for(i = 0; i < len; i++) printf(" %.2x", start[i]);
     	printf("\n"); 
     } 
     
     void show_int(int x) {
     	show_bytes((byte_pointer)&x, sizeof(int)); 
     } 
     	
     void show_float(float x) {
     	show_bytes((byte_pointer)&x, sizeof(float)); 
     } 
     	
     void show_pointer(void *x) {
     	show_bytes((byte_pointer)&x, sizeof(void *)); 
     }

	 void show_short(short x) {
     	show_bytes((byte_pointer)&x, sizeof(short)); 
     }

	 void show_long(long x) {
     	show_bytes((byte_pointer)&x, sizeof(long)); 
     }

	 void show_double(double x) {
     	show_bytes((byte_pointer)&x, sizeof(double)); 
     }
     
     int main()
     {
	     short  s = 100;
	     long   l = 100;
	     double d = 100.0;

		 printf("Show short of %d:",s);
		 show_short(x);
		 printf("Show long of %ld:",l);
		 show_short(x);
		 printf("Show double of %lf:",d);
		 show_short(x);
		 return 0;
     }
```

## 2.58
Write a procedure is_little_endian that will return 1 when compiled and run on a little-endian machine, and will return 0 when compiled and run on a big-endian machine. This program should run on any machine, regardless of its word size.
**Solution**:
```c
// hw2_58.c
#include<stdio.h>
	/* 指针法检测 */
	int checkLitEndMachByPointer()
	{
		int a = 1;
		char* p = (char*)&a;

		if(1 == *p) return 1;
		return 0;
	}

	/* 联合体法检测 */
	int checkLitEndMachByUnion()
	{
		union{
			char c;
			int  i;
		}u;
		
		u.i = 1;

		if(1 == u.c) return 1;
		return 0;
	}

	int main()
	{
		if(checkLitEndMachByPointer())
		{
			printf("Using pointer check little endian, this machine is little endian type.\n");
		}
		else 
		{
			printf("Using pointer check little endian, this machine is big endian type.\n");
		}

		if(checkLitEndMachByUnion())
		{
			printf("Using union check little endian, this machine is little endian type.\n");
		}
		else 
		{
			printf("Using union check little endian, this machine is big endian type.\n");
		}
		return 0;
	}

```

![[2_homework.assets/image-20240103153431948.png]]

## 2.59
Write a C expression that will yield a word consisting of the least significant byte of x and the remaining bytes of y. For operands x = 0x89ABCDEF and y = 0x76543210, this would give 0x765432EF.
**Solution**:
(x&0x000000FF)｜(y&0xFFFFFF00)
0x000000EF | 0x76543200 = 0x765432EF

## 2.60
Suppose we number the bytes in a w-bit word from 0 (least significant) to w/8 − 1 (most significant). Write code for the following C function, which will return an unsigned value in which byte i of argument x has been replaced by byte b:
```C
unsigned replace_byte (unsigned x, int i, unsigned char b);
```
Here are some examples showing how the function should work:
replace_byte(0x12345678, 2, 0xAB) --> 0x12AB5678
replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB

**Solution**:
```c
#include<stdio.h>

unsigned replace_byte (unsigned x, int n, unsigned char b)
{
	char* p = &x; // *p - LSB, *(p+1), ... *(p+3) - MSB
	*(p+n)=b;
	return x;
}

int main()
{
	// replace_byte(0x12345678, 2, 0xAB) --> 0x12AB5678
	printf("Replace the 3rd byte of 0x12345678 with 0xAB is: 0x%X\n",replace_byte(0x12345678, 2, 0xAB));
	// replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB
	printf("Replace the 1st byte of 0x12345678 with 0xAB is: 0x%X\n",replace_byte(0x12345678, 0, 0xAB));
	return 0;
}
```
![[2_homework.assets/image-20240104160405394.png]]

## 2.61
Write C expressions that evaluate to 1 when the following conditions are true and to 0 when they are false. Assume x is of type int.
    
    A. Any bit of x equals 1.
    B. Any bit of x equals 0.
    C. Any bit in the least significant byte of x equals 1.
	D. Any bit in the least significant byte of x equals 0.
Your code should follow the bit-level integer coding rules (page 164), with the additional restriction that you may not use equality (\==) or inequality (!=) tests.

**Solution**:
A. !(~x)
B. !x
C. !~(x&0xFF)
D. !(x&0xFF)

## 2.62
Write a function int_shifts_are_arithmetic() that yields 1 when run on a machine that uses arithmetic right shifts for data type int and yields 0 otherwise. Your code should work on a machine with any word size. Test your code on several machines.
**Solution**:
```C
#include<stdio.h>

bool int_shifts_are_arithmetic()
{
	return !~(0x80 >> 7);// if arithmetic, 0b 1000 000 >>7 will be 0b 1111 1111, otherwise, it will be 0b 0000 0001
}

int main()
{
	if(int_shifts_are_arithmetic())
		printf("This is a machine using arithmetic right shift.");
	else
		printf("This is NOT a machine using arithmetic right shift.");
	return 0;
}
```

![[2_homework.assets/image-20240105154036003.png]]
![[2_homework.assets/image-20240105154108139.png]]

## 2.63
Fill in code for the following C functions. Function srl performs a logical right shift using an arithmetic right shift (given by value xsra), followed by other operations not including right shifts or division. Function sra performs an arithmetic right shift using a logical right shift (given by value xsrl), followed by other operations not including right shifts or division. You may use the computation 8$\times$sizeof(int) to determine w, the number of bits in data type int. The shift amount k can range from 0 to w − 1.
```c
unsigned srl(unsigned x, int k) {
    /* Perform shift arithmetically */
    unsigned xsra = (int) x >> k;
。。。。。。


}

int sra(int x, int k) {
    /* Perform shift logically */
    int xsrl = (unsigned) x >> k;

. . . . . .

}
```

**Solution**:
```c
#include <stdio.h>

/* Perform right shift logically */
unsigned srl(unsigned x, int k) {
    unsigned xsra = (int)x >> k;  // right shift arithmetically
    unsigned w = sizeof(int) * 8; // bit width of integer
    unsigned mask = ~(0xFFFFFFFF << (w-k)); // 0b 00...0 11...1
    return xsra & mask;
}

/* Perform right shift arithmetically */
int sra(int x, int k) {
    int xsrl = (unsigned)x >> k; // right shift arithmetically
    unsigned w = sizeof(int) * 8; // bit width of integer
    unsigned mask = 0xFFFFFFFF << (w - k); // 0b 11...1 00...0
    return xsrl | mask;

}

int main()
{
    unsigned test1 = 0x80000000;
    printf("0x80000000 right shift logically for 3 bits is: 0x %X\n", srl(test1, 3));
    printf("0x80000000 right shift logically for 8 bits is: 0x %X\n", srl(test1, 8));
    printf("0x80000000 right shift logically for 16 bits is: 0x %X\n", srl(test1, 16));
    printf("\n");
    printf("0x80000000 right shift arithmetically for 3 bits is: 0x %X\n", sra(test1, 3));
    printf("0x80000000 right shift arithmetically for 8 bits is: 0x %X\n", sra(test1, 8));
    printf("0x80000000 right shift arithmetically for 16 bits is: 0x %X\n", sra(test1, 16));
    return 0;
}

```

## 2.64
Write code to implement the following function:

```c
/* Return 1 when any odd bit of x equals 1; 0 otherwise.
   Assume w=32 */

int any_odd_one(unsigned x);
```

Your function should follow the bit-level integer coding rules (page 164), except that you may assume that data type int has w = 32 bits.

**Solution**:
```c
#include <stdio.h>

/* Return 1 when any odd bit of x equals 1; 0 otherwise. Assume w=32 */
int any_odd_one(unsigned x)
{
	// 0b 1010 1010 1010 1010 1010 1010 1010 1010
	unsigned mask = 0xAAAAAAAA;
	return (mask & x)&&1;
}

int main()
{
	printf("Function any_odd_one returns %d with input of 0xA.\n", any_odd_one(0xA));
	printf("Function any_odd_one returns %d with input of 0xB.\n", any_odd_one(0xB));
	printf("Function any_odd_one returns %d with input of 0x4.\n", any_odd_one(0x4));
	return 0;
}
```

## 2.65
Write code to implement the following function:
```c
/* Return 1 when x contains an odd number of 1s; 0 otherwise.
   Assume w=32 */

int odd_ones(unsigned x);
```
Your function should follow the bit-level integer coding rules (page 164), except that you may assume that data type int has w = 32 bits.

Your code should contain a total of at most 12 arithmetic, bitwise, and logical operations.

**Solution**:
To find a rule:

| unsigned x (w=2) | x>>1 | x^x>>1 | odd number of 1s |
| ---- | ---- | ---- | :--: |
| 00 | 00 | 00 | False |
| 01 | 00 | 01 | True |
| 10 | 11 | 01 | True |
| 11 | 11 | 00 | False |

| unsigned x (w=4) | x>>1 | x=x^x>>1 | x>>2 | x=x^x>>2 | odd number of 1s |
| ---- | ---- | ---- | ---- | ---- | :--: |
| 0000 | 0000 | 0000 | 0000 | 0000 | False |
| 0001 | 0000 | 0001 | 0000 | 0001 | True |
| 0010 | 0001 | 0011 | 0000 | 0011 | True |
| 0011 | 0001 | 0010 | 0000 | 0010 | False |
| 0100 | 0010 | 0110 | 0001 | 0111 | True |
| 0101 | 0010 | 0111 | 0001 | 0110 | False |
| 0110 | 0011 | 0101 | 0001 | 0100 | False |
| 0111 | 0011 | 0100 | 0001 | 0101 | True |
| 1000 | 0100 | 1100 | 0011 | 1111 | True |
| 1001 | 0100 | 1101 | 0011 | 1110 | False |
| 1010 | 0101 | 1111 | 0011 | 1100 | False |
| 1011 | 0101 | 1110 | 0011 | 1101 | True |
| 1100 | 0110 | 1010 | 0010 | 1000 | False |
| 1101 | 0110 | 1011 | 0010 | 1001 | True |
| 1110 | 0111 | 1001 | 0010 | 1011 | True |
| 1111 | 0111 | 1000 | 0010 | 1010 | False |

| ux (w=8) | x>>1 | x=x^x>>1 | x>>2 | x=x^x>>2 | x>>4 | x=x^x>>4 | odd 1s? |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | False |
| 0000 0001 | 0000 0000 | 0000 0001 | 0000 0000 | 0000 0001 | 0000 0000 | 0000 0001 | True |
| ... | ... | ... | ... | ... | ... | ... | ... |
| 1111 1110 | 0111 1111 | 1000 0001 | 0010 0000 | 1010 0001 | 0000 1010 | 1010 1011 | True |
| 1111 1111 | 0111 1111 | 1000 0000 | 0010 0000 | 1010 0000 | 0000 1010 | 1010 1010 | False |
To summarize and deduce logically:
1. Bit 0 after executing the code below shows if bit i and bit i+1 contain odd number of 1s.
	```c
	x ^= x >> 1;
	```
2. Bit i after executing the code below shows if bit i and bit i+1 contain odd number of 1s.
	```c
	x ^= x >> 1;
	```
3. Bit 0 after executing the code below shows if x (w=4) contains odd number of 1s.
	```c
	x ^= x >> 1;
	x ^= x >> 2;
	```
4. Bit i of x after executing the code below shows if ==bit i, bit i+1, bit i+2, bit i+3== contains odd number of 1s.
	```C
	x ^= x >> 1;
	x ^= x >> 2;
	```
5. Bit i of x after executing the code below shows if ==bit i, bit i+1, ..., bit i+7== contains odd number of 1s.
	```C
	x ^= x >> 1;
	x ^= x >> 2;
	x ^= x >> 4;
	```
6. Bit i of x after executing the code below shows if ==bit i, bit i+1, ..., bit i+31== contains odd number of 1s.
	```C
	x ^= x >> 1;
	x ^= x >> 2;
	x ^= x >> 4;
	x ^= x >> 8;
	x ^= x >> 16;
	```
7. Bit 0 of x (w=32) after executing the code below shows if ==bit 0, bit 1, ..., bit 31== contains odd number of 1s, finally.
8. According to the result above, we write the code below:

```c
// hw2_65.c
#include <stdio.h>

/* Return 1 when x contains an odd number of 1s; 0 otherwise. Assume w=32 */
int odd_ones(unsigned x)
{
    x ^= x >> 1;
	// odd number of 1s for i and i+1 bit of x

	x ^= x >> 2;
	// odd number of 1s for i and i+2 bit of x
	// Combined above, odd number of 1s for i,i+1,i+2,i+3 bit of x

	x ^= x >> 4;
	// odd number of 1s for i and i+4 bit of x
	// Combined above, odd number of 1s for i,i+1,i+2,i+3,i+4,i+5,i+6,i+7 bit of x

	x ^= x >> 8;
	// odd number of 1s for i and i+8 bit of x
	// Combined above, odd number of 1s for i,i+1,...,i+15 bit of x

	x ^= x >> 16;
	// odd number of 1s for i and i+16 bit of x
	// Combined above, odd number of 1s for i,i+1,...,i+31 bit of x

	return x&0x1;
}

int main()
{
	// 0x A = 0b 1010
	printf("Function odd_ones returns %d with input of 0xA.\n", odd_ones(0xA));
	// 0x B = 0b 1011
	printf("Function odd_ones returns %d with input of 0xB.\n", odd_ones(0xB));
	// 0x 4 = 0b 0100
	printf("Function odd_ones returns %d with input of 0x4.\n", odd_ones(0x4));
	return 0;
}
```


## 2.66
Write code to implement the following function:
```c
/* 

* Generate mask indicating leftmost 1 in x. Assume w=32.

* For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.

* If x = 0, then return 0.

*/

int leftmost_one(unsigned x);
```

Your function should follow the bit-level integer coding rules (page 164), except that you may assume that data type int has w = 32 bits.

Your code should contain a total of at most 15 arithmetic, bitwise, and logical operations.

Hint: First transform x into a bit vector of the form [0 ... 011 ... 1]

**Solution**:
According to the hint, what we want is to transform x into a bit vector of the form [0 ... 011 ... 1], which means all 0s on the left of leftmost 1 but all 1s on the right of leftmost 1.
We will see '|' operation to make sure replicate the leftmost 1 to all bits on the right of leftmost 1.
```c
// hw2_66.c
#include <stdio.h>

/*
* Generate mask indicating leftmost 1 in x. Assume w=32.
* For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
* If x = 0, then return 0.
*/
int leftmost_one(unsigned x) {
	x |= x >> 16; // To make sure all right bits after leftmost 1 are 1s
	x |= x >> 8;  
	x |= x >> 4;  
	x |= x >> 2;
	x |= x >> 1;
	return (x >> 1) + 0x1;
}

int main()
{
	// 0x A = 0b 1010
	printf("Function leftmost_one returns 0x %x with input of 0xA.\n", leftmost_one(0xA));
	// 0x B = 0b 1011
	printf("Function leftmost_one returns 0x %x with input of 0xB.\n", leftmost_one(0xB));
	// 0x 4 = 0b 0100
	printf("Function leftmost_one returns 0x %x with input of 0x4.\n", leftmost_one(0x4));
	return 0;
}
```


## 2.67
You are given the task of writing a procedure int_size_is_32() that yields 1 when run on a machine for which an int is 32 bits, and yields 0 otherwise. You are not allowed to use the sizeof operator. Here is a first attempt:

```c
/* The following code does not run properly on some machines */

int bad_int_size_is_32() {
	/* Set most significant bit (msb) of 32-bit machine */
	int set_msb = 1 << 31;
	/* Shift past msb of 32-bit word */
	int beyond_msb = 1 << 32;
	/* set_msb is nonzero when word size >= 32
	   beyond_msb is zero when word size <= 32  */
    return set_msb && !beyond_msb;
}
```

When compiled and run on a 32-bit SUN SPARC, however, this procedure returns 0. The following compiler message gives us an indication of the problem:
```
	warning: left shift count >= width of type
```
1. In what way does our code fail to comply with the C standard?
    
2. Modify the code to run properly on any machine for which data type int is at least 32 bits.
    
3. Modify the code to run properly on any machine for which data type int is at least 16 bits.

**Solution**:
1. When the shift count is greater or equal to the width of type, the behavior is undefined.
	```c
	#include<stdio.h>
	
	int bad_int_size_is_32() {
		/* Set most significant bit (msb) of 32-bit machine */
		int set_msb = 1 << 31;
		/* Shift past msb of 32-bit word */
		int beyond_msb = 1 << 32;
		/* set_msb is nonzero when word size >= 32
		   beyond_msb is zero when word size <= 32  */
	    return set_msb && !beyond_msb;
	}

	int main()
	{
		if(bad_int_size_is_32())
		{
			printf("An int is 32 bits on this machine");
		}
		else 
		{
			printf("An int is NOT 32 bits on this machine");
		}
		return 0;
	}
	```
2. code
	```c	
	#include<stdio.h>
	
	int int_size_is_32() 
	{ 
		int set_msb = 1 << 31; 
		int beyond_msb = set_msb << 1; 
		return set_msb && !beyond_msb; 
	}

	int main()
	{
		if(int_size_is_32())
		{
			printf("An int is 32 bits on this machine\n");
		}
		else 
		{
			printf("An int is NOT 32 bits on this machine\n");
		}
		return 0;
	}
	```
3. code
	```c
		int int_size_is_32_for_16bit() 
		{ 
			int set_msb = 1 << 15 << 15 << 1; 
			int beyond_msb = set_msb << 1; 
			return set_msb && !beyond_msb; 
		}
	```

## 2.68
Write code for a function with the following prototype:
```c
/*
 * Mask with least signficant n bits set to 1
 * Examples: n = 6 --> 0x3F, n = 17 --> 0x1FFFF
 * Assume 1 <= n <= w
 */

int lower_one_mask(int n);
```

Your function should follow the bit-level integer coding rules (page 164). Be careful of the case n = w.

**Solution**:
2 ideas: shift left and shift right.
```c
#include<stdio.h>

int lower_one_mask(int n)
{
	int w = sizeof(int) << 3;
	if (n >= w) return 0;

	int mask = 1;
	for (int i = 1; i < n; i++)
	{
		mask = mask << 1;
		mask += 1;
	}
	return mask;
}

/*
int lower_one_mask(int n) {
	int w = sizeof(int) << 3;
	int mask = (unsigned)-1 >> (w - n);
	return mask;
}*/

int main()
{
	int n = 10;
	printf("Mask with least signficant %d bits set to 1 is 0x%X\n",n, lower_one_mask(n));
	return 0;
}
```

## 2.69
Write code for a function with the following prototype:

```c
/*
 * Do rotating left shift.  Assume 0 <= n < w
 * Examples when x = 0x12345678 and w = 32:
 *    n=4 -> 0x23456781, n=20 -> 0x67812345
 */

unsigned rotate_left(unsigned x, int n);
```
Your function should follow the bit-level integer coding rules (page 164). Be careful of the case n = 0.

**Solution**:
```c
#include <stdio.h>

int lower_one_mask(int n) {
	int w = sizeof(int) << 3;
	int mask = (unsigned)-1 >> (w - n);
	return mask;
}

/*
 * Do rotating left shift.  Assume 0 <= n < w
 * Examples when x = 0x12345678 and w = 32:
 *    n=4 -> 0x23456781, n=20 -> 0x67812345
 */
unsigned rotate_left(unsigned x, int n)
{
	int w = sizeof(int) << 3;
	if (n >= w) return 0;

	int low_bits = lower_one_mask(n) & ((unsigned)x>> (w-n));
	int high_bits = x << n;
	return high_bits | low_bits;
}

int main()
{
	int x = 0x12345678;
	int n = 4;
	printf("When x = 0x%X, it will be 0x%X after rotating left shift by %d.\n", x, rotate_left(x, n), n);

	n = 0;
	printf("When x = 0x%X, it will be 0x%X after rotating left shift by %d.\n", x, rotate_left(x, n), n);

	n = 20;
	printf("When x = 0x%X, it will be 0x%X after rotating left shift by %d.\n", x, rotate_left(x, n), n);
	return 0;
}
```

## 2.70 
Write code for the function with the following prototype:
```c

/*
 * Return 1 when x can be represented as an n-bit, 2’s-complement
 * number; 0 otherwise
 * Assume 1 <= n <= w
 */

int fits_bits(int x, int n);
```
Your function should follow the bit-level integer coding rules (page 164).
**Solution**:
```c
#include<stdio.h>
/*
 * Return 1 when x can be represented as an n-bit, 2’s-complement
 * number; 0 otherwise
 * Assume 1 <= n <= w
 */
int fits_bits(int x, int n) {
	int test = x >> (n-1);
	return (test == 0) || (test == -1); // 2 cases: x >= 0 or x < 0;
}

/*
int fits_bits(int x, int n){
	int w = sizeof(int) << 3;
	int shift_count = w - n;
	return (x << shift_count >> shift_count) == x;
}*/

int main() {
	int x = 0b0100;
	int n = 3;
	if(fits_bits(x,n)) printf("0x%X can be represented as an %d - bit, 2’s - complement number\n",x,n);
	else printf("0x%X can NOT be represented as an %d - bit, 2’s - complement number\n", x, n);

	n = 4;
	if (fits_bits(x, n)) printf("0x%X can be represented as an %d - bit, 2’s - complement number\n", x, n);
	else printf("0x%X can NOT be represented as an %d - bit, 2’s - complement number\n", x, n);

	return 0;
}
```


## 2.71 
You just started working for a company that is implementing a set of procedures to operate on a data structure where 4 signed bytes are packed into a 32-bit unsigned. Bytes within the word are numbered from 0 (least significant) to 3(most significant). You have been assigned the task of implementing a function for a machine using two’s-complement arithmetic and arithmetic right shifts with the following prototype:
```c
/* Declaration of data type where 4 bytes are packed
   into an unsigned */
typedef unsigned packed_t;

/* Extract byte from word.  Return as signed integer */
int xbyte(packed_t word, int bytenum);
```

That is, the function will extract the designated byte and sign extend it to be a 32-bit int.

Your predecessor (who was fired for incompetence) wrote the following code:
```C
/* Failed attempt at xbyte */
int xbyte(packed_t word, int bytenum)
{
    return (word >> (bytenum << 3)) & 0xFF;
}
```

A. What is wrong with this code?

B. Give a correct implementation of the function that uses only left and right shifts, along with one subtraction.

**Solution**:
A. Failed at negative number, because packed_it is unsigned int which means only logical right shifts happen. Thus the function cannot return as signed integer.
B.
```c
#include<stdio.h>

/* Declaration of data type where 4 bytes are packed
   into an unsigned */
typedef unsigned packed_t;

/* Failed attempt at xbyte */
int xbyte(packed_t word, int bytenum)
{
    return (word >> (bytenum << 3)) & 0xFF;
}

/* Correct xbyte*/
int correct_xbyte(packed_t word, int bytenum)   // suppose word = 0xFFF80000 bytenum = 2
{
    int w = sizeof(int) << 3;       // w = 32
    int temp_shift = word << (w - (bytenum + 1) << 3);  // temp_shift = 0xF8000000
    int result = temp_shift >> (w - 1 << 3);            // result = 0xFFFFFFF8
    return result;
}

int main()
{
    packed_t packedword = -10;
    int bytenum = 0;
    printf("xbytes: extract bytenumber%d from word 0x%08X.  Return 0x%08X as signed integer.\n",bytenum,(int)packedword, xbyte(packedword,bytenum));
    printf("correct_xbyte: extract bytenumber%d from word 0x%08X.  Return 0x%08X as signed integer.\n", bytenum, (int)packedword, correct_xbyte(packedword, bytenum));
    packedword = 0xFFF80000;
    bytenum = 2;
    printf("correct_xbyte: extract bytenumber%d from word 0x%08X.  Return 0x%08X as signed integer.\n", bytenum, (int)packedword, correct_xbyte(packedword, bytenum));
    return 0;
}
```

## 2.72 
You are given the task of writing a function that will copy an integer val into a buffer buf, but it should do so only if enough space is available in the buffer.

Here is the code you write:
```c
/* Copy integer into buffer if space is available */
/* WARNING: The following code is buggy */
void copy_int(int val, void *buf, int maxbytes) {

    if (maxbytes-sizeof(val) >= 0)
            memcpy(buf, (void *) &val, sizeof(val));

}
```

This code makes use of the library function memcpy. Although its use is a bit artificial here, where we simply want to copy an int, it illustrates an approach commonly used to copy larger data structures.

You carefully test the code and discover that it always copies the value to the buffer, even when maxbytes is too small.

A. Explain why the conditional test in the code always succeeds. Hint: The sizeof operator returns a value of type size_t.

B. Show how you can rewrite the conditional test to make it work properly.

**Solution**:
A. sizeof() always return unsigned, which make maxbytes-sizeof(val) is always greater or equal to 0.
B. 
```C
	if (maxbytes >= (int)sizeof(val))
```

## 2.73 
Write code for a function with the following prototype:
```C
/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y);
```

Instead of overflowing the way normal two’s-complement addition does, saturating addition returns TMax when there would be positive overflow, and TMin when there would be negative overflow. Saturating arithmetic is commonly used in programs that perform digital signal processing.

Your function should follow the bit-level integer coding rules (page 164).

**Solution**:
```c
#include<stdio.h>
#include<limits.h>

/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y)
{
    /*
   * if x > 0, y > 0 but sum < 0, it's a positive overflow
   * if x < 0, y < 0 but sum >= 0, it's a negetive overflow
   */
    int sum = x + y;
    int pos_flag = !(INT_MIN & x) && !(INT_MIN & y) && (INT_MIN & sum); // INT_MIN = 0x 8000 0000
    int neg_flag = (INT_MIN & x) && (INT_MIN & y) && !(INT_MIN & sum);

    pos_flag && (sum = INT_MAX);
    neg_flag && (sum = INT_MIN);

    return sum;
}

int main()
{
    int x = 5;
    int y = 5;
    printf("0x%x saturating_add 0x%x equals 0x%x.\n", x, y, saturating_add(x, y));
    x = INT_MAX;
    printf("0x%x saturating_add 0x%x equals 0x%x.\n", x, y, saturating_add(x, y));
    x = -5;
    y = INT_MIN;
    printf("0x%x saturating_add 0x%x equals 0x%x.\n", x, y, saturating_add(x, y));
    return 0;
}
```

## 2.74 
Write a function with the following prototype:
```C
/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y);
```
This function should return 1 if the computation x-y does not overflow.

**Solution**:
```c
#include <stdio.h>
#include <limits.h>

/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y)
{
    /*
   * if x > 0, y < 0 but res <= 0, it's a positive overflow
   * if x < 0, y > 0 but res >= 0, it's a negetive overflow
   */
    int res = x - y;
    int pos_flag = !(INT_MIN & x) && (INT_MIN & y) && ((INT_MIN & res)||(res==0)); // INT_MIN = 0x 8000 0000
    int neg_flag = (INT_MIN & x) && !(INT_MIN & y) && (!(INT_MIN & res) || (res == 0));
    return !(pos_flag || neg_flag);
}

int main()
{
    int x = 5;
    int y = 6;
    printf("tsub_ok() return %d when check %d - %d\n", tsub_ok(x, y), x, y);

    x = INT_MIN;
    printf("tsub_ok() return %d when check %X - %X\n", tsub_ok(x, y), x, y);

    x = INT_MAX;
    y = -5;
    printf("tsub_ok() return %d when check %X - %X\n", tsub_ok(x, y), x, y);
    return 0;
}
```


## 2.75 
Suppose we want to compute the complete 2w-bit representation of x . y, where both x and y are unsigned, on a machine for which data type unsigned is w bits. The low-order w bits of the product can be computed with the expression x\*y, so we only require a procedure with prototype

    unsigned unsigned_high_prod(unsigned x, unsigned y);

that computes the high-order w bits of x . y for unsigned variables. We have access to a library function with prototype

    int signed_high_prod(int x, int y);

that computes the high-order w bits of x . y for the case where x and y are in two’s-complement form. Write code calling this procedure to implement the function for unsigned arguments. Justify the correctness of your solution.

Hint: Look at the relationship between the signed product x . y and the unsigned product x′ . y′ in the derivation of Equation 2.18.

**Solution**:
We know the original equation 2.18:
$x'*^u_wy'=(x' \cdot y')mod2^w$

    $=((x+x_{w-1}2^w) \cdot (y+y_{w-1}2^w))mod2^w$

Because w = 32, and we can have 64 bit width, we don't need $mod2^w$

$x'\cdot y'=(x+x_{w-1}2^w) \cdot (y+y_{w-1}2^w) = x\cdot y + x\cdot y_{w-1}2^w+y\cdot x_{w-1}2^w+y_{w-1}2^w\cdot x_{w-1}2^w$

Our goal is to get the high 32 bits of unsigned multiplication, so we right shift the equation above by 32 bits .

$(x'\cdot y')>> 32= (x\cdot y)>>32 + x\cdot y_{w-1}+y\cdot x_{w-1}+y_{w-1}\cdot x_{w-1}2^w$
$=signed\_high\_prod(x,y) + x\cdot y_{w-1}+y\cdot x_{w-1}+y_{w-1}\cdot x_{w-1}\cdot 2^w$

The last $y_{w-1}\cdot x_{w-1}\cdot 2^{32}$ can be discarded because the bit width is 32 bits.

Therefore, we get the code below:
```C

#include <stdio.h>
#include <inttypes.h>

int signed_high_prod(int x, int y)
{
    int64_t res = (int64_t) x * y;
    res = res >> 32;
    return res;
}

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    // $=signed\_high\_prod(x,y) + x\cdot y_{w-1}+y\cdot x_{w-1}+y_{w-1}\cdot x_{w-1}2^w$
    unsigned y31 = y >> 31;
    unsigned x31 = x >> 31;
    return signed_high_prod(x, y) + x * y31 + y * x31;
}

int main()
{
    unsigned x = 0xFFFFFFFF;
    unsigned y = 0xA;
    printf("unsigned_high_prod returns 0x%X with 0x%X + 0x%X\n", unsigned_high_prod(x,y),x,y);

    return 0;
}
```

## 2.76 
The library function calloc has the following declaration:
```C
	  void *calloc(size_t nmemb, size_t size);
```
According to the library documentation, “The calloc function allocates memory for an array of nmemb elements of size bytes each. The memory is set to zero. If nmemb or size is zero, then calloc returns NULL.”

Write an implementation of calloc that performs the allocation by a call to malloc and sets the memory to zero via memset. Your code should not have any vulnerabilities due to arithmetic overflow, and it should work correctly regardless of the number of bits used to represent data of type size_t.

As a reference, functions malloc and memset have the following declarations:
```C
	 void *malloc(size_t size);
	 void *memset(void *s, int c, size_t n);
```
**Solution**:
Overflow check, we may refer to Practice Problem 2.35:
```C
/* Determine whether arguments can be multiplied without overflow */
int tmult_ok(int x, int y) {

    int p = x*y;
    /* Either x is zero, or dividing p by x gives y */
    return !x || p/x == y;

}
```
So we write the code below:
```C
#include <stdio.h>
#include <limits.h>

void* calloc(size_t ele_count, size_t ele_size)
{
    if (ele_count == 0 || ele_size == 0) return NULL;

    size_t size = ele_count * ele_size;
    if (!ele_count || size / ele_count != ele_size) return NULL; // overflow check

    void* p = malloc(size);
    if (p == NULL) return NULL;
    memset(p, 0, size);
    return p;
}

int main()
{
    size_t ele_count = UINT_MAX;
    size_t ele_size = 2;
    printf("calloc(0x%X, 0x%X) returns 0x%p.\n", ele_count, ele_size, calloc(ele_count, ele_size));

    ele_count = 0;
    ele_size = 2;
    printf("calloc(0x%X, 0x%X) returns 0x%p.\n", ele_count, ele_size, calloc(ele_count, ele_size));

    ele_count = 5;
    ele_size = 2;
    printf("calloc(0x%X, 0x%X) returns 0x%p.\n", ele_count, ele_size, calloc(ele_count, ele_size));
    return 0;
}
```

## 2.77 
Suppose we are given the task of generating code to multiply integer variable x by various different constant factors K. To be efficient, we want to use only the operations +, -, and <<. For the following values of K, write C expressions to perform the multiplication using at most three operations per expression.

A. K = 17  
B. K=−7 
C. K=60 
D. K=−112

**Solution**:
$x\times K=x\times 17=x\times(2^4+2^0)=x<<4+x$
```C
int K17(int x)
{
	return x<<4+x;
}
```

$x\times K=x\times -7=x\times(2^0-2^3)=x-x<<3$
```C
int Kneg7(int x)
{
	return x-x<<3;
}
```

$x\times K=x\times 60=x\times(2^5+2^4+2^3+2^2)=x<<5+x<<4+x<<3+x<<2$ - discarded because of more than 3 operations.
$x\times K=x\times 60=x\times(2^6-2^2)=x<<6-x<<2$
int K60(int x)
{
	return x<<6-x<<2;
}


$x\times K=x\times -112=x\times(2^4-2^7)=x<<4-x<<7$
```C
int Kneg112(int x)
{
	return x<<4-x<<7;
}
```


## 2.78  
Write code for a function with the following prototype:
```C
/* Divide by power of 2. Assume 0 <= k < w-1 */
int divide_power2(int x, int k);
```

The function should compute $x/2^k$ with correct rounding, and it should follow the bit-level integer coding rules (page 164).

**Solution**:
We may refer to the chapter of **Two’s-complement division by a power of 2**, thus get the formula below:
$x/2^k=(x<0? x+(1<<k)-1 : x)>>k$

Then we get the code below:
```C
#include <limits.h>
#include <stdio.h>


/* Divide by power of 2. Assume 0 <= k < w-1 */
int divide_power2(int x, int k)
{
    // $x/2^k=(x<0? x+(1<<k)-1 : x)>>k$
    int sign_bit = ((x & INT_MIN) == INT_MIN);
    sign_bit && (x = x + (1 << k) - 1);
    return x >> k;
}

int main()
{
    int x = 8;
    int k = 3;
    printf("divide_power2(0x%X, 0x%X) returns 0x%X.\n",x,k, divide_power2(x,k));
    x = -17;
    printf("divide_power2(0x%X, 0x%X) returns 0x%X.\n", x, k, divide_power2(x, k));
    return 0;
}
```

## 2.79 
Write code for a function mul3div4 that, for integer argument x, computes 3 ∗ x/4 but follows the bit-level integer coding rules (page 164). Your code should replicate the fact that the computation 3\*x can cause overflow.

**Solution**:
$x\times 3 / 4 = x\times(2^1+2^0)/2^2 = (x<<1 + x)>>2$
We can use the divide_power2 function from 2.78.
We get the code below:
```C
#include <limits.h>
#include <stdio.h>

/* Divide by power of 2. Assume 0 <= k < w-1 */
int divide_power2(int x, int k)
{
    // $x/2^k=(x<0? x+(1<<k)-1 : x)>>k$
    int sign_bit = ((x & INT_MIN) == INT_MIN);
    sign_bit && (x = x + (1 << k) - 1);
    return x >> k;
}

int mul3div4(int x)
{
    // x*3/4 = x * (2^1 + 2^0) / 2^2
    int res = (x << 1) + x;
    return divide_power2(res, 2);
}

int main()
{
    int x = 10;
    printf("mul3div4(0x%X) returns 0x%X.\n", x, mul3div4(x));
    x = INT_MAX;
    printf("mul3div4(0x%X) returns 0x%X.\n", x, mul3div4(x));
    x = INT_MIN;
    printf("mul3div4(0x%X) returns 0x%X.\n", x, mul3div4(x));
    return 0;
}
```

## 2.80 
Write code for a function threefourths that, for integer argument x, computes the value of $\frac{3}{4} x$, rounded toward zero. It should not overflow. Your function should follow the bit-level integer coding rules (page 164).

**Solution**:
 $x/2^k=(x<0? x+(1<<k)-1 : x)>>k$










