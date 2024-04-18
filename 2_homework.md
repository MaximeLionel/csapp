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

| unsigned x (w=2) | x>>1 | x^x>>1 | odd number of 1s |  |  |
| ---- | ---- | ---- | :--: | ---- | ---- |
| 00 | 00 | 00 | False |  |  |
| 01 | 00 | 01 | True |  |  |
| 10 | 01 | 11 | True |  |  |
| 11 | 01 | 10 | False |  |  |
| unsigned x (w=4) | x>>1 | x=x^x>>1 | x>>2 | x=x^x>>2 | odd number of 1s |
| 0000 | 0000 | 0000 | 0000 | 0000 | False |
| 0001 | 0000 | 0001 | 0000 | 0001^0000=0001 | True |
| 0010 | 0001 | 0011 | 0000 | 0011^0000=0011 | True |
| 0011 | 0001 | 0010 | 0000 | 0010^0000=0010 | False |
| 0100 | 0010 | 0110 | 0001 | 0110^0001=0111 | True |
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
|  | 0111 | 1000 | 0010 | 1010 | False |

| ux (w=8) | x>>1 | x=x^x>>1 | x>>2 | x=x^x>>2 | x>>4 | x=x^x>>4 | odd 1s? |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | False |
| 0000 0001 | 0000 0000 | 0000 0001 | 0000 0000 | 0000 0001 | 0000 0000 | 0000 0001 | True |
| ... | ... | ... | ... | ... | ... | ... | ... |
| 1111 1110 | 0111 1111 | 1000 0001 | 0010 0000 | 1010 0001 | 0000 1010 | 1010 1011 | True |
| 1111 1111 | 0111 1111 | 1000 0000 | 0010 0000 | 1010 0000 | 0000 1010 | 1010 1010 | False |
w=16, >>1, >>2, >>4, >>8
w=32, >>1, >>2, >>4, >>8, >>16

To summarize and deduce logically:
1. Bit 0 after executing the code below shows if bit 0 and bit 1 contain odd number of 1s.
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
7. Bit 0 of x (w=32) after executing the code above shows if ==bit 0, bit 1, ..., bit 31== contains odd number of 1s, finally.
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
[000111] = [000111]>>1 + 1 = [000100]
[01111111] = [01111111]>>1 + 1 = [01000000]

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
```c
#include<stdio.h>

int lower_one_mask(int n) {
	int w = sizeof(int) << 3;
	int mask = (unsigned)-1 >> (w - n);
	return mask;
}

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

/*
 * Do rotating left shift.  Assume 0 <= n < w
 * Examples when x = 0x12345678 and w = 32:
 *    n=4 -> 0x23456781, n=20 -> 0x67812345
 */
unsigned rotate_left(unsigned x, int n)
{
	int w = sizeof(int) << 3;

	int low_bits = (unsigned)x>> (w-n-1)>>1;
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
    int temp_shift = word << (w - ((bytenum + 1) << 3));  // temp_shift = 0xF8000000
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
   * if x > 0, y > 0 but sum <  0, it's a positive overflow
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
```c
int K60(int x)
{
	return x<<6-x<<2;
}
```


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
#include <assert.h>

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
    assert(mul3div4(x)==3*x/4);
    x = INT_MAX;
    assert(mul3div4(x)==3*x/4);
    x = INT_MIN;
    assert(mul3div4(x)==3*x/4);
    return 0;
}
```

## 2.80 
Write code for a function threefourths that, for integer argument x, computes the value of $\frac{3}{4} x$, rounded toward zero. It should not overflow. Your function should follow the bit-level integer coding rules (page 164).

**Solution**:
 $x/2^k=(x<0? x+(1<<k)-1 : x)>>k$

To calculate $\frac{3}{4} x$ and avoid overflow, we may choose to divide by 4 first then multiply by 3.
But there's one issue to solve that if we divide by 4 first, means we will discard the last 2 bits directly, we may lose the precision, because there's a multiplication of 3 after.
Therefore, we write the code below:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>

int threefourths(int x)
{
    // First, we divide x into high 30 bits and low 2 bits.
    int mask2bits = 0x3;
    int high30 = ~mask2bits & x;
    int low2 = mask2bits & x;

    // For high 30 bits, we may divide by 4 first 
    high30 = high30 >> 2;
    int high30_res = (high30 << 1) + high30;


    // For low 2 bits, we may multiply 3 first then divide.
    low2 = (low2 << 1) + low2;
    int sign_bit = (x & INT_MIN) == INT_MIN;
    int bias = (1 << 2) - 1;
    (sign_bit == 1) && (low2 += bias);
    int low2_res = low2 >> 2;
    
    return high30_res + low2_res;
}

int main()
{
    int x = 8;
    printf("threefourths(0x%X) returns 0x%X.\n", x, threefourths(x));
    x = 9;
    printf("threefourths(0x%X) returns 0x%X.\n", x, threefourths(x));
    x = 10;
    printf("threefourths(0x%X) returns 0x%X.\n", x, threefourths(x));
    x = -8;
    printf("threefourths(0x%X) returns 0x%X.\n", x, threefourths(x));
    x = -9;
    printf("threefourths(0x%X) returns 0x%X.\n", x, threefourths(x));
    x = -10;
    printf("threefourths(0x%X) returns 0x%X.\n", x, threefourths(x));
    return 0;
}
```

## 2.81 
Write C expressions to generate the bit patterns that follow, where $a^k$ represents k repetitions of symbol a. Assume a w-bit data type. Your code may contain references to parameters j and k, representing the values of j and k, but not a parameter representing w.

A. $1^{w−k}0^k$
B. $0^{w−k−j}1^k0^j$
* A(k) = $1^{w−k}0^k$
* ~A(k) = $0^{w−k}1^k$
* (~A(k)) << j = $0^{w−k−j}1^k0^j$

**Solution**:
```C
#include <stdio.h>

// $1^{w−k}0^k$
int A(int k)
{
    int sample = -1;
    return sample << k;
}

// $0^{w−k−j}1^k0^j$
int B(int k, int j)
{
    return (~A(k)) << j;
}

int main()
{
    int k = 16;
    printf("A(%d) returns 0x%08X.\n", k, A(16));
    int j = 8;
    printf("B(%d,%d) returns 0x%08X.\n", k,j, B(16,8));
    return 0;
}
```

## 2.82 
We are running programs where values of type int are 32 bits. They are represented in two’s complement, and they are right shifted arithmetically. Values of type unsigned are also 32 bits.
We generate arbitrary values x and y, and convert them to unsigned values as follows:
```C
/* Create some arbitrary values */
int x = random();
int y = random();
/* Convert to unsigned */
unsigned ux = (unsigned) x;
unsigned uy = (unsigned) y;
```
For each of the following C expressions, you are to indicate whether or not the expression always yields 1. If it always yields 1, describe the underlying mathematical principles. Otherwise, give an example of arguments that make it yield 0.

A. $(x<y) == (-x>-y)$
B. $((x+y)<<4) + y-x == 17*y+15*x$
C. $\sim x+\sim y+1 == \sim(x+y)$
D. $(ux-uy) == -(unsigned)(y-x)$
E. $((x>>2)<<2)<=x$

**Solution**:
A. $(x<y) == (-x>-y)$
	False. 
	When x = INT_MIN, y = 0,
	-x will still be INT_MIN.
B. $((x+y)<<4) + y-x == 17*y+15*x$
	True.
C. $\sim x+\sim y+1 == \sim(x+y)$
	-x = ~x + 1, -y = ~y+1
	$\sim x+\sim y+1 = -x-1 + -y-1 + 1 = -(x+y)-1 = \sim(x+y)+1-1$
	True.
D. $(ux-uy) == -(unsigned)(y-x)$
	True.
E. $((x>>2)<<2)<=x$
	True.
Code:
```C
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

// A. (x<y) == (-x>-y)
int A(int x, int y)
{
    return (x < y) == (-x > -y);
}

// B. ((x+y)<<4) + y-x == 17*y+15*x
int B(int x, int y)
{
    return ((x + y) << 4) + y - x == 17 * y + 15 * x;
}

// C.$~x + ~y + 1 == ~(x + y)$
int C(int x, int y)
{
    return ~x + ~y + 1 == ~(x + y);
}

// D. $(ux-uy) == -(unsigned)(y-x)$
int D(unsigned ux, unsigned uy, int x, int y)
{
    return (ux - uy) == -(unsigned)(y - x);
}

// E. $((x>>2)<<2)<=x$
int E(int x, int y)
{
    return ((x >> 2) << 2) <= x;
}

int main()
{
    /* Create some arbitrary values */
    srand((unsigned)time(0)); // set seed for rand(). rand() will use the seed to generate random number.
                                // time(0) - seconds since January 1, 1970
    int x = rand();
    int y = rand();
    /* Convert to unsigned */
    unsigned ux = (unsigned)x;
    unsigned uy = (unsigned)y;

    /*
    A. $(x<y) == (-x>-y)$
    B. $((x+y)<<4) + y-x == 17*y+15*x$
    C. $~x+~y+1 == ~(x+y)$
    D. $(ux-uy) == -(unsigned)(y-x)$
    E. $((x>>2)<<2)<=x$
    */
    assert(!A(INT_MIN, 0));
    assert(B(x, y));
    assert(C(x, y));
    assert(D(ux, uy, x, y));
    assert(E(x, y));

    return 0;
}
```

## 2.83 
Consider numbers having a binary representation consisting of an infinite string of the form $0.y y y y y y$ . . . , where y is a k-bit sequence. For example, the binary representation of $\frac{1}{3}$ is 0.01010101 . . . (y = 01), while the representation of $\frac{1}{5}$ is 0.001100110011 . . . (y = 0011).

1. Let $Y = B2U_k(y)$, that is, the number having binary representation y. Give a formula in terms of Y and k for the value represented by the infinite string. Hint: Consider the effect of shifting the binary point k positions to the right.
    
2. What is the numeric value of the string for the following values of y? 
	(a) 101
    (b) 0110 
    (c) 010011

**Solution**:
1. 
	what is y? - y is a k-bit sequence.
	what is k? - k is the width of y.
	what is Y? - Y is the number having binary representation y.
	So:
	$0.y y y... y y y<<k = y.yyy...yyy=Y+0.y y y... y y y$
	$Y=0.yyy...yyy<<k-0.y y y... y y y = 0.y y y... y y y(2^k - 1)$
	$0.yyy...yyy=Y/(2^k-1)$

2. We may use the equation above, so only need to find values of Y and k.
		(a) $Y=101_2=5_{10},k=3,result = \frac{5}{7}$
		(b) $Y=0110_2=6_{10},k=4,result = \frac{2}{5}$
		(c) $Y=010011_2=18_{10},k=6,result = \frac{19}{31}$

## 2.84 
Fill in the return value for the following procedure, which tests whether its first argument is less than or equal to its second. Assume the function f2u returns an unsigned 32-bit number having the same bit representation as its floating-point argument. You can assume that neither argument is NaN. The two flavors of zero, +0 and −0, are considered equal.
```C
// Test if x <= y
int float_le(float x, float y) {
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);

	 /* Get the sign bits */
    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;

    /* Give an expression using only ux, uy, sx, and sy */
	return    ; 
}
```

**Solution**:
Condition 1: x = +0.0 or -0.0, y = +0.0 or -0.0, then x == y
	We do not need sign bit, so just << 1.
	```C
	x << 1 == 0 && y << 1 == 0
	```
Condition 2: x < 0, y > 0
	We need to get the sign bit.
	```C
	unsigned x_signbit = ux >> 31;
	unsigned y_signbit = uy >> 31;
	x_signbit && !y_signbit
	```
Condition 3: x > 0, y > 0
	```C
	!x_signbit && !y_signbit && ux <= uy
	f = s | E | M
	```
Condition 4: x < 0, y < 0 
	```C
	x_signbit && y_signbit && ux >= uy
	```
Code:
```C
#include <stdio.h>
#include <assert.h>

unsigned f2u(float x)
{
    return *(unsigned*)(&x);
}

// Test if x <= y
int float_le(float x, float y) {
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);

    /* Get the sign bits */
    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;

    /* Give an expression using only ux, uy, sx, and sy */
    return (ux << 1 == 0 && uy << 1 == 0)   // Condition 1
        || (sx && !sy)                  // Condition 2
        || (!sx && !sy && ux <= uy)     // Condition 3
        || (sx && sy && ux >= uy);      // Condition 4
}

int main()
{
    assert(float_le(+0.0, -0.0));
    assert(float_le(-0.8, 0.9));
    assert(float_le(5.0, 9.0));
    assert(float_le(-9.0, -5.0));
    return 0;
}
```

## 2.85 
Given a floating-point format with a k-bit exponent and an n-bit fraction, write formulas for the exponent E, the significand M, the fraction f , and the value V for the quantities that follow. In addition, describe the bit representation.

A. The number 7.0  
B. The largest odd integer that can be represented exactly 
C. The reciprocal of the smallest positive normalized value

**Solution**:
$V = (−1)^s × M × 2^E$
$bias = 2^{k-1}-1$

A.
$7.0 = 111.000_2 = 1.11_2 \times 2^2$
so:
$f=0.110..0$
$E=e-bias$ then $e = E+bias = 10_2+bias = 2^{k-1}+1 = 100..01_2$
Therefore, $7.0_{10} = 0{\,} 10...01{\,} 110...0$

B.
First $s = 0$
Next we know if odd, the last bit of f must be 1, if biggest, $f = 0.1...1$ with n 1s, $M=1+f=1.1...1$ with n 1s.
Then E must be equal to n:
	$e-bias = n$
	$e=bias+n=2^{k-1}+n-1$
Therefore, the result is: $0{\,}{\,}1...(n-1)_{10}{\,}{\,}11...11$ = $2^{n+1}-1$
	
C.
$M=1.0...0$
because it's normalized, the smallest e will be 0...01.
Then $E=e-bias=1-bias$
Therefore, $V=2^{1-bias}$

For reciprocal, $V_r=2^{bias-1}$
Then, $s=0,f=0.0...0,E=bias-1,e=2\times bias-1=2\times (2^{k-1}-1)-1=2^k-3$ = $[111...101]_2$
the final V = 0  111...101  000...000
## 2.86 
Intel-compatible processors also support an “extended-precision” floating-point format with an 80-bit word divided into a sign bit, k = 15 exponent bits, a single integer bit, and n = 63 fraction bits. The integer bit is an explicit copy of the implied bit in the IEEE floating-point representation. That is, it equals 1 for normalized values and 0 for denormalized values. Fill in the following table giving the approximate values of some “interesting” numbers in this format:

| Description | Value | Decimal |
| ---- | ---- | ---- |
| Smallest positive denormalized | $0{\,}0...0{\,}0...01$ | $2^{-61-2^{14}}$ |
| Smallest positive normalized | $0{\,}0...01{\,}0...0$ | $2^{2-2^{14}}$ |
| Largest normalized | $0{\,}1...10{\,}1...1$ | $(2-2^{-63})\times 2^{bias}$ |
This format can be used in C programs compiled for Intel-compatible machines by declaring the data to be of type `long double`. However, it forces the compiler to generate code based on the legacy 8087 floating-point instructions. The resulting program will most likely run much slower than would be the case for data type float or double.

**Solution**:
$bias = 2^{k-1}-1=2^{14}-1$

Smallest positive denormalized:
	$e=[0000..000]_2=0$
	$M=f=0.00...001$
	$E=1-bias=2-2^{14}$
	$V=(-1)^s \times M \times 2^E$ = $2^{-63} \times 2^{(2-2^{14})}$ = $2^{-2^{14}-61}$

Smallest positive normalized:
	$e=[000...001]=1$
	$E=e-bias=2-2^{14}$
	$M=1+f=1$
	$V=(-1)^s \times M \times 2^E$ = $2^{2-2^{14}}$
	
Largest normalized:
	$e=[111...110]$ = $2^{15}-2$
	$E=e-bias$ = $2^{15}-2 - 2^{14}+1$ = $2^{14}-1$
	$M=1+f=1.11...11_2$
	$V=(-1)^s \times M \times 2^E$ = $1.11...11_2 \times 2^{2^{14}-1}$ = $(2-2^{-63}) \times 2^{2^{14}-1}$

## 2.87 
The 2008 version of the IEEE floating-point standard, named IEEE 754-2008, includes a 16-bit “half-precision” floating-point format. It was originally devised by computer graphics companies for storing data in which a higher dynamic range is required than can be achieved with 16-bit integers. This format has 1 sign bit, 5 exponent bits (k = 5), and 10 fraction bits (n = 10). The exponent bias is $2^{5−1} − 1 = 15$.

Fill in the table that follows for each of the numbers given, with the following instructions for each column:
	Hex: The four hexadecimal digits describing the encoded form.
	M: The value of the significand. This should be a number of the form $x$ or $\frac{x}{y}$ , where x is an integer and y is an integral power of 2. Examples include 0, $\frac{67}{64}$ and $\frac{1}{256}$
	E: The integer value of the exponent.  
	V : The numeric value represented. Use the notation x or $x \times 2^z$, where x and z are integers.  
	D: The (possibly approximate) numerical value, as is printed using the %f formatting specification of printf.

As an example, to represent the number $\frac{7}{8}$, we would have s = 0, M = $\frac{7}{4}$ and E = −1. Our number would therefore have an exponent field of $01110_2$ (decimal value 15 − 1 = 14) and a significand field of $1100000000_2$, giving a hex representation 3B00. The numerical value is 0.875.
You need not fill in entries marked —.

| Description          | Hex  | M   | E   | V         | D         |
| -------------------- | ---- | --- | --- | --------- | --------- |
| -0                   |      |     |     | -0        | -0.0      |
| Smallest value > 2   |      |     |     |           |           |
| 512                  |      |     |     | 512       | 512.0     |
| Largest denormalized |      |     |     |           |           |
| $-\infty$            |      | —   | —   | $-\infty$ | $-\infty$ |
| Number with hex      | 3BB0 |     |     |           |           |
**Solution**:

| Description          | Hex     | M                | E   | V                     | D             |
| -------------------- | ------- | ---------------- | --- | --------------------- | ------------- |
| -0                   | 0x8000  | 0                | -14 | -0                    | -0.0          |
| Smallest value > 2   | 0x4001  | $1+2^{-10}$      | 1   | $\frac{1025}{512}$    | 2.00195312    |
| 512                  | 0x 6000 | 1                | 9   | 512                   | 512.0         |
| Largest denormalized | 0x 03FF | 1                | -14 | $\frac{1023}{2^{24}}$ | 6.09755516e-5 |
| $-\infty$            | 0x FC00 | —                | —   | $-\infty$             | $-\infty$     |
| Number with hex      | 3BB0    | $\frac{123}{64}$ | -1  | $\frac{123}{128}$     | 0.9609375     |

1 sign bit, k = 5, n = 10
bias = $2^{k-1}-1$ = 15

-0:
	$s=1$
	$e=0,f=0$
	Hex = 8000
	M = f= 0
	E = 1- bias = -14

Smallest value > 2:
	if V == 2, s = 0, M = 1, E = 1, then $M \times 2^E == 2$
	if M == 1, f = 0
	if E == 1, e - bias = 1, then e = 16 = $[10000]_2$
	if we want a value just above 2, we can set f = $0000000001_2$ and M = 1+f = $1+2^{-10}$ 
	Therefore, Hex = 0b 0 10000 0000000001 = 0x 4001
	V = $(1+2^{-10}) \times 2$ = $\frac{1025}{512}$

512:
	512 = $2^9$ 
	so s = 0, M = 1, E = 9
	if M == 1, f = 0
	if E == 9, e = bias + 9 = 24 = $[11000]$
	Hex = 0b 0 11000 0000000000 = 0x 6000
	V = 512

Largest denormalized:
	e = 0, s = 0, E = -14
	we only need settle M
	M = f = $0.111..11_2$
	Hex = 0b 0 00000 1111111111 = 0x 03FF
	V = $(1-2^{-10})\times 2^{-14}$ = $\frac{1023}{2^{24}}$

$-\infty$:
	e = $[11111]_2$
	s = 1
	f = 0000000000
	Hex = 0b 1 11111 0000000000 = 0x FC00
	
Number with hex 3BB0:
	hex = 0x 3BB0 = 0b 0 01110 1110110000
	s = 0, 
	e = 0b 01110 = 14, E = e - bias = -1
	f = $0.1110110000_2$, M = 1 + f = $1.1110110000_2$ = $\frac{123}{64}$
	V = $1.1110110000_2 \times 2 ^{-1}$ = $\frac{123}{128}$

## 2.88 
Consider the following two 9-bit floating-point representations based on the IEEE floating-point format.

1. Format A  
There is 1 sign bit.

There are k = 5 exponent bits. The exponent bias is 15. There are n = 3 fraction bits.

2. Format B  
There is 1 sign bit.

There are k = 4 exponent bits. The exponent bias is 7. There are n = 4 fraction bits.

In the following table, you are given some bit patterns in format A, and your task is to convert them to the closest value in format B. If rounding is necessary you should round toward +∞. In addition, give the values of numbers given by the format A and format B bit patterns. Give these as whole numbers (e.g., 17) or as fractions (e.g., 17/64 or 17/26).

| FormatA - Bits          | FormatA - Value | FormatB - Bits | FormatB - Value |
| ----------------------- | --------------- | -------------- | --------------- |
| $1{\,\,}01111{\,\,}001$ |                 |                |                 |
| $0{\,\,}10110{\,\,}011$ |                 |                |                 |
| $1{\,\,}00111{\,\,}010$ |                 |                |                 |
| $0{\,\,}00000{\,\,}111$ |                 |                |                 |
| $1{\,\,}11100{\,\,}000$ |                 |                |                 |
| $0{\,\,}10111{\,\,}100$ |                 |                |                 |
**Solution**:

| FormatA - Bits          | FormatA - Value    | FormatB - Bits | FormatB - Value    |
| ----------------------- | ------------------ | -------------- | ------------------ |
| $1{\,\,}01111{\,\,}001$ | $-\frac{9}{8}$     | 1 0111 0010    | $-\frac{9}{8}$     |
| $0{\,\,}10110{\,\,}011$ | 176                | 0 1110 0110    | 176                |
| $1{\,\,}00111{\,\,}010$ | $-\frac{5}{1024}$  | 1 0000 0101    | $-\frac{5}{1024}$  |
| $0{\,\,}00000{\,\,}111$ | $\frac{7}{2^{17}}$ | 0 0000 0001    | $\frac{1}{2^{10}}$ |
| $1{\,\,}11100{\,\,}000$ | $-2^{13}$          | 1 1110 1111    | -248               |
| $0{\,\,}10111{\,\,}100$ | $3\times 2^7$      | 0 1111 0000    | +$\infty$          |

A: s = 1, k = 5, n = 3, bias = 15
B: s = 1, k = 4, n = 4, bias = 7

$1{\,\,}01111{\,\,}001$:
Format A: 
	$s_A$ = 1
	$e_A$ = $[01111]_2$ = 15 - normalized value
	$E_A$ = $e_A - bias_A$ = 0
	$f_A$ = $0.001_2$
	$M_A$ = 1 + $f_A$ = $1.001_2$ = $\frac{9}{8}$
	$V_A$ = $(-1)\times \frac{9}{8} \times 2^0$ = $-\frac{9}{8}$
Format B:
	$s_B$ = 1
	$E_B$ = $E_A$ -> $e_B$ = 7 = $[0111]_2$
	$f_B$ = $f_A$ = $0.0010_2$
	$V_B$ =  $-\frac{9}{8}$ = [1 0111 0010]

$0{\,\,}10110{\,\,}011$:
Format A:
	s = 0
	e = $[10110]_2$ = 22 - normalized value
	E = e - bias = 7
	f = $0.011_2$
	M = 1 + f = $1.011_2$ = $\frac{11}{8}$
	V = $\frac{11}{8} \times 2^7$ = 176
Format B:
	$E_B = 7$ -> $e_B = 7 + 7$ = 14 = $[1110]_2$
	$f_B = 0.0110$ = $f_A$
	$V_B = 176$ = [0 1110 0110]

$1{\,\,}00111{\,\,}010$:
Format A: 
	$s_A$ = 1
	$e_A$ = $[00111]_2$ = 7 - normalized value
	$E_A$ = $e_A - bias_A$ = -8
	$f_A$ = $0.010_2$
	$M_A$ = 1 + $f_A$ = $1.010_2$ = $\frac{5}{4}$
	$V_A$ = $(-1)\times \frac{5}{4} \times 2^{-8}$ = $-\frac{5}{1024}$
Format B:
	$s_B$ = 1
	$E_B$ = $E_A$ -> $e_B$ = $E_A + bias_B$ 
	Normalized:  [0001] <= $e_B$ <= [1110] ->   -6 <= $E_B$ <= 7 - to discard!
	Denormalized: $e_B$ = 0
	$E_B$ = -6
	To make sure, $V_B$ = $V_A$ = $(-1) \times M_B \times 2^{-6}$ = $-\frac{5}{1024}$ = $-\frac{5}{2^{10}}$
	so $M_B$ = $\frac{5}{16}$ = $0.0101_2$ = f
	$V_B$ = 1 0000 0101

$0{\,\,}00000{\,\,}111$:
Format A: 
	$s_A$ = 0
	$e_A$ = $[00000]_2$ - denormalized value
	$E_A$ = $1 - bias_A$ = -14
	$f_A$ = $0.111_2$
	$M_A$ = $f_A$ =  $\frac{7}{8}$
	$V_A$ = $\times \frac{7}{8} \times 2^{-14}$ = $\frac{7}{2^{17}}$
Format B:
In denormalized form:
	$s_B$ = 0
	$e_B$ = 0000
	$E_B$ = -6
	$f_B$ = $0.0001_2$ = $M_B$
	$V_B$ =  $\frac{1}{2^{10}}$ = 0 0000 0001 
	because it's rounding toward +∞ and $\frac{1}{2^{10}}$ is already the smallest value in denormalized form, it's the correct result.
	Why no need to consider the normalized form? because the smallest value of normalized form must be larger than that of denormalized form.
		![[image-20240324104632780.png]]

$1{\,\,}11100{\,\,}000$:
Format A: 
	$s_A$ = 1
	$e_A$ = $[11100]_2$ = 28 - normalized value
	$E_A$ = $e_A - bias_A$ = 13
	$f_A$ = $0.000_2$
	$M_A$ = 1 
	$V_A$ = $(-1)\times 1 \times 2^{13}$ = $-2^{13}$
Format B:
	$s_B$ = 1
	$E_B$ = $E_A$ -> $e_B$ = 28+7 = 35 we cannot fulfill
	in normalized form: -6 <= $E_B$ <= 7, we find that cannot meet the exponent.
	suppose $E_B = 7$ -> $e_B$ = $1110_2$
	$f_B$ = $1111_2$
	$V_B$ = $-1\times 1.1111_2 \times 2^{7}$ = -248 = 1 1110 1111

$0{\,\,}10111{\,\,}100$:
Format A: 
	$s_A$ = 0
	$e_A$ = $[10111]_2$ = 23 - normalized form
	$E_A$ = $e_A - bias_A$ = 8
	$f_A$ = $0.100_2$
	$M_A$ = 1 + $f_A$ = $1.100_2$ = $\frac{3}{2}$
	$V_A$ = $\frac{3}{2} \times 2^8$ = $3\times 2^7$
Format B:
	$s_B$ = 0
	if $E_B$ = $E_A$ -> $e_B$ = 15 which is special value
	$V_B$ = +$\infty$ = 0 1111 0000
	

## 2.89
We are running programs on a machine where values of type int have a 32- bit two’s-complement representation. Values of type float use the 32-bit IEEE format, and values of type double use the 64-bit IEEE format.

We generate arbitrary integer values x, y, and z, and convert them to values of type double as follows:
```C
/* Create some arbitrary values */

int x = random();
int y = random();
int z = random();


/* Convert to double */

double dx = (double) x;
double dy = (double) y;
double dz = (double) z;
```

For each of the following C expressions, you are to indicate whether or not the expression always yields 1. If it always yields 1, describe the underlying mathematical principles. Otherwise, give an example of arguments that make it yield 0. Note that you cannot use an IA32 machine running gcc to test your answers, since it would use the 80-bit extended-precision representation for both float and double.

1. (float) x == (float) dx
2. dx - dy == (double) (x-y)
3. (dx+dy)+dz\==dx+(dy+dz)
4. (dx\*dy)\*dz\==dx\*(dy\*dz)
5. dx/dx\==dz/dz

**Solution**：
1. (float) x == (float) dx: right
2. dx - dy == (double) (x-y): wrong when x = 0, y = INT_MIN
3. (dx+dy)+dz\==dx+(dy+dz): right
4. (dx\*dy)\*dz\==dx\*(dy\*dz): wrong
		(1e20 * 1e20) * 1e(-20) = $+\infty$
		1e20 *(1e20 * 1e(-20)) = 1e20
5. dx/dx\==dz/dz: wrong when dx !=0 and dz == 0

## 2.90 
You have been assigned the task of writing a C function to compute a floating-point representation of $2^x$. You decide that the best way to do this is to directly construct the IEEE single-precision representation of the result. When x is too small, your routine will return 0.0. When x is too large, it will return +∞. Fill in the blank portions of the code that follows to compute the correct result. Assume the function u2f returns a floating-point value having an identical bit representation as its unsigned argument.
```C
float fpwr2(int x)
{

    /* Result exponent and fraction */
    unsigned exp, frac;
    unsigned u;

if (x < ) {  
/* Too small. Return 0.0 */ exp = ;  
frac = ;

} elseif(x< ){ /* Denormalized result */ exp = ;  
frac = ;

} else if (x < ) { /* Normalized result. */ exp = ;  
frac = ;

} else {  
/* Too big. Return +oo */ exp = ;  
frac = ;

}

    /* Pack exp and frac into 32 bits */
    u = exp << 23 | frac;
    /* Return as float */
    return u2f(u);

}
```

**Solution**:
floating-point is single precision:
s = 1, k = 8, n = 23
$bias = 2^{k-1}-1=127$
For positive normalized,
	For max, e = [11111110], so E = e - bias = 127
	$V_{max}=(-1)^0\times1.11...11_2\times2^{127}$ is in range ($2^{127},2^{128}$)
	For min, e = [00000001], so E = -126
	$V_{min}=(-1)^0\times1.00...00\times2^{-126}=2^{-126}$
For positive denormalized,  
	E = 1 - bias = -126
	M = 0.f = $0.111...111_2$
	$V_{max}\approx 2^{-126}$
	$M_{min}=f_{min}=0.000...001_2$
	$V_{min}=(-1)^0\times 0.00...001\times 2^{1-127}=2^{-149}$

0 -> $2^{-149}$ -> $2^{-126}$ -> $2^{127}$ -> $+\infty$
1. 0 -> $2^{-149}$ $\approx$ 0.0 then e = 0, f = 0
2. $2^{-149}$ -> $2^{-126}$ : e = 0, f is in range [00...00] and [11...10]
	we know $E = 2^{-126}$
	in denormalized form, M = 0.f
	$2^x$ = $2^{-126}\times 2^{x+126}$, then f = $2^{x+126}\times 2^{23}$ = $2^{x+149}$ = 1 << (x + 149)
3. $2^{-126}$ -> $2^{127}$: 
	we wanna do $2^x$,in normalized case, $M = 1 + 0.f$ cannot be represented into $2^x$, so f have to be 0. 
	f = 0, e is in range [0000 0001] to [1111 1110] 
	for $2^x$ = $2^{x+bias-bias}$, then e = x + bias = x + 127
4. $2^{127}$ -> $+\infty$: e = [1111 1111], f = 0

Therefore, the code is like:
```C
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

float fpwr2(int x)
{
    /* Result exponent and fraction */
    unsigned exp, frac;
    unsigned u;

    /*if x < smallest positive denormalized, return 0.0 (denormalized) */
    if (x < -149) {
        /* Too small. Return 0.0 */ 
        exp = 0;
        frac = 0;
    } 
    /*if x < smallest positive normalized / largest positive denormalized, return (denormalized)*/
    else if(x < -126) 
    {   /* Denormalized result */ 
        exp = 0;
        frac = 1 << (x + 149);
    }
    else if (x < 128) 
    { /* Normalized result. */ 
        exp = x + 127;
        frac = 0;
    }
    else {
        /* Too big. Return +oo */ 
        exp = 0xFF;
        frac = 0;
    }

    /* Pack exp and frac into 32 bits */
    u = (exp << 23 | frac) & INT_MAX;
    /* Return as float */
    return u2f(u);
}

int main()
{
    assert(fpwr2(0) == powf(2, 0));
    assert(fpwr2(100) == powf(2, 100));
    assert(fpwr2(-100) == powf(2, -100));
    assert(fpwr2(10000) == powf(2, 10000));
    assert(fpwr2(-10000) == powf(2, -10000));
    return 0;
}
```

## 2.91 
Around 250 B.C., the Greek mathematician Archimedes proved that $\frac{223}{71}$ < π < $\frac{22}{7}$ . Had he had access to a computer and the standard library <math.h>, he would have been able to determine that the single-precision floating-point approximation of π has the hexadecimal representation 0x40490FDB. Of course, all of these are just approximations, since π is not rational.

A. What is the fractional binary number denoted by this floating-point value?

B. What is the fractional binary representation of $\frac{22}{7}$? Hint: See Problem 2.83. 

C. At what bit position (relative to the binary point) do these two approximations to π diverge?

**Solution**:
A. 
0x40490FDB = 0100 0000 0100 1001 0000 1111 1101 1011 = 0 10000000 10010010000111111011011
s = 0, e = 1000 0000, f = 10010010000111111011011
bias = 127
$V=(-1)^0\times (1+0.f)\times 2^{e-127}$ = $11.0010010000111111011011_2$

B.
From 2.83, we know that:
	$0.yyy...yyy=Y/(2^k-1)$
		y is a k-bit sequence
		k is the width of y;
		Y is the number having binary representation y;
So $\frac{22}{7}$ = $3\frac{1}{7}$, 
suppose Y = 1, then $2^k-1=7$, so we know the width of y = k =3, the value of y is Y = 1, therefore, y = 001, Finally, $\frac{1}{7}=0.001001001...$
$3\frac{1}{7}$ = 3+ $\frac{1}{7}$ = $0b11.001001(001)...$

C. 
$\frac{223}{71}$ = $3\frac{10}{71}$
$\frac{10}{71} \times 2$ = $\frac{20}{71}$, it's less than 1, so the 1st bit after fraction point is 0. -> 0.0
$\frac{20}{71} \times 2$ = $\frac{40}{71}$ -> 0.00
$\frac{40}{71} \times 2$ = $1\frac{9}{71}$ -> 0.001
$\frac{9}{71} \times 2$ = $\frac{18}{71}$ -> 0.0010
$\frac{18}{71} \times 2$ = $\frac{36}{71}$ -> 0.00100
$\frac{36}{71} \times 2$ = $1\frac{1}{71}$ -> 0.001001
$\frac{1}{71} \times 2$ = $\frac{2}{71}$ -> 0.0010010
$\frac{2}{71} \times 2$ = $\frac{4}{71}$ -> 0.00100100
$\frac{4}{71} \times 2$ = $\frac{8}{71}$ -> 0.001001000
so the 9th bit is different.

## Bit-Level Floating-Point Coding Rules
In the following problems, you will write code to implement floating-point functions, operating directly on bit-level representations of floating-point numbers. Your code should exactly replicate the conventions for IEEE floating-point operations, including using round-to-even mode when rounding is required.

To this end, we define data type float_bits to be equivalent to unsigned: 
```C
/* Access bit-level representation floating-point number */

typedef unsigned float_bits;
```

Rather than using data type float in your code, you will use float_bits. You may use both int and unsigned data types, including unsigned and integer constants and operations. You ==may not use any unions, structs, or arrays==. Most significantly, you ==may not use any floating-point data types, operations, or constants==. Instead, your code should perform the bit manipulations that implement the specified floating-point operations.

The following function illustrates the use of these coding rules. For argument f, it returns ±0 if f is denormalized (preserving the sign of f ), and returns f otherwise.
```C
/* If f is denorm, return 0.  Otherwise, return f */
float_bits float_denorm_zero(float_bits f) {

    /* Decompose bit representation into parts */
    unsigned sign = f>>31;
    unsigned exp  = f>>23 & 0xFF;
    unsigned frac = f     & 0x7FFFFF;

    if (exp == 0) {
        /* Denormalized.  Set fraction to 0 */
        frac = 0;
    }
    
    /* Reassemble bits */
    return (sign << 31) | (exp << 23) | frac;
}
```


## 2.92 
Following the bit-level floating-point coding rules, implement the function with the following prototype:
```C
/* Compute -f.  If f is NaN, then return f. */
float_bits float_negate(float_bits f);
```
For floating-point number f, this function computes −f. If f is NaN , your function should simply return f.

Test your function by evaluating it for all $2^{32}$ values of argument f and comparing the result to what would be obtained using your machine’s floating-point operations.

**Solution**:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Compute -f.  If f is NaN, then return f. */
float_bits float_negate(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    /* NaN */
    if (exp == 0xFF && frac != 0) return f;

    /* Reassemble bits */
    return (~sign << 31) | (exp << 23) | frac;
}

int main()
{
    unsigned x = 0;
    while (x <= UINT_MAX)
    {
        assert(-u2f(x) == u2f(float_negate(x)));
        x++;
        printf("0x%X ", x);
    }
    return 0;
}
```


## 2.93 
Following the bit-level floating-point coding rules, implement the function with the following prototype:
```C
/* Compute |f|.  If f is NaN, then return f. */
float_bits float_absval(float_bits f);
```

For floating-point number f, this function computes |f|. If f is NaN, your function should simply return f .

Test your function by evaluating it for all $2^{32}$ values of argument f and comparing the result to what would be obtained using your machine’s floating-point operations.

**Solution**:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Compute -f.  If f is NaN, then return f. */
float_bits float_absval(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = 0;  // to align absolute value
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    /* NaN */
    if (exp == 0xFF && frac != 0) return f;

    /* Reassemble bits */
    return (sign << 31) | (exp << 23) | frac;
}

int main()
{
    unsigned x = 0;
    while (x <= UINT_MAX)
    {
        assert(fabs(u2f(x)) == u2f(float_absval(x))); // fabs() - calculate the absolute value of a floating-point value.
        x++;
        printf("0x%X \n", x);
    }
    return 0;
}
```


## 2.94 
Following the bit-level floating-point coding rules, implement the function with the following prototype:
```C
/* Compute 2*f.  If f is NaN, then return f. */
float_bits float_twice(float_bits f);
```

For floating-point number f , this function computes $2.0\times f$ . If f is NaN , your function should simply return f .

Test your function by evaluating it for all $2^{32}$ values of argument f and comparing the result to what would be obtained using your machine’s floating-point operations.

**Solution**:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Compute -f.  If f is NaN, then return f. */
float_bits float_twice(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;  // to align absolute value
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    /* NaN */
    if (exp == 0xFF && frac != 0) return f;

    /* Denormalized value */
    if (exp == 0)
    {
        frac = frac << 1;
    }
    /* infinity */
    else if (exp == 0xFE)
    {
        return f;
    }
    else exp += 1;

    /* Reassemble bits */
    return (sign << 31) | (exp << 23) | frac;
}

int main()
{
    unsigned x = 0;
    while (x < UINT_MAX)
    {
        assert((u2f(x))*2 == u2f(float_twice(x)));
        x++;
        printf("0x%X ", x);
    }
    return 0;
}

```


## 2.95 
Following the bit-level floating-point coding rules, implement the function with the following prototype:
```C
/* Compute 0.5*f.  If f is NaN, then return f. */
float_bits float_half(float_bits f);
```
For floating-point number f , this function computes $0.5\times f$ . If f is NaN , your function should simply return f.

Test your function by evaluating it for all $2^{32}$ values of argument f and comparing the result to what would be obtained using your machine’s floating-point operations.

**Solution**:
We will use rounding to even rule when we need to do rounding.
Here's some points we need to talk about:
In denormalized mode:
* Denormalized: e=0, M=f.
* when do halving, means f >> 1.
* because of rounding-to-even rule, we need to consider last 2 bits of f
	* if f = ...11, after f>>1, we need to add 1 to make it be even.
	* else, we just f >> 1, no need to add 1.
```C
/* Denormalized value */
    if (exp == 0)
    {
        frac = frac >> 1;
        frac += carrybit;
    }
```
In normalized mode when e = 1:
* there's one exception that is e=1, when do the halving, it will transit to the denormalized mode.
* in normalized mode: $V_n=(1+0.f_n)\times 2^{1-bias}$ when e = 1
* in denormalized mode: $V_d = 0.f_d\times 2^{1-bias}$
* so we get that, if we wanna do the halving, we only need to do 
	* firstly e >> 1 which makes e = 0.
	* secondly $1.f_n >> 1 = f_d$  to make $M_n\times 0.5$ = $M_d$
* Also we need to consider the round-to-even rule on  $1.f_n >> 1 = f_d$
```C
 /* Normalized transit to denormalized */
    else if (exp == 0x1)
    {
        exp = 0;
        frac = ((frac >> 1) | 0x400000) + carrybit;
    }
```
In normalized mode when e != 1:
```C
else exp -= 1;
```


Fullcode:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Compute 0.5*f.  If f is NaN, then return f. */
float_bits float_half(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;  // to align absolute value
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    /* NaN */
    if (exp == 0xFF && frac != 0) return f;

    int carrybit = (frac & 0x3) == 0x3; // When do round-to-even, if or not need to carry bit

    /* Denormalized value */
    if (exp == 0)
    {
        frac = frac >> 1;
        frac += carrybit;
    }
    /* Normalized transit to denormalized */
    else if (exp == 0x1)
    {
        exp = 0;
        frac = ((frac >> 1) | 0x400000) + carrybit;
    }
    else exp -= 1;

    /* Reassemble bits */
    return (sign << 31) | (exp << 23) | frac;
}

int main()
{
    unsigned x = 0;
    while (x < UINT_MAX)
    {
        assert((u2f(x)) / 2 == u2f(float_half(x)));
        x++;
        printf("0x%X ", x);
    }
    return 0;
}
```

## 2.96 ◆◆◆◆
Following the bit-level floating-point coding rules, implement the function with the following prototype:
```C
/*
 * Compute (int) f.
 * If conversion causes overflow or f is NaN, return 0x80000000
 */

int float_f2i(float_bits f);
```
For floating-point number f , this function computes (int) f . Your function should round toward zero. If f cannot be represented as an integer (e.g., it is out of range, or it is NaN), then the function should return 0x80000000.

Test your function by evaluating it for all $2^{32}$ values of argument f and com- paring the result to what would be obtained using your machine’s floating-point operations.

**Solution**:
Case 1: Normalized Values
* Pre-condition:  $exp \neq [00...0]or[11...1]$ 
* $V = (−1)^s × (1+0.f_{n-1}...f_1f_0) × 2^{[e_{k-1}...e_1e_0]-(2^{k-1}-1)}$
Case 2: Denormalized Values (非规格化)
* Pre-condition: $exp = [00...0]$
* $V = (−1)^s × 0.f_{n-1}...f_1f_0 × 2^{1-(2^{k-1}-1)}$
Case 3: Special Values
* Pre-condition: $exp = [11...1]$
* If $f=[00...0]$ and $s=0$, then $V = +\infty$ 
* If $f = [00...0]$ and $s=1$, then $V = -\infty$
* If $f = nonzero$, then $V=NaN$

Then we need to think about set checkpoint of 1 and $+\infty$
* \[-1,1\]: return 0 based on rounding to zero rule.
* \[1,INT_MAX\] or \[-1,INT_MIN\]: return based on rounding to zero rule.
* \[INT_MAX,$+\infty$\] or \[INT_MIN,$-\infty$\]: return 0x80000000
Checkpoint values on floating representations:
* Value 1 or -1: f = 0 and e = bias = 0x7F
* INT_MAX: $0x7FFFFFFF \approx 1.1...1\times2^{30} \approx 2^{31}$
* INT_MIN: $0x80000000 = 2^{31}$
* So when exp >= 31, it's overflow and we should return 0x80000000
* when $v==+\infty$, f=0 and e = 0xFF
Some ideas we get:
* value from -1 to +1 (all denormalized values and part of normalized values):
```C
if(e <= bias && e >= 0)
{
	result = 0;
}
```
* value from INT_MIN/INT_MAX to $-\infty$ or $+\infty$:
```C
if(e >= bias + 31)
{
	result = 0x80000000;
}
```
* value from INT_MIN to -1 and from +1 to INT_MAX (Normalized values):
```C
if(e > bias && e < bias + 31)
{
	E = e - bias;
	M = 0x800000 | f; // this M already shift n=23 bits
	//result = M >> 23 << E = M << E-23;
	if(E > 23) M << E - 23;
	else M >> 23 - E;
}
```

Integrate all code:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/*
 * Compute (int) f.
 * If conversion causes overflow or f is NaN, return 0x80000000
 */
int float_f2i(float_bits f)
{
    /* Decompose bit representation into parts */
    unsigned sign = f >> 31;  // to align absolute value
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned result = 0;
    int E;
    int bias = 0x7F;
    unsigned M;

    if (exp == 0xFF && frac != 0) return 0x80000000;

    
    if (exp < bias && exp >= 0)    // value from -1 to +1 (all denormalized values and part of normalized values)
    {
        result = 0;
        return result;
    }
    else if (exp >= bias + 31)      //  value from INT_MIN/INT_MAX to negative infinity or positive infinity:
    {
        result = 0x80000000;
        return result;
    }
    else // (e > bias && e < bias + 31): value from INT_MIN to -1 and from +1 to INT_MAX (Normalized values)
    {
        E = exp - bias;
        M = 0x800000 | frac; // this M already shift n=23 bits
        //result = M >> 23 << E = M << E-23;
        if (E > 23) result = M << E - 23;
        else result = M >> 23 - E;
    }


    /* Reassemble bits */
    return (sign << 31) | result;
}

int main()
{
    unsigned x = 0x3F800000;// 0 01111111 00000000000000000000000
    while (x < UINT_MAX)
    {
        float fx = *(float*) & x;
        int ix = float_f2i(x);
        assert((int)(fx) == ix);
        x++;
        printf("Original unsigned: 0x%08X,  Integer: %08d, Floating: %08f\n", x, ix, fx);
    }
    return 0;
}
```


## 2.97 ◆◆◆◆  
Following the bit-level floating-point coding rules, implement the function with the following prototype:
```C
/* Compute (float) i */
float_bits float_i2f(int i);
```
For argument i, this function computes the bit-level representation of (float) i.

Test your function by evaluating it for all $2^{32}$ values of argument f and comparing the result to what would be obtained using your machine’s floating-point operations.

**Solution**:
Our goal is to extract sign bit, exponent bits and fraction bits from the integer.
For sign bit, we need:
* extract the MSB
* one point to notice: for negative integer, it's two's complement encoding. We need to change it back to unsigned encoding and store the sign bit.
```C
if ((INT_MIN & i) == 0)
    {
        sign = 0;
        u = (unsigned)i;
    }
else{
        sign = 1;
        u = (unsigned)(~i + 1);
    }
```
For exponent bits, we need:
* calculate the bit width of the integer (width).
* then f width (frac_width) should be width - 1.
* E = exp - bias = width - 1, then exp = width - 1 + bias.
* because the range of floating representation is far beyond the the range of integer, we do not need to talk about overflow.
```C
exp = frac_width + bias;
```
For fraction bits (f), we need:
* f should be the i removing the MSB.
* some special conditions:
	* if frac_width > n, we need to take rounding (round-to-even) into consideration.
	* when doing round-to-even, in some cases, exp also need to add 1.
```C
if (frac_width < 24) // n = 23 is the precision of single-floating representation. No rounding required.
    {  
        frac = raw_frac << (23 - frac_width);
    }
else
	{
		frac = raw_frac >> (frac_width - 23);
	}
```


Rounding (3 cases - rounding to even):
* rounding_part >   rounding_halfflag
* rounding_part <   rounding_halfflag
* rounding_part == rounding_halfflag: rounding to even
```C
		if (rounding_part > rounding_halfflag)  // round to closest 
        {

            // frac = (frac + rounding_halfflag) & (~rounding_mask);
            raw_frac = raw_frac + rounding_halfflag;
            if (bit_width(raw_frac) > frac_width)     // if(there's carry bit) exp need to add
            {
                exp++;
                //rounding_mask = (rounding_mask << 1) + 1;
                raw_frac = raw_frac & (~rounding_mask);
                //frac_width++;
            }
            
        }
        else if (rounding_part < rounding_halfflag) // round to closest
        {
            raw_frac = raw_frac & (~rounding_mask);
        }
        else // round to even
        {
            unsigned odd_bit = raw_frac & (~rounding_mask) & ((rounding_mask << 1) + 1);
            if (odd_bit)
            {
                raw_frac = raw_frac + rounding_part;
                if (bit_width(raw_frac) > frac_width)     // if(there's carry bit) exp need to add
                {
                    exp++;
                    //rounding_mask = (rounding_mask << 1) + 1;
                    raw_frac = raw_frac & (~rounding_mask);
                    //frac_width++;
                }
            }
            else
            {
                raw_frac = raw_frac & (~rounding_mask);
            }
        }
```

Full code:
```C
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

unsigned bit_width(unsigned ux)
{
    if ((ux & INT_MIN) != 0)
    {
        return 0x20;
    }

    unsigned width = 0;
    while (ux != 0)
    {
        width++;
        ux = ux >> 1;
    }
    return width;
}

unsigned bit_mask(unsigned x)
{
    unsigned w = sizeof(unsigned) << 3;
    if (x == 0) return 0;
    return (unsigned)(-1) >> (w - x);
}

float u2f(unsigned x)
{
    unsigned* p_x = &x;
    return *(float*)p_x;
}

unsigned f2u(float f)
{
    return *(unsigned*)&f;
}

/* Generate fraction bits based on the u.*/
/* For example, if u = 0b 11, then the function will return 0b 1100 0000 ... 0000*/
unsigned frac_gen(unsigned u)
{
    unsigned ret = u;
    while ((0x80000000 & ret) == 0)
    {
        ret = ret << 1;
    }
    return ret;
}

/* Compute (float) i */
float_bits float_i2f(int i)
{
    /* Decompose bit representation into parts */
    unsigned sign;
    unsigned frac;
    unsigned exp;

    /* Other useful local variables */
    unsigned width;     // bit width of integer i
    unsigned u;         // usigned representation of integer i
    int bias = 0x7F;    // k = 8
    unsigned frac_mask; // frac = frac_mask & u
    unsigned frac_width;// the width of frac
    unsigned raw_frac;// fraction on the rightmost.
    unsigned rounding_part, rounding_mask, rounding_halfflag;
    float_bits result;

    if ((INT_MIN & i) == 0)
    {
        sign = 0;
        u = (unsigned)i;
    }
    else {
        sign = 1;
        u = (unsigned)(~i + 1);
    }

    // exception: i == 0
    if (i == 0) {
        frac = 0;
        exp = 0;
        return sign << 31 | exp << 23 | frac;
    }
    // exception; i == INT_MIN
    else if (i == INT_MIN) {
        sign = 1;
        exp = bias + 31;
        frac = 0;
        return sign << 31 | exp << 23 | frac;
    }

    width = bit_width(u);
    frac_width = width - 1;
    frac_mask = bit_mask(frac_width);
    raw_frac = u & frac_mask;

    if (frac_width < 24) // n = 23 is the precision of single-floating representation. No rounding required.
    {  
        exp = frac_width + bias;
        frac = raw_frac << (23 - frac_width);
    }
    else    // consider rounding-to-even rule
    {
        exp = frac_width + bias;
        rounding_mask = bit_mask(frac_width - 23);
        rounding_part = rounding_mask & raw_frac;
        rounding_halfflag = (rounding_mask + 1) >> 1;
        if (rounding_part > rounding_halfflag)  // round to closest 
        {

            // frac = (frac + rounding_halfflag) & (~rounding_mask);
            raw_frac = raw_frac + rounding_halfflag;
            if (bit_width(raw_frac) > frac_width)     // if(there's carry bit) exp need to add
            {
                exp++;
                //rounding_mask = (rounding_mask << 1) + 1;
                raw_frac = raw_frac & (~rounding_mask);
                //frac_width++;
            }
            
        }
        else if (rounding_part < rounding_halfflag) // round to closest
        {
            raw_frac = raw_frac & (~rounding_mask);
        }
        else // round to even
        {
            unsigned odd_bit = raw_frac & (~rounding_mask) & ((rounding_mask << 1) + 1);
            if (odd_bit)
            {
                raw_frac = raw_frac + rounding_part;
                if (bit_width(raw_frac) > frac_width)     // if(there's carry bit) exp need to add
                {
                    exp++;
                    //rounding_mask = (rounding_mask << 1) + 1;
                    raw_frac = raw_frac & (~rounding_mask);
                    //frac_width++;
                }
            }
            else
            {
                raw_frac = raw_frac & (~rounding_mask);
            }
        }
        frac = raw_frac >> (frac_width - 23);
        frac = frac & 0x7FFFFF;
    }

    return sign << 31 | exp << 23 | frac;
}

int main()
{
    //int ix = INT_MIN + 1;// 0 01111111 00000000000000000000000
    //int ix = INT_MIN+1;
    //int ix = 0;
    int ix = INT_MAX - 0x100;
    while (ix < INT_MAX)
    {
        float_bits fbx = float_i2f(ix);
        float fx = *(float*)&fbx;
        float fix = (float)ix;
        assert(fix == fx);
        printf("ix value: %08d, Floating from float_i2f: %08f, Floating from casting: %08f\n",ix, fx, fix);
        ix++;
    }
    return 0;
}
```













