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
		* We can declare and assign a pointer `fp` to this function:
			```C
			int (*fp)(int, int *); # the syntax for declaring function pointers
			fp = fun;
			```
		* We can then invoke the function using this pointer:
			```C
			int y = 1;
			int result = fp(3, &y);
			```
		* The value of a function pointer is the address of the first instruction in the machine-code representation of the function.

# 3.10.2 Life in the Real World: Using the gdb Debugger
* The GNU debugger gdb provides a number of useful features to support the run-time evaluation and analysis of machine-level programs.
* Some GDB commands:
	![[image-20240607130614162.png|550]]
* Start gdb: `linux> gdb prog`
* The general scheme is to set breakpoints near points of interest in the program. 
	* These can be set to just after the entry of a function or at a program address.

# 3.10.3 Out-of-Bounds Memory References and Buffer Overflow
* We have seen that C does not perform any bounds checking for array references, and that local variables are stored on the stack along with state information such as saved register values and return addresses.
	* The state stored on the stack can be corrupted by a write to an out-of-bounds array element.
	* When the program then tries to reload the register or execute a ret instruction with this corrupted state, things will be unpredictable.
* A particularly common source of state corruption is known as **buffer overflow**.
## Example:
```C
	/* Implementation of library function gets() */
	char *gets(char *s)
	{
		int c;
		char *dest = s;
		while ((c = getchar()) != ’\n’ && c != EOF)
			*dest++ = c;
			
		if (c == EOF && dest == s) /* No characters read */
			return NULL;
	
		*dest++ = ’\0’; /* Terminate string */
		return s;
	}
	
	/* Read input line and write it back */
	void echo()
	{
		char buf[8]; /* Way too small! */
		gets(buf);
		puts(buf);
	}
```
* Some interpretations:
	* Function `gets` reads a line from the standard input, stopping when either a terminating newline character or some error condition is encountered.
	* Function `gets` then copies this string to the location designated by argument `s` and terminates the string with a `null` character.
	* Function `echo` using `gets` simply reads a line from standard input and echos it back to standard output.
	* In our echo example, we have purposely made the buffer very small—just eight characters long. Any string longer than seven characters will cause an out-of-bounds write.


















