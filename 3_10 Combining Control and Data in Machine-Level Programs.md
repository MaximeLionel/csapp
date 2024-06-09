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
* Assembly Code:
	```z80
	# void echo()
	echo:
		subq $24, %rsp     # Allocate 24 bytes on stack
		movq %rsp, %rdi    # Compute buf as %rsp
		call gets
		movq %rsp, %rdi    # Compute buf as %rsp
		call puts
		addq $24, %rsp     # Deallocate stack space
		ret
	```
	![[image-20240607174112881.png|400]]
* Details on Assembly Code:
	* The program allocates 24 bytes on the stack by subtracting 24 from the stack pointer - `subq $24, %rsp`.
	* Character `buf` is positioned at the top of the stack, as can be seen by the fact that `%rsp` is copied to `%rdi` to be used as the argument to the calls to both `gets` and `puts`.
	* The 16 bytes between `buf` and the stored return pointer are not used.
	* As long as the user types at most seven characters, the string returned by `gets` (including the terminating null) will fit within the space allocated for `buf`.
	* A longer string, however, will cause `gets` to overwrite some of the information stored on the stack.
		![[image-20240607175320293.png|300]]
	* No serious consequence occurs for strings of up to 23 characters, but beyond that, the value of the return pointer, and possibly additional saved state, will be corrupted.
	* If the stored value of the return address is corrupted, then the `ret` instruction will cause the program to jump to a totally unexpected location.
* A better version involves using the function `fgets`, which includes as an argument a count on the maximum number of bytes to read.

# Practice Problem 3.46
Figure below shows a (low-quality) implementation of a function that reads a line from standard input, copies the string to newly allocated storage, and returns a pointer to the result.

Consider the following scenario. Procedure `get_line` is called with the return address equal to `0x400776` and register `%rbx` equal to `0x0123456789ABCDEF`. You type in the string `0123456789012345678901234`
(a) C code
```C
/* This is very low-quality code.
It is intended to illustrate bad programming practices.
See Practice Problem 3.46. */

char *get_line()
{
	char buf[4];
	char *result;
	gets(buf);
	result = malloc(strlen(buf));
	strcpy(result, buf);
	return result;
}
```

(b) Disassembly up through call to gets
```
# char *get_line()
0000000000400720 <get_line>:
	400720: 53              push %rbx
	400721: 48 83 ec 10     sub $0x10,%rsp

	# Diagram stack at this point
	400725: 48 89 e7        mov %rsp,%rdi
	400728: e8 73 ff ff ff  callq 4006a0 <gets>

Modify diagram to show stack contents at this point
```



















