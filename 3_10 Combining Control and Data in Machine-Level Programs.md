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
* A more pernicious use of buffer overflow is to get a program to perform a function that it would otherwise be unwilling to do. 
	* This is one of the most common methods to attack the security of a system over a computer network.
	* Typically, the program is fed with a string that contains the byte encoding of some executable code, called the exploit code, plus some extra bytes that overwrite the return address with a pointer to the exploit code. 
	* The effect of executing the `ret` instruction is then to jump to the exploit code.

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
	400721: 48 83 ec 10     sub $0x10,%rsp        # line 3

	# Diagram stack at this point
	400725: 48 89 e7        mov %rsp,%rdi
	400728: e8 73 ff ff ff  callq 4006a0 <gets>   # line 5

    # Modify diagram to show stack contents at this point
```

The program terminates with a segmentation fault. You run gdb and determine that the error occurs during the execution of the ret instruction of `get_line`.

A. Fill in the diagram that follows, indicating as much as you can about the stack just after executing the instruction at line 3 in the disassembly. Label the quantities stored on the stack (e.g., “Return address”) on the right, and their hexadecimal values (if known) within the box. Each box represents 8 bytes. Indicate the position of `%rsp`. Recall that the ASCII codes for characters 0–9 are `0x30–0x39`.

| Stack                   | Label          |
| ----------------------- | -------------- |
| 00 00 00 00 00 40 00 76 | Return Address |
|                         |                |
|                         |                |
|                         |                |
|                         |                |
B. Modify your diagram to show the effect of the call to `gets` (line 5).

C. To what address does the program attempt to return?

D. What register(s) have corrupted value(s) when get_line returns?

E. Besides the potential for buffer overflow, what two other things are wrong with the code for get_line?

**Solution**：
A.
1. After executing `call get_line()`, stack would be:

| Stack                   | Label          |
| ----------------------- | -------------- |
| 00 00 00 00 00 40 00 76 | Return Address |
2. The executing flow goes into function `get_line()`. After executing `push %rbx`, the stack would be:

| Stack                   | Label          |
| ----------------------- | -------------- |
| 00 00 00 00 00 40 00 76 | Return Address |
| 01 23 45 67 89 AB CD EF | saved `%rbp`   |

3. After executing `sub $0x10,%rsp`, the stack would still be:

| Stack                   | Label                                              |
| ----------------------- | -------------------------------------------------- |
| 00 00 00 00 00 40 00 76 | Return Address                                     |
| 01 23 45 67 89 AB CD EF | saved `%rbp`                                       |
|                         | extended stack box                                 |
|                         | extended stack box (`rsp`=bottom of the stack box) |

B.
4. After executing `mov %rsp,%rdi`, the stack would be:

| Stack                   | Label                                                |
| ----------------------- | ---------------------------------------------------- |
| 00 00 00 00 00 40 00 76 | Return Address                                       |
| 01 23 45 67 89 AB CD EF | saved `%rbp`                                         |
|                         | extended stack                                       |
|                         | extended stack (`rsp`=`rdi`=bottom of the stack box) |
5. After entering line 5 `callq 4006a0 <gets>`, the stack would be:

| Stack                   | Label                                                                |
| ----------------------- | -------------------------------------------------------------------- |
| 00 00 00 00 00 40 00 76 | Return Address                                                       |
| 01 23 45 67 89 AB CD EF | saved `%rbp`                                                         |
|                         | extended stack                                                       |
|                         | extended stack <br>(`rdi`= bottom of the stack box)                  |
| 00 00 00 00 00 40 07 2D | Return Address after call `gets`<br>(`rsp`= bottom of the stack box) |


6. After finishing line 5 `callq 4006a0 <gets>`, the stack would be:
`char *gets(char *s)`

| Stack                       | Label                                               |
| --------------------------- | --------------------------------------------------- |
| 00 00 00 00 00 40 00 ==34== | Return Address (changed)                            |
| 33 32 31 30 39 38 37 36     | saved `%rbp`                                        |
| 35 34 33 32 31 30 39 38     | extended stack                                      |
| 37 36 35 34 33 32 31 30     | extended stack <br>(`rsp`= bottom of the stack box) |

C. 
`0x040034`

D. 
`%rbp`

E. 
The call to malloc should have had `strlen(buf)+1` as its argument, and the code should also check that the returned value is not equal to `NULL`.

# 3.46 in real GDB
Let's try to do this problem in real GDB.
* Firstly, use GDB to open executable program and disassemble the `get_line` function:
	```
	root@ml:~/csapp/chap3/prac3_46# gdb get_line
	(gdb) disassemble get_line
	Dump of assembler code for function get_line:
	   0x000000000000121b <+0>:     endbr64 
	   0x000000000000121f <+4>:     push   %rbp
	   0x0000000000001220 <+5>:     push   %rbx
	   0x0000000000001221 <+6>:     sub    $0x18,%rsp                  # line 3
	   0x0000000000001225 <+10>:    mov    %fs:0x28,%rax
	   0x000000000000122e <+19>:    mov    %rax,0x8(%rsp)
	   0x0000000000001233 <+24>:    xor    %eax,%eax
	   0x0000000000001235 <+26>:    lea    0x4(%rsp),%rbp
	   0x000000000000123a <+31>:    mov    %rbp,%rdi
	   0x000000000000123d <+34>:    call   0x11c9 <gets>               # line 5
	   0x0000000000001242 <+39>:    mov    %rbp,%rdi
	   0x0000000000001245 <+42>:    call   0x10a0 <strlen@plt>
	   0x000000000000124a <+47>:    mov    %rax,%rdi
	   0x000000000000124d <+50>:    call   0x10c0 <malloc@plt>
	   0x0000000000001252 <+55>:    mov    %rax,%rbx
	   0x0000000000001255 <+58>:    mov    %rbp,%rsi
	   0x0000000000001258 <+61>:    mov    %rax,%rdi
	   0x000000000000125b <+64>:    call   0x1090 <strcpy@plt>
	   0x0000000000001260 <+69>:    mov    0x8(%rsp),%rax
	   0x0000000000001265 <+74>:    sub    %fs:0x28,%rax
	   0x000000000000126e <+83>:    jne    0x127a <get_line+95>
	   0x0000000000001270 <+85>:    mov    %rbx,%rax
	   0x0000000000001273 <+88>:    add    $0x18,%rsp
	   0x0000000000001277 <+92>:    pop    %rbx
	   0x0000000000001278 <+93>:    pop    %rbp
	   0x0000000000001279 <+94>:    ret    
	   0x000000000000127a <+95>:    call   0x10b0 <__stack_chk_fail@plt>
	End of assembler dump.
	```
	* We find it's not actually same as it in PracticeProblem3.46, but we can still find the `line 3` and `line 5` required.
	* Sometimes, we need to open the assembly window: 
	```shell
	(gdb) layout asm
	```
* Secondly, let's analyze and run to `line 3`.
	* When just enter `get_line` function (`0x000000000000121f <+4>:     push   %rbp`), the stack is like:
		![[image-20240611144015558.png|350]]
	* When finish `line3`, the stack is like:
		![[image-20240611144257065.png||350]]
	* Analyze as below:
	```
	0x7fffffffe350: 0x0000000000000000      # extended stack
					0x0000000000000000      # extended stack
	0x7fffffffe360: 0x0000000000000000      # extended stack
					0x0000000000000000      # saved %rbx
	0x7fffffffe370: 0x0000000000000001      # saved %rbp
					0x0000555555555291      # return address to main
	0x7fffffffe380: 0x0000000000000000      
					0x00007ffff7da9d90
	```
* let's run to `line 5` and input `0123456789012345678901234`, then analyze:
	![[image-20240611145133085.png|350]]
```
0x7fffffffe350: 0x3332313000000000      
				0x3130393837363534
0x7fffffffe360: 0x3938373635343332      
				0x0000003433323130
0x7fffffffe370: 0x0000000000000001      
				0x0000555555555291        # return address to main - still safe
0x7fffffffe380: 0x0000000000000000      
				0x00007ffff7da9d90
```
* Then we try longer string again: `012345678901234567890123456789012345678901234`
```
0x7fffffffe340: 0x3332313000000000      
				0x3130393837363534
0x7fffffffe350: 0x3938373635343332      
				0x3736353433323130
0x7fffffffe360: 0x3534333231303938      
				0x3332313039383736         # the return address has been broken
0x7fffffffe370: 0x0000000000000034      
				0x00007ffff7da9d90
```
















