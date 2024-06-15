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



# 3.10.4 Thwarting Buffer Overflow Attacks
* Modern compilers and operating systems have implemented mechanisms to make it more difficult to mount these attacks and to limit the ways by which an intruder can seize control of a system via a **buffer overflow attack**.

## Stack Randomization
* The idea of stack randomization is to make the position of the stack vary from one run of a program to another.
* Stack randomization has become standard practice in Linux systems. It is one of a larger class of techniques known as **address-space layout randomization, or ASLR**.
	* With ASLR, different parts of the program, including program code, library code, stack, global variables, and heap data, are loaded into different regions of memory each time a program is run.
	* A program running on one machine will have very different address mappings than the same program running on other machines.
* The following code shows a simple way to determine a “typical” stack address:
	```C
	int main() {
		long local;
		printf("local at %p\n", &local);
		return 0;
	}
	```
	* This code simply prints the address of a local variable in the main function.
	* Running the code 10,000 times on a Linux machine in 32-bit mode, the addresses ranged from 0xff7fc59c to 0xffffd09c, a range of around $2^{23}$. 
	* Running in 64-bit mode on the newer machine, the addresses ranged from 0x7fff0001b698 to 0x7ffffffaa4a8, a range of nearly $2^{32}$.
* `nop` sled - include a long sequence of `nop` instructions before the actual exploit code.
	* `nop` is short for “no operation” - Executing this instruction has no effect, other than incrementing the program counter to the next instruction.
	* As long as the attacker can guess an address somewhere within this sequence, the program will run through the sequence and then hit the exploit code.
	* Example: If we set up a 256-byte `nop` sled, then the randomization over $n = 2^{23}$ can be cracked by enumerating $2^{15} = 32,768$ starting addresses, which is entirely feasible for a determined attacker.

## Stack Corruption Detection
* We saw in the example of the echo function below (Figure 3.40) that the corruption typically occurs when the program overruns the bounds of a local buffer.
	![[image-20240612104851161.png|500]]
* In C, there is no reliable way to prevent writing beyond the bounds of an array.
* Recent versions of gcc incorporate a mechanism known as a **stack protector** into the generated code to detect buffer overruns.
	* The idea is to store a special **guard value/canary value** in the stack frame between any local buffer and the rest of the stack state as below:
		![[image-20240612110009008.png|300]]
	* Guard value is generated randomly each time the program runs, and so there is no easy way for an attacker to determine what it is.
	* Before restoring the register state and returning from the function, the program checks if the canary has been altered by some operation of this function or one that it has called. If so, the program aborts with an error.
* Recent versions of gcc try to determine whether a function is vulnerable to a stack overflow and insert this type of overflow detection automatically.
	* The command-line option `-fno-stack-protector` to prevent gcc from inserting this code.
	* Compiling the function echo without this option, and hence with the stack protector enabled, gives the following assembly code:
		![[image-20240612112045247.png|400]]
		* The instruction argument `%fs:40` is an indication that the canary value is read from memory using `segmented addressing`.
			* **Segmented addressing** - an addressing mechanism that dates back to the 80286 and is seldom found in programs running on modern systems.
		* By storing the canary in a special segment, it can be marked as “read only,” so that an attacker cannot overwrite the stored canary value.
		* Before restoring the register state and returning, the function compares the value stored at the stack location with the canary value.
			* If the two are identical, the xorq instruction will yield zero, and the function will complete in the normal fashion. 
			* A nonzero value indicates that the canary on the stack has been modified, and so the code will call an error routine.

## Limiting Executable Code Regions
* A final step is to eliminate the ability of an attacker to insert executable code into a system. One method is to limit which memory regions hold executable code.
	* In typical programs, only the portion of memory holding the code generated by the compiler need be executable. The other portions can be restricted to allow just reading and writing.
* Many systems allow control over three forms of access: read (reading data from memory), write (storing data into memory), and execute (treating the memory contents as machine-level code).
* Some types of programs require the ability to dynamically generate and execute code, for example: “just-in-time” compilation techniques.
* The techniques we have outlined—randomization, stack protection, and limiting which portions of memory can hold executable code—are three of the most common mechanisms used to minimize the vulnerability of programs to buffer overflow attacks.
## Extented knowledge for `executable` access:
* The CPU determines if a page is executable through a combination of hardware support in the Memory Management Unit (MMU) and the operating system’s page table entries (PTEs). Here's a detailed explanation of how this process works:
### Memory Management Unit (MMU) and Page Tables

1. **Memory Pages and Page Tables**:
    
    - Memory is divided into fixed-size blocks called pages.
    - Page tables are data structures maintained by the operating system that map virtual addresses to physical addresses and define page attributes.
2. **Page Table Entries (PTEs)**:
    
    - Each page table entry (PTE) corresponds to a single page and includes various control bits (flags) that describe the properties of the page.
    - Common flags include Present (P), Read/Write (R/W), User/Supervisor (U/S), and Execute (X) or Execute Disable (XD).

### Setting Execute Permissions

3. **Control Bits for Execution**:
    
    - **X (Execute) Bit**: Indicates if the page is executable.
    - For Intel architectures, this is often referred to as the NX (No-Execute) bit or XD (Execute Disable) bit.
    - For ARM architectures, it's known as the XN (Execute Never) bit.
4. **Operating System Configuration**:
    
    - When the operating system sets up memory pages, it configures the PTEs accordingly.
    - To mark a page as executable, the OS sets the appropriate bit (e.g., clears the NX/XD bit).
    - To mark a page as non-executable, the OS sets the NX/XD bit.

### CPU Execution and Permission Check

5. **CPU Fetching an Instruction**:
    
    - When the CPU fetches an instruction to execute, it translates the virtual address to a physical address using the page table.
    - The MMU reads the corresponding PTE to determine the properties of the page.
6. **Permission Check**:
    
    - The MMU checks the Execute (X) bit in the PTE.
        - **If the X bit is set** (indicating executable), the CPU proceeds to execute the instruction.
        - **If the X bit is not set** (indicating non-executable), the MMU generates a fault (e.g., an Execute Disable fault or similar exception).
    - The CPU then handles this fault, typically causing the operating system to take corrective action (e.g., terminating the offending process, logging an error).

### Hardware Support

7. **Intel’s NX Bit**:
    
    - In x86/x64 architectures, the NX bit (bit in the page table entry) marks pages as non-executable.
    - If the NX bit is set, the CPU will not execute code from that page.
8. **ARM’s XN Bit**:
    
    - In ARM architectures, the XN bit (Execute Never) serves the same purpose.
    - Setting the XN bit prevents execution of code on that page.

### Security Implications

9. **Enhanced Security**:
    - This mechanism enhances security by preventing code execution in data pages, mitigating certain types of attacks like buffer overflows.
    - It allows for finer-grained control over memory access permissions, contributing to overall system stability and security.
### Summary

The CPU, with the help of the MMU and the operating system’s page table entries, determines if a page is executable by checking the execute permission bit (such as the NX bit in Intel or the XN bit in ARM) in the PTE. If the bit is set, the page is considered non-executable, and any attempt to execute code from that page results in a fault, which the operating system handles. This system provides robust security against execution of unintended or malicious code.



# Practice Problem 3.47
Running our stack-checking code 10,000 times on a system running Linux version 2.6.16, we obtained addresses ranging from a minimum of 0xffffb754 to a maximum of 0xffffd754.

A. What is the approximate range of addresses?

B. If we attempted a buffer overrun with a 128-byte `nop` sled, about how many attempts would it take to test all starting addresses?

**Solution**:
A. $2^{13}$
B. 
$128=2^7$
Thus we only need $2^6$ attempts.

# Practice Problem 3.48
The functions `intlen`, `len`, and `iptoa` provide a very convoluted way to compute the number of decimal digits required to represent an integer. We will use this as a way to study some aspects of the gcc stack protector facility.
```C
int len(char *s) {
	return strlen(s);
}

void iptoa(char *s, long *p) {
	long val = *p;
	sprintf(s, "%ld", val);
}

int intlen(long x) {
	long v;
	char buf[12];
	v = x;
	iptoa(buf, &v);
	return len(buf);
}
```

The following show portions of the code for intlen, compiled both with and without stack protector:
(a) Without protector
```
# int intlen(long x)
# x in %rdi

intlen:
	subq $40, %rsp
	movq %rdi, 24(%rsp)
	leaq 24(%rsp), %rsi
	movq %rsp, %rdi
	call iptoa
```

(b) With protector
```
# int intlen(long x)
# x in %rdi

intlen:
	subq $56, %rsp
	movq %fs:40, %rax
	movq %rax, 40(%rsp)
	xorl %eax, %eax
	movq %rdi, 8(%rsp)
	leaq 8(%rsp), %rsi
	leaq 16(%rsp), %rdi
	call iptoa
```

A. For both versions: What are the positions in the stack frame for buf, v, and (when present) the canary value?

B. How does the rearranged ordering of the local variables in the protected code provide greater security against a buffer overrun attack?

**Solution**:
A. 
Analyze one by one.
For unprotected version:
```
# int intlen(long x)
# x in %rdi

intlen:
	subq $40, %rsp          # rsp-=40: extend stack
	movq %rdi, 24(%rsp)     # *(rsp+24)=x: 
	leaq 24(%rsp), %rsi     # rsi=rsp+24: &v
	movq %rsp, %rdi         # rdi=rsp: buf
	call iptoa
```
So, 
buf = rsp
v = \*(rsp+24)

For protected version:
```
# int intlen(long x)
# x in %rdi

intlen:
	subq $56, %rsp          # rsp -= 56
	movq %fs:40, %rax       # rax = canary value
	movq %rax, 40(%rsp)     # *(rsp+40) = canary value
	xorl %eax, %eax
	movq %rdi, 8(%rsp)      # *(rsp+8) = x
	leaq 8(%rsp), %rsi      # rsi=rsp+8: &v
	leaq 16(%rsp), %rdi     # rdi=rsp+16: buf
	call iptoa
```
So,
buf = rsp+16
v = \*(rsp+8)
canary value = \*(rsp+40)

B.

| Unprotected Stack | Comment |
| ----------------- | ------- |
| rsp+24            | &v      |
| ...               | ...     |
| rsp+12            | buf+12  |
| ...               | ...     |
| rsp               | buf     |
For unprotected stack, if there's a buffer overrun attack, the `v` value will probably be changed.

| Protected Stack | Comment          |
| --------------- | ---------------- |
| rsp+40          | **canary value** |
| ...             |                  |
| rsp+28          | buf+12           |
| ...             | ...              |
| rsp+16          | buf              |
| rsp+8           | &v               |
| rsp             |                  |

For protected stack, if there's a buffer overrun attack, the `v` value will never be changed. And because of canary value, if the return address is changed, it will be detected and pop up an error.



# 3.10.5 Supporting Variable-Size Stack Frames
* For functions with fixed amount of local storage, the compiler can determine in advance the amount of space that must be allocated for their stack frames.
* Some functions, however, require a variable amount of local storage. For example, when the function calls `alloca`, a standard library function that can allocate an arbitrary number of bytes of storage on the stack.
## C code
```c
	long vframe(long n, long idx, long *q) {
		long i;
		long *p[n];             
		p[0] = &i;
		for (i = 1; i < n; i++)
			p[i] = q;
		return *p[idx];
	}
```
* `long *p[n]`
	* Declares local array `p` of `n` pointers, where `n` is given by the first argument.
	* Requires allocating `8n` bytes on the stack, where the value of `n` may vary from one call of the function to another.
* `p[0] = &i`
	* Generates a reference to the address of local variable `i`, and so this variable must also be stored on the stack.
* The program must be able to access both local variable `i` and the elements of array `p`.
* On returning, the function must deallocate the stack frame and set the stack pointer to the position of the stored return address.

## Generated assembly code on book
```
	# long vframe(long n, long idx, long *q)
	# n in %rdi, idx in %rsi, q in %rdx
	# Only portions of code shown
vframe: 
	pushq %rbp           # Save old %rbp 
	movq %rsp, %rbp      # Set frame pointer 
	subq $16, %rsp       # Allocate space for i (%rsp = s1) 
	leaq 22(,%rdi,8), %rax 
	andq $-16, %rax 
	subq %rax, %rsp      # Allocate space for array p (%rsp = s2) 
	leaq 7(%rsp), %rax 
	shrq $3, %rax 
	leaq 0(,%rax,8), %r8 # Set %r8 to &p[0] 
	movq %r8, %rcx       # Set %rcx to &p[0] (%rcx = p) 
	... 
	# Code for initialization loop 
	# i in %rax and on stack, n in %rdi, p in %rcx, q in %rdx 
.L3:                     # loop: 
	movq %rdx, (%rcx,%rax,8) # Set p[i] to q 
	addq $1, %rax        # Increment i 
	movq %rax, -8(%rbp)  # Store on stack 
.L2: 
	movq -8(%rbp), %rax  # Retrieve i from stack 
	cmpq %rdi, %rax      # Compare i:n 
	jl .L3               # If <, goto loop 
	... 
	# Code for function exit 
	leave                # Restore %rbp and %rsp 
	ret                  # Return
```

* Sets up the stack frame:
	```
	pushq %rbp           # Save old %rbp 
	movq %rsp, %rbp      # Set frame pointer 
	```
	* x86-64 code uses register `%rbp` to serve as a **frame pointer** (sometimes referred to as a base pointer).
	* `%rbp` is a **callee-saved** register. 
	* It starts by pushing the current value of `%rbp` onto the stack and setting `%rbp` to point to this stack position.
	* It then keeps `%rbp` pointing to this position throughout the execution of the function, and it references fixed-length local variables, such as `i`, at offsets relative to `%rbp`.
* Allocates space for `i`:
	```
	subq $16, %rsp
	```
	* Allocates 16 bytes on the stack, the first 8 of which are used to store local variable `i`, and the second 8 of which are unused.
	* How we decide it's space for `i`? 
	```
	addq $1, %rax        # Increment i 
	movq %rax, -8(%rbp)  # Store on stack 
	```
* Allocates space for array `p`:
	```
	leaq 22(,%rdi,8), %rax     # rax=8n+22
	andq $-16, %rax      # rax=rax&0xFFFF FFFF FFFF FFF0: clear the low 4 bits
	subq %rax, %rsp      # rsp=rsp-rax: allocate space for array p 
	leaq 7(%rsp), %rax   # rax=rsp+7: 
	shrq $3, %rax 
	leaq 0(,%rax,8), %r8 # Set %r8 to &p[0] 
	movq %r8, %rcx       # Set %rcx to &p[0] (%rcx = p)
	```
	* why 22? Compiler decided this, which is to make sure:
		* 16 bytes' memory alignment;
		* Allocates proper stack space for use.
		* For example: 
			* if n=4, then rax=8n+22=54;
			* after `andq $-16, %rax`, rax=48;
			* `subq %rax, %rsp` will allocate 48 bytes on stack for array `p`
			* 48 can both meet enough space for array `p` and also 16 bytes' memory alignment requirement.
	* `andq $-16, %rax`: clear the low 4 bits to meet 16 bytes' alignment.
		* -16 = 0x FFFF FFFF FFFF FFF0
	* `subq %rax, %rsp`: allocate space for array `p` while the size is rax.
	```
	leaq 7(%rsp), %rax   # rax=rsp+7: 
	shrq $3, %rax        # rax=rax>>3
	leaq 0(,%rax,8), %r8 # Set %r8 to &p[0] 
	```
	![[image-20240614215310752.png|400]]
	* after the 3 lines, r8 will equal to p.
	* By now, the whole array p will be showed above.

* Restore frame pointer - `leave` instruction.
	* `leave` equal to the instructions below:
	```
	movq %rbp, %rsp         # Set stack pointer to beginning of frame
	popq %rbp               # Restore saved %rbp and set stack ptr to end of caller’s frame
	```
	* `leave` instruction has the effect of deallocating the entire stack frame.
* With x86-64 code, the frame pointer is used only in cases where the stack frame may be of **variable size**.

## Generated assembly code in real life
```
# long vframe(long n, long idx, long *q)
# n in %rdi, idx in %rsi, q in %rdx
		.text
        .globl  vframe
        .type   vframe, @function
vframe:
        pushq   %rbp
        movq    %rsp, %rbp         # rbp=rsp: frame pointer
        subq    $16, %rsp          # rsp=rsp-0x10
        movq    %rdi, %rcx         # rcx=rdi: rcx=n
        movq    %rsi, %rdi         # rdi=rsi: rdi=idx
        leaq    15(,%rcx,8), %rax  # rax=8*rcx+0xf: rax=8n+15
        movq    %rax, %r8          # r8=rax: r8=8n+0xf
        andq    $-16, %r8          # clear low 4 bits
        andq    $-4096, %rax       # clear low 12 bits
        movq    %rsp, %rsi         # rsi=rsp
        subq    %rax, %rsi         # rsi=rsi-rax: rsi=rsp-(8n+15)
.L2:
        cmpq    %rsi, %rsp
        je      .L3
        subq    $4096, %rsp        # rsp=rsp-0x1000
        orq     $0, 4088(%rsp)     # 0xFF8
        jmp     .L2
.L3:
        movq    %r8, %rax
        andl    $4095, %eax
        subq    %rax, %rsp
        testq   %rax, %rax
        je      .L4
        orq     $0, -8(%rsp,%rax)
.L4:
        leaq    7(%rsp), %rsi
        movq    %rsi, %rax
        shrq    $3, %rax
        andq    $-8, %rsi
        leaq    -8(%rbp), %r8
        movq    %r8, 0(,%rax,8)
        movq    $1, -8(%rbp)
        jmp     .L5
.L6:
        movq    %rdx, (%rsi,%rax,8)
        addq    $1, -8(%rbp)
.L5:
        movq    -8(%rbp), %rax
        cmpq    %rcx, %rax
        jl      .L6
        movq    (%rsi,%rdi,8), %rax
        movq    (%rax), %rax
        leave
        ret
```
# Practice Problem 3.49
In this problem, we will explore the logic behind the code in lines 8–14 of Figure below, where space is allocated for variable-size array p. As the annotations of the code indicate, let us let s1 denote the address of the stack pointer after executing the `subq` instruction of line 7. This instruction allocates the space for local variable i. Let s2 denote the value of the stack pointer after executing the `subq` instruction of line 10. This instruction allocates the storage for local array p. Finally, let p denote the value assigned to registers `%r8` and `%rcx` in the instructions of lines 13–14. Both of these registers are used to reference array p. The right-hand side of Figure below diagrams the positions of the locations indicated by s1, s2, and p. It also shows that there may be an offset of e2 bytes between the values of s1 and p. This space will not be used. There may also be an offset of e1 bytes between the end of array p and the position indicated by s1.

```
	# long vframe(long n, long idx, long *q)
	# n in %rdi, idx in %rsi, q in %rdx
	# Only portions of code shown
vframe: 
	pushq %rbp           # Save old %rbp 
	movq %rsp, %rbp      # Set frame pointer 
	subq $16, %rsp       # Allocate space for i (%rsp = s1) 
	leaq 22(,%rdi,8), %rax 
	andq $-16, %rax 
	subq %rax, %rsp      # Allocate space for array p (%rsp = s2) 
	leaq 7(%rsp), %rax 
	shrq $3, %rax 
	leaq 0(,%rax,8), %r8 # Set %r8 to &p[0] 
	movq %r8, %rcx       # Set %rcx to &p[0] (%rcx = p) 
	... 
	# Code for initialization loop 
	# i in %rax and on stack, n in %rdi, p in %rcx, q in %rdx 
.L3:                     # loop: 
	movq %rdx, (%rcx,%rax,8) # Set p[i] to q 
	addq $1, %rax        # Increment i 
	movq %rax, -8(%rbp)  # Store on stack 
.L2: 
	movq -8(%rbp), %rax  # Retrieve i from stack 
	cmpq %rdi, %rax      # Compare i:n 
	jl .L3               # If <, goto loop 
	... 
	# Code for function exit 
	leave                # Restore %rbp and %rsp 
	ret                  # Return
```
![[image-20240614215310752.png|300]]

A. Explain, in mathematical terms, the logic in the computation of s2 on lines 8–10. Hint: Think about the bit-level representation of −16 and its effect in the `andq` instruction of line 9.

B. Explain, in mathematical terms, the logic in the computation of p on lines 11–13. Hint: You may want to refer to the discussion on division by powers of 2 in Section 2.3.7.

C. For the following values of n and s1, trace the execution of the code to determine what the resulting values would be for s2, p, e1, and e2.

**Solution**:
A. 
Firstly, -16 = 0x FFFF FFFF FFFF FFF0
Then, `andq $-16, %rax` is to clear the last 4 bits, to meet 16 bytes' alignment.
Next, from `leaq 22(,%rdi,8), %rax`, we get `rax=8n+22`.













