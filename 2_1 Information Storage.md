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
  * little endian(小端排序) - the least signiﬁcant byte(最低有效字节) comes ﬁrst from lower addres
* 


















