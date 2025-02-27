* Rather than accessing individual bits in memory, most computers use blocks of 8 bits, or bytes, as the smallest addressable unit of memory. 
* A machine-level program views memory as a very large array of bytes, referred to as ==virtual memory==. Every byte of memory is identified by a unique number, known as its address, and the set of all possible addresses is known as the ==virtual address space==.
  * As indicated by its name, this virtual address space is just a conceptual image presented to the machine-level program.
* In subsequent chapters, we will cover how the compiler and run-time system partitions **this memory space into more manageable units to store the different program objects**, that is, program data, instructions, and control information.
* Various mechanisms are used to allocate and manage the storage for different parts of the program. This management is all performed **within the virtual address space**.
* Example (C pointer): 
	* Value of a pointer in C - the virtual address of the first byte of some block of storage, no matter whether it points to an integer, a structure, or some other program object.
	* Type of a pointer in C - C compiler also associates type information with each pointer, so that it can generate different machine-level code to access the value stored at the location designated by the pointer depending on the type of that value.
    ```
    int a[10]; // a's value is a virtual address. Type int: a+4
    char b[10]; // b's value is also a virtual address. Type char: b+1
    ```
	* Although the C compiler maintains this type information, the actual machine-level program it generates has no information about data types. It simply treats each program object as a block of bytes and the program itself as a sequence of bytes.

# 2.1.1 Hexadecimal Notation
* Reason for hexadecimal notation, a single byte consists of 8 bits. 
	* In binary notation, its value ranges from 0B 00000000 to 0B 11111111 - too complex.
	* In decimal, its value ranges from 0 to 255. - No bit patterns' description.
	* Neither notation is very convenient for describing bit patterns.
	* Hexadecimal notation can be simple and better descriptive of bit patterns at the same time.
* Structure of hexadecimal
	* Hexadecimal (or simply “hex”) uses digits ‘0’ through ‘9’ along with characters ‘A’ through ‘F’ to represent 16 possible values.
	* In C, numeric constants starting with 0x or 0X are interpreted as being in hexadecimal.
	* The characters ‘A’ through ‘F’ may be written in either upper- or lowercase.
	```
	  0x1FA6 or 0x1fa6
	```
* A common task in working with machine-level programs is to manually convert between decimal, binary, and hexadecimal representations of bit patterns:
	![[./2_1.assets/Screenshot 2023-10-07 at 13.14.01.png]]
* Example of converting hex to binary:
	```
	  Hex: 0x 567abe4f
	  Binary: 0b 0101 0110 0111 1010 1011 1110 0100 1111
	```
* Some tricks: $x = 2^n$
	* Suppose: n = i + 4*j
	* Then we can write x with a leading hex digit of 1 (i = 0), 2 (i = 1), 4 (i = 2), or 8 (i = 3), followed by j hexadecimal 0s.
	* Example:
		* $2^{11}$ -> 11 = 3 + 4\*2 -> i=3,j=2 -> 0x800
		* $2^{45}$ -> 45 = 1 + 4\*11 -> i=1,j=11 -> 0x200000000000

# Practice Problem 2.1
Perform the following number conversions:

A. 0x25B9D2 to binary
B. binary 1010111001001001 to hexadecimal
C. 0xA8B3D to binary
D. binary 1100100010110110010110 to hexadecimal

**Solution**:
​	A. 0x25B9D2 to binary
```
0x25B9D2 = 0b 0010 0101 1011 1001 1101 0010
```
​	B. binary 1010111001001001 to hexadecimal
```
0b 1010 1110 0100 1001 = 0xAE49
```
​	C. 0xA8B3D to binary
```
0xA8B3D = 0b 1010 1000 1011 0011 1101
```
​	D. binary 1100100010110110010110 to hexadecimal
```
0b 11 0010 0010 1101 1001 0110 = 0x322D96
```

* Converting between decimal and hexadecimal representations requires using multiplication or division to handle the general case.
* Decimal -> hexadecimal representations:
	* To convert a decimal number x to hexadecimal, we can repeatedly divide x by 16, giving a quotient q and a remainder r, such that x = q . 16 + r.
	* We then use the hexadecimal digit representing r as the least significant digit and generate the remaining digits by repeating the process on q.
	![[./2_1.assets/Screenshot 2023-10-07 at 17.47.06.png|200]]
	314156 = 0x 4cb2c
    ```
    For decimal: 500000, how to get its hex value?
    500000 = 31250 * 16 + 0	(0)
    31250 = 1953 * 16 + 2	(2)
    1953 = 122 * 16 + 1		(1)
    122 = 7 * 16 + 10		(A)
    7										(7)
    
    decimal 500000 = hex 0x 7a120
    ```

```c
// decToHex.c
// Require customer to input a decimal, then program will translate to hex and print.

#include<stdio.h>

int main()
{
	int i_inputNum;	// store the input number
	char alphabet[0x10] = "0123456789ABCDEF";	// alphabet table for hex
	char result[0x100];
	int length;	// the length of result array

	printf("This program will convert a decimal you input to a hex accordinglly.\n");
	printf("Please input a decimal number:");
	scanf("%d",&i_inputNum);

	if(i_inputNum == 0) printf("0x0");

	// Calculate the result and store into result array
	for(int i = 0;i_inputNum!=0;i++)
	{
		result[i] = alphabet[i_inputNum%0x10];// repeatly divide input number by 16
						      // record the remainder into result[i]
		i_inputNum /= 0x10;	// get the quotient
		length = i;
	}	

	// Print the result
	printf("The hex result: 0x");
	for(int i = length; i >= 0; i--) printf("%c",result[i]);
	printf("\n");

	return 0;
}

```

* Hexadecimal -> decimal representations:
	* Multiply each of the hexadecimal digits by the appropriate power of 16.
	* Example:
		* hex: 0x8AF
		* dec: $8 \times 16^2 + 10 \times 16^1 + 15 \times 16^0 = 1967$

# Practice Problem 2.3
A single byte can be represented by 2 hexadecimal digits. Fill in the missing entries in the following table, giving the decimal, binary, and hexadecimal values of different byte patterns:
![[./2_1.assets/Screenshot 2023-10-08 at 22.09.25.png|200]]

![[./2_1.assets/Screenshot 2023-10-08 at 22.10.44.png|200]]

**Solution**:

```
Decimal			    Binary				  Hexadecimal
158					0b 1110 1001				0x E9
76					0b 1100 0100				0x C4
145					0b 0001 1001				0x 19
174					0b 1010 1110				0x AE
60					0b 0011 1100				0x 3C
241					0b 1111 0001				0x F1
```

```
Decimal 			    Binary 			Hex
117						0b 0111 0101	0x75
189						0b 1011 1101    0xBD
245						0b 1111 0101	0xF5
```

# Practice Problem 2.4
Without converting the numbers to decimal or binary, try to solve the following arithmetic problems, giving the answers in hexadecimal. Hint: Just modify the methods you use for performing decimal addition and subtraction to use base 16.
A. 0x605c + 0x5 = ?
B. 0x605c − 0x20 = ?
C. 0x605c + 32 = ?
D. 0x60fa − 0x605c = ?

**Solution**:
A. 0x605c + 0x5 = 0x6062

B. 0x605c − 0x20 = 0x603c

C. 0x605c + 32(0x20) = 0x607c

D. 0x60fa − 0x605c = 0x 9e
# 2.1.2 Data Sizes
* Word Size:
	* Every computer has ==a word size==, indicating the nominal size of pointer data. 
	* Since a virtual address is encoded by such a word, the most important system parameter determined by the word size is **the maximum size of the virtual address space**. 
	    ```
	    suppose word size is 8 bits
	    virtual address space range: 00000000 ~ 11111111
	    ```
	* For a machine with a w-bit word size (32位或64位系统), the virtual addresses can range from 0 to $2^w − 1$, giving the program access to at most $2^w$ bytes.
* 32bit and 64bit:
	* 32-bit word size - 4 gigabytes
	* 64-bit word size - 16 exabytes
* Compile:
  ```
  linux> gcc -m32 prog.c
  ```
  * This program will run correctly on either a 32-bit or a 64-bit machine.
  ```
  linux> gcc -m64 prog.c
  ```
  * This program will only run on a 64-bit machine.

* We will therefore refer to programs as being either “32-bit programs” or “64-bit programs,” since the distinction lies in **how a program is compiled**, rather than the type of machine on which it runs.
* Computers and compilers support ==multiple data formats== using different ways to encode data, such as integers and floating point, as well as different lengths.
* The C language supports multiple data formats for both integer and ﬂoating-point data.
	![[./2_1.assets/Screenshot 2023-10-08 at 23.04.37.png|300]]
	* To avoid the vagaries of relying on “typical” sizes and different compiler settings, ISO C99 introduced a class of data types where the data sizes are fixed regardless of compiler and machine settings. Among these are data types int32_t and int64_t, having **exactly 4 and 8 bytes**.
	* Most of the data types encode signed values, unless prefixed by the keyword unsigned or using the specific unsigned declaration for fixed-sized data types. 
		```
		    unsigned int 
		```
* The C language allows a variety of ways to order the keywords and to include or omit optional keywords. 
    ```c
    unsigned long 
    unsigned long int 
    long unsigned 
    long unsigned int
    ```
    * A pointer uses the **full word size** of the program.
	* Programmers should strive to make their programs portable across different machines and compilers. One aspect of portability is to make the program insensitive to the exact sizes of the different data types.

# 2.1.3 Addressing and Byte Ordering (寻址和字节顺序)
* In virtually all machines, a multi-byte object is stored as a contiguous sequence of bytes, with the address of the object given by the smallest address of the bytes used.
* Example:
	* A variable x of type int has address 0x100. So the value of &x is 0x100, which is address of variable x.
	* Code:
	```c
	      // varAdr.c
	      int x = 5;
	      printf("The address of variable x is: %p \n", &x)
	```
	![[./2_1.assets/Screenshot 2023-10-19 at 09.45.05.png|400]]
* 2 common conventions of ordering the bytes representing an object:
	* little endian(小端排序) - the least significant byte(最低有效字节) comes first from lower address
	* big endian(大端排序) - the most significant byte (最高有效字节) comes first from lower address
	* Example:
		* For variable x of type in and at address 0x100 has a hexadecimal value of 0x01234567
		![[./2_1.assets/Screenshot 2023-10-09 at 12.30.54.png|400]]
		![[./2_1.assets/Screenshot 2023-10-09 at 12.31.28.png|400]]
```c
// big_small_end.c
#include <stdio.h>
#include <stdint.h>

int main()
{
	uint32_t x = 0x01234567;
	uint32_t* x_adr = &x;
	printf("The value of variable x is: 0x%08x \n",x);
	printf("The address of variable x is: %p\n",x_adr);
	printf("The 1st byte value of x address is: 0x%02x \n",*(char*)x_adr);
	printf("The 2nd byte value of x address is: 0x%02x \n",*((char*)x_adr+1));
	printf("The 3rd byte value of x address is: 0x%02x \n",*((char*)x_adr+2));
	printf("The 4th byte value of x address is: 0x%02x \n",*((char*)x_adr+3));
	return 0;
}
```
![[./2_1.assets/Screenshot 2023-10-19 at 09.55.12.png|500]]
* Most Intel-compatible machines operate exclusively in little-endian mode. On the other hand, most machines from IBM and Oracle (arising from their acquisition of Sun Microsystems in 2010) operate in big-endian mode.
* Possible issues of differences on little-endian and big-endian mode:
	1. Binary data are communicated over a network between different machines.
		* To avoid such problems, code written for networking applications must follow established conventions for byte ordering to make sure the sending machine converts its internal representation to the network standard.
	2. When looking at the byte sequences representing integer data, which occurs often when inspecting machine-level programs.
		* Example:
		```
		4004d3: 01 05 43 0b 20 00			add %eax,0x200b43(%rip)
		```
		* eax -> (0x200b43 + rip)
		* Operation: adds a word of data to the value stored at an address computed by adding 0x200b43 to the current value of the program counter.
	```
	Final 4 bytes: 
	4004d5: 43 0b 20 00
	If little-endian, the value is: 0x 00 20 0b 43
	If big-endian, the value is: 		0x 43 0b 20 00
	```
	3. Using a cast(强制类型转换) or a union to allow an object to be referenced according to a different data type from which it was created.
		```c
		// cast.c
		#include <stdio.h> 
		
		typedef unsigned char* byte_pointer; 
		
		void show_bytes(byte_pointer start, size_t len) {
			int i;
			for(i = 0; i < len; i++) printf(" %.2x", start[i]);
			printf("\n"); 
		} 
		
		void show_int(int x) {
			show_bytes((byte_pointer) &x, sizeof(int)); 
		} 
		
		void show_float(float x) {
			show_bytes((byte_pointer) &x, sizeof(float)); 
		} 
			
		void show_pointer(void *x) {
			show_bytes((byte_pointer) &x, sizeof(void *)); 
		}
		
		void test_show_bytes(int val) { 
		  int ival 	= val; 
		  float fval = (float) ival;
			int	*pval	=	&ival;
			show_int(ival); 
		  show_float(fval); 
		  show_pointer(pval);
		}
		```
		* show_bytes is given the address of a sequence of bytes, indicated by a byte pointer, and a byte count. 
		* show_int, show_float, and show_pointer demonstrate how to use procedure show_bytes to print the byte representations of C program objects of type int, float, and void\*.
		![[./2_1.assets/Screenshot 2023-10-10 at 23.29.00.png]]
		* Decimal: 12345 = Hex: 0x 00003039
			* Linux 32, Windows, and Linux 64, indicating little-endian machines.
			* Sun, indicating a big-endian machine.
		* Float: 12345.0 = Hex: 0x 4640E400
			* the bytes of the float data are identical, except for the byte ordering.
		* Pointer: &ival -> totally different.
			* Linux 32, Windows, and Sun machines use 4-byte addresses, while the Linux 64 machine uses 8-byte addresses.
		* Observations:
			* the ﬂoating-point and the integer data both encode the numeric value 12,345, they have very different ==byte patterns== and use different ==encoding schemes==.
			![[./2_1.assets/Screenshot 2023-10-10 at 23.42.36.png|300]]

# Practice Problem 2.5
Consider the following three calls to show_bytes:
int a = 0x12345678;
byte_pointer ap = (byte_pointer) &a;
show_bytes(ap, 1); /\* A. \*/
show_bytes(ap, 2); /\* B. \*/
show_bytes(ap, 3); /\* C. \*/

Indicate the values that will be printed by each call on a little-endian machine

A. Little endian: ? Big endian: ?
B. Little endian: ? Big endian: ?
C. Little endian: ? Big endian: ?

**Solution**:
A. Little endian: 0x 78 		Big endian: 0x 12

B. Little endian: 0x 78 56		Big endian: 0x 12 34

C. Little endian: 0x 78 56 34 	Big endian: 0x 12 34 56

```c
//cast.c
#include <stdio.h>

int main()
{
	int a = 0x12345678; 

	byte_pointer ap = (byte_pointer) &a; 

	show_bytes(ap, 1); /* A. */ 

	show_bytes(ap, 2); /* B. */ 

	show_bytes(ap, 3); /* C. */
	
	return 0;
}
```

# Practice Problem 2.6
Using show_int and show_float, we determine that the integer 2607352 has hexadecimal representation 0x0027C8F8, while the ﬂoating-point number 2607352.0(original wrong point) has hexadecimal representation 0x4A1F23E0.

A. Write the binary representations of these two hexadecimal values.
B. Shift these two strings relative to one another to maximize the number of matching bits. How many bits match?
C. What parts of the strings do not match?

**Solution**:
A. Write the binary representations of these two hexadecimal values.

Show hex code:
```c
// P2_6.c
#include <stdio.h>
#include <string.h>

typedef unsigned char* byte_pointer; 

void show_bytes(byte_pointer start, size_t len) {
	int i;
	printf("Show %ld bytes from address: %p\n",len,start);
	for(i = 0; i < len; i++) 
		printf("\t%p: %.2x\n",start+i, start[i]);
	printf("\n"); 
} 

void show_int(int x) {
	show_bytes((byte_pointer) &x, sizeof(int)); 
} 
	
void show_float(float x) {
	show_bytes((byte_pointer) &x, sizeof(float)); 
} 
	
void show_pointer(void *x) {
	show_bytes((byte_pointer) &x, sizeof(void *)); 
}

int main()
{
	char str_type[0x10] = {0};
	printf("Please choose the kind of conversion (int/float/pointer): ");
	scanf("%s",str_type);
	if(!strcmp(str_type,"int"))
	{
		int tgt_int;
		printf("Please input the target integer:");
		scanf("%d",&tgt_int);
		show_int(tgt_int);
	}
	else if(!strcmp(str_type,"float"))
	{
		float tgt_float;
		printf("Please input the target float:");
		scanf("%f",&tgt_float);
		show_float(tgt_float);
	}
	else if(!strcmp(str_type,"pointer"))
	{
		int val = 0;
		printf("Please input a number then its pointer will be calculated:");
		scanf("%d",&val);
		void* tgt_pointer = (void*)&val;
		show_pointer(tgt_pointer);
	}
	else
	{
		printf("Wrong inputs!\n");
	}
	return 0;
}
```

```
Decimal: 2607352 Hex: 0x0027C8F8
0b 0000 0000 0010 0111 1100 1000 1111 1000
Float: 2607352.0 Hex: 0x4a1f23e0
0b 0100 1010 0001 1111 0010 0011 1110 0000
```

B. Shift these two strings relative to one another to maximize the number of matching bits. How many bits match?

```
Decimal: 2607352 Hex: 0x0027C8F8
0b 00000000001001111100100011111000
Float: 2607352.0 Hex: 0x4a1f23e0
0b   01001010000111110010001111100000

21 bits match.
```

C. What parts of the strings do not match?

Except the highest bit, the other bits of integer are included in the float bits.

# 2.1.4 Representing Strings
* A string in C is encoded by an array of characters terminated by the **null** (having value 0) character.
* Example:
```c
show_bytes("12345",6);
```
![[./2_1.assets/Screenshot 2023-10-11 at 12.53.45.png]]
* the ASCII code for decimal digit x happens to be 0x3x, and that the terminating byte has the hex representation 0x00.

# Practice Problem 2.7
What would be printed as a result of the following call to show_bytes?
```c
const char *m = "mnopqr";
show_bytes((byte_pointer) m, strlen(m));
```

Note that letters ‘a’ through ‘z’ have ASCII codes 0x61 through 0x7A.

**Solution**:
![[./2_1.assets/Screenshot 2023-10-04 at 15.27.00.png]]

prints 6D 6E 6F 70 71 72.

# 2.1.5 Representing Code
* Example:
	```c
	int sum(int x, int y) { 
		return x + y; 
	}
	```
	* how to check its byte representations:
		```
		$ gcc -og -c main.c -o main.o
		$ objdump -d main.o
		```
		* arm cpu:
		![[./2_1.assets/Screenshot 2023-10-11 at 13.11.53.png|350]]
		* X86 cpu:
		![[./2_1.assets/Screenshot 2023-10-11 at 13.12.54.png|350]]
	* Highlights:
		* Instruction codings are different. Different machine types use different and incompatible instructions and encodings. Even identical processors running different operating systems have differences in their coding conventions and hence are not binary compatible.
	* **Binary code is seldom portable across different combinations of machine and operating system**.
	* A program, from the perspective of the machine, is simply a sequence of bytes. 
	* The machine has no information about the original source program, except perhaps some auxiliary tables maintained to aid in debugging.
		
# 2.1.6 Introduction to Boolean Algebra
* Boolean algebra - logic values true and false as binary values 1 and 0.
* Operations of boolean algebra - NOT, AND, OR, EXCLUSIVE-OR:
	![[./2_1.assets/Screenshot 2023-10-11 at 13.30.16.png|350]]
	* ~ corresponds to the logical operation not, denoted by the symbol ¬ (命题逻辑). That is, we say that ¬P is true when P is not true, and vice versa. ~p equals 1 when p equals 0, and vice versa.
	* & corresponds to the logical operation and, denoted by the symbol ∧. We say that P ∧ Q holds when both P is true and Q is true. Correspondingly, p & q equals 1 only when p = 1 and q = 1.
	* | corresponds to the logical operation or, denoted by the symbol ∨. We say that P ∨ Q holds when either P is true or Q is true. Correspondingly, p | q equals 1 when either p = 1 or q = 1.
	* ^ corresponds to the logical operation exclusive-or, denoted by the symbol ⊕ . We say that P ⊕ Q holds when either P is true or Q is true, but not both. Correspondingly, p ^ q equals 1 when either p = 1 and q = 0, or p = 0 and q = 1.
* We can extend the four Boolean operations to also operate on bit vectors(位向量运算), strings of zeros and ones of some fixed length w.
* Example:
	* Let a and b denote the bit vectors $[a_{w−1}, a_{w−2}, . . . , a_0]$] and $[b_{w−1} , b_{w−2} , . . . , b_0 ]$, respectively. 
	* We define a & b to also be a bit vector of length w, where the i-th element equals a i& b i, for 0 ≤ i < w.
	* Suppose w=4,a= \[0110\],b=\[1100\]:
	![[./2_1.assets/Screenshot 2023-10-11 at 13.53.27.png|350]]

# Practice Problem 2.8
Fill in the following table showing the results of evaluating Boolean operations on bit vectors.
![[Pasted image 20241021231815.png|150]]

**Solution**:

```
a 	= [01001110]
b 	= [11100001]
~a 	= [10110001]
~b	= [00011110]
a&b = [01000000]
a|b = [11101111]
a^b = [10101111]
```

* One useful application of bit vectors is to represent finite sets（有限集合）.
	* We can encode any subset A ⊆ { 0, 1, . . . , w − 1 } with a bit vector $[a_{w−1} , . . . , a_1 , a_0]$, where a i = 1 if and only if i ∈ A.
	* Example:
		* bit vector a = [01101001]encodes the set A = { 0, 3, 5, 6 } 
		* bit vector b = [01010101]encodes the set B = { 0, 2, 4, 6 } 
	* operations | and & correspond to set ==union(并)== and ==intersection(交)==.
	* ~ corresponds to set ==complement（补）==.
	* Example:
		a 	  = [01001110] -> A = {1,2,3,6}
		b 	  = [11100001] -> B = {0,5,6,7}
		~a 	  = [10110001] -> $\overline{A}$ = {0,4,5,7}
		~b	  = [00011110] -> $\overline{B}$ = {1,2,3,4}
		a&b   = [01000000] -> A$\cap$B = { 6 }
		a|b   = [11101111] -> A$\cup$B = {0,1,2,3,5,6,7}
		a^b   = [10101111] -> A$\bigoplus$B = {0,1,2,3,5,7}

# Practice Problem 2.9
Computers generate color pictures on a video screen or liquid crystal display by mixing three different colors of light: red, green, and blue. Imagine a simple scheme, with three different lights, each of which can be turned on or off, projecting onto a glass screen:
![[./2_1.assets/Screenshot 2023-10-11 at 14.16.50.png|350]]
We can then create eight different colors based on the absence (0) or presence (1) of light sources R, G, and B:
![[./2_1.assets/Screenshot 2023-10-11 at 14.17.50.png|200]]

Each of these colors can be represented as a bit vector of length 3, and we can apply Boolean operations to them.
A. The complement of a color is formed by turning off the lights that are on and turning on the lights that are off. What would be the complement of each of the eight colors listed above?
B. Describe the effect of applying Boolean operations on the following colors:
Blue | Green  = ?
Yellow & Cyan = ?
Red ^ Magenta = ?

**Solution**:
A.
```
black = [0,0,0] -> ~black = [1,1,1] = white
blue  = [0,0,1] -> ~blue = [1,1,0] = yellow
green = [0,1,0] -> ~green = [1,0,1] = magenta
cyan(青色)  = [0,1,1] -> ~cyan [1,0,0] = red
red   = [1,0,0] -> ~red = cyan
magenta(洋红) = [1,0,1] -> ~magenta = green
yellow = [1,1,0] -> ~yellow = blue
white  = [1,1,1] -> ~white = black
```

B.
```
Blue | Green = [001]|[010] = [011] = Cyan

Yellow & Cyan = [110]&[011] = [010] = Green

Red ^ Magenta = [100]^[101] = [001] = Blue
```

# 2.1.7 Bit-Level Operations in C
* C supports bitwise Boolean operations.
	```
	| for or 	// 或
	& for and	// 与
	~ for not	// 取反
	^ for exclusive-or	// 异或
	```
	![[./2_1.assets/Screenshot 2023-10-11 at 17.10.22.png|450]]
	* The best way to determine the effect of a bit-level expression is to expand the hexadecimal arguments to their binary representations, perform the operations in binary, and then convert back to hexadecimal.

# Practice Problem 2.10
As an application of the property that a ^ a = 0 for any bit vector a, consider the following program:

```c
void inplace_swap(int *x, int *y) {
	*y = *x ^ *y;	// Step 1
	*x = *x ^ *y;	// Step 2
	*y = *x ^ *y;	// Step 3
}
```

As the name implies, we claim that the effect of this procedure is to swap the values stored at the locations denoted by pointer variables x and y. Note
that unlike the usual technique for swapping two values, we do not need a third location to temporarily store one value while we are moving the other. There is
no performance advantage to this way of swapping; it is merely an intellectual amusement.

Starting with values a and b in the locations pointed to by x and y, respectively, fill in the table that follows, giving the values stored at the two locations after each step of the procedure. Use the properties of ^ to show that the desired effect is achieved. Recall that every element is its own additive inverse (that is, a ^ a = 0).
![[Pasted image 20241021232916.png|250]]

**Solution**:
* Purpose: swap the values stored at the locations denoted by pointer variables x and y. 
* Do not need a third location to temporarily store one value while we are moving the other.
```
Step		  *x	*y
Initially     a     b
Step1		  a     a^b	
Step2		  b	    a^b
Step3		  b     a
```

# Practice Problem 2.11
Armed with the function `inplace_swap` from Problem 2.10, you decide to write code that will reverse the elements of an array by swapping elements from opposite
ends of the array, working toward the middle.

You arrive at the following function:
```c
void reverse_array(int a[], int cnt) {
	int first, last;
	for(first = 0, last = cnt - 1; first <= last;first++,last--)
    inplace_swap(&a[first], &a[last]);
}
```

When you apply your function to an array containing elements 1, 2, 3, and 4, you find the array now has, as expected, elements 4, 3, 2, and 1. When you try it on an array with elements 1, 2, 3, 4, and 5, however, you are surprised to see that the array now has elements 5, 4, 0, 2, and 1. In fact, you discover that the code always works correctly on arrays of even length, but it sets the middle element to 0 whenever the array has odd length.

A. For an array of odd length cnt = 2k + 1, what are the values of variables first and last in the final iteration of function reverse_array?

B. Why does this call to function `inplace_swap` set the array element to 0?

C. What simple modiﬁcation to the code for reverse_array would eliminate this problem?

**Solution**:
```c
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
	for(first = 0, last = cnt - 1; first <= last; first++, last--)
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
```

```
gdb debugging:
gcc -g test.c -o test	// add debugging symbols to test program
gdb test	// start gdb to debug test program
l 1,26	// list the code from line 1 to 26
b 14		// add breakpoint on line 14
r				// start run the code from first line
p test[2]	// check variable content while hit breakpoint
```

![[./2_1.assets/Screenshot 2023-10-12 at 17.27.39.png|500]]

B.
![[./2_1.assets/Screenshot 2023-10-12 at 17.44.00.png|500]]

So \*x ^ \*y = 0, which make it wrong result!

C.
==first <= last== ->==first < last==

```c
void reverse_array(int a[], int cnt)
{
	int first, last;
	for(first = 0, last = cnt - 1; first < last; first++, last--)
		inplace_swap(&a[first],&a[last]);
}
```

* One common use of bit-level operations is to implement ==masking operations==, where a mask is a bit pattern that indicates a selected set of bits within a word. 
	* The mask 0xFF indicates the low-order byte of a word. The bit-level operation x & 0xFF yields a value consisting of the least significant byte of x, but with all other bytes set to 0.
	* with x = 0x89ABCDEF, the expression would yield 0x000000EF.

# Practice Problem 2.12
Write C expressions, in terms of variable x, for the following values. Your code should work for any word size w ≥ 8. For reference, we show the result of evaluating the expressions for x = 0x87654321, with w = 32.

A. The least significant byte of x, with all other bits set to 0. [0x00000021]

B. All but the least signiﬁcant byte of x complemented, with the least signiﬁcant byte left unchanged. [0x789ABC21]

C. The least signiﬁcant byte set to all ones, and all other bytes of x left unchanged. [0x876543FF]

**Solution**:
A.
x & 0xFF

B.
x ^ ~0x000000FF

C.
x | 0x000000FF

# Practice Problem 2.13
The Digital Equipment VAX computer was a very popular machine from the late 1970s until the late 1980s. Rather than instructions for Boolean operations and and or, it had instructions bis (bit set) and bic (bit clear). Both instructions take a data word x and a mask word m. They generate a result z consisting of the bits of x modiﬁed according to the bits of m. With bis, the modiﬁcation involves setting z to 1 at each bit position where m is 1. With bic, the modiﬁcation involves setting z to 0 at each bit position where m is 1.

To see how these operations relate to the C bit-level operations, assume we have functions bis and bic implementing the bit set and bit clear operations, and that we want to use these to implement functions computing bitwise operations | and ^, without using any other C operations. Fill in the missing code below. Hint: Write C expressions for the operations bis and bic.

```c
/* Declarations of functions implementing operations bis and bic */ 
int bis(int x, int m); 
int bic(int x, int m);

/* Compute x|y using only calls to functions bis and bic */ 
int bool_or(int x, int y) {
	int result = bis(x,y);
	return result; 
}

/* Compute x^y using only calls to functions bis and bic */ 
int bool_xor(int x, int y) {
	int result = bis(bic(x,y), bic(y,x));
	return result; 
}
```

**Solution**:
bic(x,m) = x & ~m

bis(x,m) = x | m

xor formula:  x ^ y = (x & ~y) | (~x & y)

x ^ y = bis(bic(x,y), bic(y,x))

# 2.1.8 Logical Operations in C
* C also provides a set of logical operators ||, &&, and !, which correspond to the or, and, and not operations of logic.
* Differences between logical operations(|| && !) and bit-level operations (| & ~):
	1. The logical operations treat any **nonzero argument as representing true and argument 0 as representing false**.
	    * View 0 as "False", while anything nonzero as "True"
		* The logical operations return either 1 or 0, indicating a result of either true or false.
		* Example:
		![[./2_1.assets/Screenshot 2023-10-12 at 19.33.14.png|180]]
	2. A second important distinction between the logical operators ‘&&’ and ‘||’ versus their bit-level counterparts ‘&’ and ‘|’ is that **the logical operators do not evaluate their second argument if the result of the expression can be determined by evaluating the first argument**. 
		* Example:
			* a && 5/a will never cause a division by zero
			* p && \*p will never cause the dereferencing of a null pointer.

# Practice Problem 2.14
Suppose that a and b have byte values 0x55 and 0x46, respectively. Fill in the following table indicating the byte values of the different C expressions:

![[Pasted image 20241022083923.png|350]]

**Solution**:
```
a  = 0b 0101 0101
b  = 0b 0100 0110
~a = 0b 1010 1010
~b = 0b 1011 1001

a & b = 0b 0101 0101 & 0b 0100 0110 = 0b 0100 0100 = 0x44
a && b = 1

a | b = 0b 0101 0111
a || b = 1

~a | ~b = 0b 1011 1011 = 0xBB					
!a || !b = 0

a & !b = 0					a && ~b = 1
```

# Practice Problem 2.15
Using only bit-level and logical operations, write a C expression that is equivalent to x == y. In other words, it will return 1 when x and y are equal and 0 otherwise.

**Solution**:
```
! (x ^ y)
```

# 2.1.9 Shift Operations in C (移位)
* C also provides a set of shift operations for shifting bit patterns to the left and to the right.
	* Left shift: x<\<y
		* shift bit-vector x left y positions
	* Right shift: x>>y
		* shift bit-vector x right y positions
* To the left x<<k:
	* an operand x having bit representation $[x_{w−1}, x_{w−2}, . . . , x_0]$, the C expression x << k yields a value with bit representation $[x_{w−k−1} , x_{w−k−2} , . . . , x_0 , 0, . . . , 0]$. That is, x is shifted k bits to the left, dropping off the k most significant bits and filling the right end with k zeros.
	* Shift operations associate from left to right, so x << j << k is equivalent to (x << j) << k.
* To the right x>>k:
	* Logical - fills the left end with ==k zeros==, giving a result Arithmetic. 
		* $[0,...,0,x_{w-1},x_{w-2},...x_k]$
	* Arithmetic - shift fills the left end with ==k repetitions of the most significant bit==, giving a result $[x_{w−1} , . . . , x_{w−1} , x_{w−1} , x_{w−2} , . . . x_k ]$. This convention might seem peculiar, but as we will see, it is useful for operating on signed integer data.
* Example:
	![[./2_1.assets/Screenshot 2023-10-12 at 20.24.34.png|300]]
	* Code:
```c
#include <stdio.h>

void print_CharToBin(char num)
{
	char chars_list[0x2] = "01";
	char result[0x8] = "00000000";
	int length;	// the length of result array
			
	if(num == 0) printf("0b 0\n");

	// Calculate the result and store into result array
	for(int i = 0;num!=0;i++)
	{
		result[i] = chars_list[num%0x2];// repeatly divide input number by 16
						// record the remainder into result[i]
		num /= 0x2;	// get the quotient
	}

	printf("0b ");
	for(int i = 8; i >= 0; i--)
	{
		printf("%c",result[i]);
	}
	printf("\r\n");
}

void print_UIntToBin(unsigned int num)
{
	char chars_list[0x2] = "01";
	char result[0x20] = "00000000000000000000000000000000";
	int length;	// the length of result array
			
	if(num == 0) printf("0b 0\n");

	// Calculate the result and store into result array
	for(int i = 0;num!=0;i++)
	{
		result[i] = chars_list[num%0x2];// repeatly divide input number by 16
						// record the remainder into result[i]
		num /= 0x2;	// get the quotient
	}

	printf("0b ");
	for(int i = 0x20; i >= 0; i--)
	{
		printf("%c",result[i]);
	}
	printf("\r\n");
}

int main()
{
	char x = 0b10010101;
	printf("=== CHAR TEST ===\n");
	printf("The original number: ");
	print_CharToBin(x);
	printf("x << 4: ");
	print_CharToBin(x<<4);
	printf("x >> 4: ");
	print_CharToBin(x>>4);

	printf("\n");
	printf("=== INT TEST ===\n");
	int y = 0b10010101000000001111111100001111;
	printf("The original number: ");
	print_UIntToBin(y);
	printf("y << 4: ");
	print_UIntToBin(y<<4);
	printf("y >> 4: ");
	print_UIntToBin(y>>4);
	return 0;
}
```

![[./2_1.assets/Screenshot 2023-10-16 at 13.17.41.png|500]]

* Some truths:
	* For C, The C standards do not precisely deﬁne which type of right shift should be used with signed numbers—either arithmetic or logical shifts may be used. This unfortunately means that any code assuming one form or the other will potentially encounter portability problems.
	* In practice, however, almost all compiler/machine combinations use ==arithmetic right shifts for signed data==, and many programmers assume this to be the case. But, ==unsigned data right shifts must be logical==.
	* For Java, Java has a precise deﬁnition of how right shifts should be performed.
		* The expression x >> k shifts x arithmetically by k positions, while x >>> k shifts it logically.
	* Undefined behavior: shift amount < 0 or >= word size
		* Example:
```c
char x = 0x12
x<<8 = ?
```

# Practice Problem 2.16
Fill in the table below showing the effects of the different shift operations on single-byte quantities. The best way to think about shift operations is to work with binary representations. Convert the initial values to binary, perform the shifts, and then convert back to hexadecimal. Each of the answers should be 8 binary digits or 2 hexadecimal digits.
![[./2_1.assets/Screenshot 2023-10-12 at 20.29.38.png]]
0x 87

**Solution**:
```
Hex: 					0xD4
Binary: 			0b 1101 0100
a<<2 binary:  0b 0101 0000	
a<<2 hex: 		0x 50
Logical a>>3 binary: 0b 0001 1010
Logical a>>3 hex: 	 0x 1A
Arithmetic a>>3 binary: 0b 1111 1010
Arithmetic a>>3 hex:		0x FA
```

```
Hex: 					0x 64
Binary: 			0b 0110 0100	
a<<2 binary: 	0b 1001 0000
a<<2 hex: 		0x 90
Logical a>>3 binary: 0b 0000 1100
Logical a>>3 hex: 	 0x 0C
Arithmetic a>>3 binary: 0b 0000 1100
Arithmetic a>>3 hex:		0x 0C
```

```
Hex: 					0x 72
Binary: 			0b 0111 0010
a<<2 binary: 	0b 1100 1000
a<<2 hex: 		0x C8
Logical a>>3 binary: 0b 0000 1110
Logical a>>3 hex: 	 0x 0E
Arithmetic a>>3 binary: 0b 0000 1110
Arithmetic a>>3 hex:		0x 0E
```

```
Hex: 					0x 44
Binary: 			0b 0100 0100
a<<2 binary: 	0b 0001 0000
a<<2 hex: 		0x 10
Logical a>>3 binary: 0b 0000 1000
Logical a>>3 hex:    0x 08
Arithmetic a>>3 binary: 0b 0000 1000
Arithmetic a>>3 hex:	  0x 08
```

```
Hex: 					0x 87
Binary: 			0b 1000 0111
a<<2 binary: 	0b 0001 1100
a<<2 hex: 		0x 1C
Logical a>>3 binary: 0b 0001 0000
Logical a>>3 hex: 	 0x 10
Arithmetic a>>3 binary: 0b 1111 0000
Arithmetic a>>3 hex:		0x F0
```

* Addition (and subtraction) have higher precedence than shifts.
	* 1<<2 + 3<<4 -> 1 << (2+3) << 4











