* Procedures are a key abstraction in software.
	* Provide a way to package code that implements some functionality with a designated set of arguments and an optional return value.
* Procedures come in many guises in different programming languages — functions, methods, subroutines, handlers, and so on — but they all share a general set of features.
* suppose procedure P calls procedure Q, and Q then executes and returns back to P. These actions involve one or more of the following mechanisms:
	* **Passing control** - The program counter must be set to the starting address of the code for Q upon entry and then set to the instruction in P following the call to Q upon return.
	* **Passing data** - P must be able to provide one or more parameters to Q, and Q must be able to return a value back to P.
	* **Allocating and deallocating memory** - Q may need to allocate space for local variables when it begins and then free that storage before it returns.

# 3.7.1 The Run-Time Stack
* A key feature of the procedure-calling mechanism of C, and of most other languages, is that it can make use of the **last-in, first-out** memory management discipline provided by a **stack** data structure.
* Suppose procedure P calling Q:
	* while Q is executing, P is temporarily suspended.
	* While Q is running, only it will need the ability to **allocate new storage** for its local variables or to set up a call to another procedure.
	* When Q returns, any local storage it has allocated can be freed.
* A program can manage the storage required by its procedures using a stack:
	* As P calls Q, control and data information are added to the end of the stack. 
	* This information gets deallocated when P returns.
* Stack management:
	* The x86-64 stack grows toward lower addresses and the stack pointer `%rsp` points to the top element of the stack.
	* Data can be stored on and retrieved from the stack using the `pushq` and `popq` instructions.
	* Space for data with no specified initial value can be allocated on the stack by simply decrementing the stack pointer by an appropriate amount. 
	* Space can be deallocated by incrementing the stack pointer.
## Stack Frame
![[3_7 Procedures.assets/image-20240522094322604.png|500]]
* When an x86-64 procedure requires storage beyond what it can hold in registers, it allocates space on the stack, which is the procedure's **stack frame**.
* The frame for the currently executing procedure is always at the top of the stack.
* When procedure P calls procedure Q, it will push the **return address** onto the stack, indicating where within P the program should resume execution once Q returns.
	* The return address is part of P’s stack frame, since it holds state relevant to P.
* Q allocates the space required for its stack frame by extending the current stack boundary.
	* With the stack fram, Q cAan save the values of registers, allocate space for local variables, and set up arguments for the procedures it calls.
* The stack frames for most procedures are of fixed size, allocated at the beginning of the procedure.
* Procedure P can pass up to six integral values on the stack, but if Q requires more arguments, these can be stored by P within its stack frame prior to the call.
* In the interest of space and time efficiency, x86-64 procedures allocate only the portions of stack frames they require.
* Some functions do not even require a stack frame. This occurs when all of the local variables can be held in registers and the function does not call any other functions.

# 3.7.2 Control Transfer
* Passing control from function P to function Q involves simply setting the **program counter (PC)** to the starting address of the code for Q.
* When later comes time for Q to return, the processor must have some record of the code location where it should resume the execution of P. This information is recorded in x86-64 machines by invoking procedure Q with the instruction `call Q`.
* `Call Q`:
	* Pushes an address A onto the stack; 
		* Address A is referred to as the return address and is computed as the address of the instruction immediately following the call instruction.
	* Sets the PC to the beginning of Q.
* `ret`:
	* Pops an address A off the stack;
	* Sets the PC to A.
![[3_7 Procedures.assets/image-20240522101614495.png|300]]
* A call can be either direct or indirect:
	* Direct call - the target of a direct call is given as a label;
	* Indirect call - the target of an indirect call is given by `*` followed by an operand specifier.
## Example 0
```c
#include<stdio.h>

int add(int a, int b)
{
        return a+b;
}

int main()
{
        int a = 6;
        int b = 9;
        int c = add(a,b);

        printf("%d + %d = %d \n",a,b,c);
        return 0;
}
```
* Firstly, we check and select the stack frame, also list the frame info and stack 64bits' information:
```shell
(gdb) backtrace
#0  main () at main.c:9
(gdb) frame 0
#0  main () at main.c:9
9       {
```

```shell
(gdb) info locals
a = 6
b = 9
c = <optimized out>
(gdb) info args
No arguments.
```

```shell
(gdb) x/10dx $rsp
0x7fffffffe398: 0xf7da9d90      0x00007fff      0x00000000    0x00000000
0x7fffffffe3a8: 0x55555151      0x00005555      0x00000000    0x00000001
0x7fffffffe3b8: 0xffffe4a8      0x00007fff
(gdb) 
```

* Secondly, step into the add function and check the same information again:
```shell
(gdb) backtrace
#0  add (a=a@entry=6, b=b@entry=9) at main.c:5
#1  0x0000555555555168 in main () at main.c:12
(gdb) frame 0
#0  add (a=a@entry=6, b=b@entry=9) at main.c:5
5               return a+b;
```

```shell
(gdb) info locals
No locals.
(gdb) info args
a = 6
b = 9
```

```shell
(gdb) x/10dx $rsp
0x7fffffffe388: 0x55555168      0x00005555      0x00000000    0x00000000
0x7fffffffe398: 0xf7da9d90      0x00007fff      0x00000000    0x00000000
0x7fffffffe3a8: 0x55555151      0x00005555
```

* What we find is that on top of the stack, there's one 64bits number: `0x55555168      0x00005555`. Let's find out what it is. We disassemble the main function.
```shell
(gdb) disassemble main
Dump of assembler code for function main:
   0x0000555555555151 <+0>:     endbr64 
   0x0000555555555155 <+4>:     sub    $0x8,%rsp
   0x0000555555555159 <+8>:     mov    $0x9,%esi
   0x000055555555515e <+13>:    mov    $0x6,%edi
   0x0000555555555163 <+18>:    call   0x555555555149 <add>
   0x0000555555555168 <+23>:    mov    %eax,%r8d
   0x000055555555516b <+26>:    mov    $0x9,%ecx
   0x0000555555555170 <+31>:    mov    $0x6,%edx
   0x0000555555555175 <+36>:    lea    0xe88(%rip),%rsi        # 0x555555556004
   0x000055555555517c <+43>:    mov    $0x1,%edi
   0x0000555555555181 <+48>:    mov    $0x0,%eax
   0x0000555555555186 <+53>:    call   0x555555555050 <__printf_chk@plt>
   0x000055555555518b <+58>:    mov    $0x0,%eax
   0x0000555555555190 <+63>:    add    $0x8,%rsp
   0x0000555555555194 <+67>:    ret    
End of assembler dump.
```
* It's actually the address of the code line below:
`0x0000555555555168 <+23>:    mov    %eax,%r8d`
* Thus, it is the `returnn address` after calling add function.
* Then we step to the end of add function and go back to main function again. We check the stack information again:
```shell
(gdb) x/10dx $rsp
0x7fffffffe390: 0x00000000      0x00000000      0xf7da9d90    0x00007fff
0x7fffffffe3a0: 0x00000000      0x00000000      0x55555151    0x00005555
0x7fffffffe3b0: 0x00000000      0x00000001
```
* We find the `return address` has been popped already.

## Example 1 on book
![[3_7 Procedures.assets/image-20240522130848202.png|600]]
![[3_7 Procedures.assets/image-20240522130957356.png|600]]

* Executing call - the call instruction with address 0x400563 in main calls function `multstore`.
* After call - push the return address 0x400568 onto the stack and to jump to the first instruction in function `multstore`, at address 0x0400540.
* After ret - The execution of function `multstore` continues until it hits the ret instruction at address 0x40054d.
	* This instruction pops the value 0x400568 from the stack and jumps to this address, resuming the execution of main just after the call instruction.
		
## Example 2 on book
![[3_7 Procedures.assets/image-20240522131654513.png|700]]
![[3_7 Procedures.assets/image-20240522131746300.png|700]]



