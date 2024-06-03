* C provides two mechanisms for creating data types by combining objects of different types:
	* structure
	* union
# 3.9.1 Structures
* The C struct declaration creates a data type that groups objects of possibly different types into a single object.
* The implementation of structures is similar to that of arrays in that all of the components of a structure are stored in a **contiguous region of memory** and a pointer to a structure is the address of its first byte.
* Example:
	```c
	struct rec {
		int i;
		int j;
		int a[2];
		int *p;
	};
	```
	* This structure contains four fields: two 4-byte values of type int, a two-element array of type int, and an 8-byte integer pointer, giving a total of 24 bytes:
		![[image-20240603164441657.png|500]]
	* Array a is embedded within the structure.
	* To access the fields of a structure, the compiler generates code that adds the appropriate offset to the address of the structure.
