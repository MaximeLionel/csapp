* C provides two mechanisms for creating data types by combining objects of different types:
	* structure
	* union
# 3.9.1 Structures
* The C struct declaration creates a data type that groups objects of possibly different types into a single object.
* The implementation of structures is similar to that of arrays in that all of the components of a structure are stored in a **contiguous region of memory** and a pointer to a structure is the address of its first byte.