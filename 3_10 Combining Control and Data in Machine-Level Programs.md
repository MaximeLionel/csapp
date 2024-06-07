# 3.10.1 Understanding Pointers
* Pointers are a central feature of the C programming language.
* Key principles of pointers:
	* **Every pointer has an associated ==type==**.
		* Pointer declaration:
			```C
			int *ip;
			char **cpp;
			```
			* Variable `ip` is a pointer to an object of type int.
			* Variable `cpp` is a pointer to an object that itself is a pointer to an object of type char.
		* In general, if the object has type `T`, then the pointer has type `*T`. 
		* The special `void *` type represents a generic pointer. For example, the malloc function returns a generic pointer.
		* Pointer types are not part of machine code; they are an abstraction provided by C to help programmers avoid addressing errors.
	* **Every pointer has a value.**
		* This value is an address of some object of the designated type. 
		* The special `NULL (0)` value indicates that the pointer does not point anywhere.
	* **Pointers are created with the ‘&’ operator**.
		* The machinecode realization of the `‘&’` operator often uses the `leaq` instruction to compute the expression value, since this instruction is designed to compute the address of a memory reference.
	* **Pointers are dereferenced with the ‘*’ operator**.
		* Dereferencing is implemented by a memory reference, either storing to or retrieving from the specified address.
	* **Arrays and pointers are closely related**.
		* The name of an array can be referenced (but not updated) as if it were a pointer variable.
		* Array referencing (e.g., `a[3]`) has the exact same effect as pointer arithmetic and dereferencing (e.g., `*(a+3)`).
		* Both array referencing and pointer arithmetic require scaling the offsets by the object size. 
			* When we write an expression `p+i` for pointer `p` with value `p`, the resulting address is computed as `p + L*i`, where `L` is the size of the data type associated with `p`.
	* **Casting from one type of pointer to another changes its type but not its value**.
		* One effect of casting is to change any scaling of pointer arithmetic. 
			* For example, if `p` is a pointer of type `char*` having value `p`, then the expression `(int*)p+7` computes `p+28`, while `(int*)(p+7)` computes `p+7`.
	* **Pointers can also point to functions**.
		* For example, if we have a function defined by the prototype: `int fun(int x, int *p)`;

















