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



















