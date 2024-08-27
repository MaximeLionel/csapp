* Procedures are a key abstraction in software.
	* Provide a way to package code that implements some functionality with a designated set of arguments and an optional return value.
* Procedures come in many guises in different programming languages — functions, methods, subroutines, handlers, and so on — but they all share a general set of features.
* suppose procedure P calls procedure Q, and Q then executes and returns back to P. These actions involve one or more of the following mechanisms:
	* **Passing control** - The program counter must be set to the starting address of the code for Q upon entry and then set to the instruction in P following the call to Q upon return.
	* **Passing data** - P must be able to provide one or more parameters to Q, and Q must be able to return a value back to P.
	* **Allocating and deallocating memory** - Q may need to allocate space for local variables when it begins and then free that storage before it returns.

# 3.7.1 The Run-Time Stack
* A key feature of the procedure-calling mechanism of C, and of most other languages, is that it can make use of the **last-in, first-out** memory management discipline provided by a **stack** data structure.
* Suppose procedure P is calling Q:
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
![[3_7 Procedures.assets/image-20240522131654513.png|600]]
![[3_7 Procedures.assets/image-20240522131746300.png|600]]
* Each instruction is identified by labels L1–L2 (in leaf), T1–T4 (in top), and M1–M2 in main.
* main calls top(100), causing top to call leaf(95). Function leaf returns 97 to top, then returns 194 to main.
* Details:
	* M1 - main calls top. The return address (0x400560) will be push on top of stack, thus the `rsp` will be decreased by 8 to store the return address.
	* T1 - after executing the operation above.
	* T2 - top calls leaf. The return address (0x40054E) will be push on top of stack, thus the `rsp` will be decreased by 8 to store the return address.
	* L1 - after executing the operation above.
	* L2 - return to top. The return address (0x40054E) will be popped to `rip`, thus the `rsp` will be increased by 8.
	* T3 - after executing the operation above.
	* T4 - return to top. The return address (0x400560) will be popped to `rip`, thus the `rsp` will be increased by 8.
	* M2 - after executing the operation above.


# Practice Problem 3.32
The disassembled code for two functions first and last is shown below, along with the code for a call of first by function main:
![[3_7 Procedures.assets/image-20240522160933277.png|600]]
Each of these instructions is given a label. Starting with the calling of first(10) by main, fill in the following table to trace instruction execution through to the point where the program returns back to main.

| Label |    PC    | Instruction | %rdi | %rsi | %rax |      %rsp      | *%rsp |  Description   |
| :---: | :------: | :---------: | :--: | :--: | :--: | :------------: | :---: | :------------: |
|  M1   | 0x400560 |    callq    |  10  |  -   |  -   | 0x7fffffffe820 |   -   | Call first(10) |
|  F1   |          |             |      |      |      |                |       |                |
|  F2   |          |             |      |      |      |                |       |                |
|  F3   |          |             |      |      |      |                |       |                |
|  L1   |          |             |      |      |      |                |       |                |
|  L2   |          |             |      |      |      |                |       |                |
|  L3   |          |             |      |      |      |                |       |                |
|  F4   |          |             |      |      |      |                |       |                |
|  M2   |          |             |      |      |      |                |       |                |

**Solution**:
![[3_7 Procedures.assets/image-20240522160933277.png|600]]

| Label |    PC    | Instruction | %rdi | %rsi | %rax |      %rsp      |  *%rsp   |       Description       |
| :---: | :------: | :---------: | :--: | :--: | :--: | :------------: | :------: | :---------------------: |
|  M1   | 0x400560 |    callq    |  10  |  -   |  -   | 0x7fffffffe820 |    -     |   Will call first(10)   |
|  F1   | 0x400548 |     lea     |  10  |  -   |  -   | 0x7fffffffe818 | 0x400565 |  Will do: rsi=rdi+1=10  |
|  F2   | 0x40054c |     sub     |  10  |  11  |  -   | 0x7fffffffe818 | 0x400565 |      Will do:rdi--      |
|  F3   | 0x400550 |    callq    |  9   |  11  |  -   | 0x7fffffffe818 | 0x400565 |     Will call last      |
|  L1   | 0x400540 |     mov     |  9   |  11  |  -   | 0x7fffffffe810 | 0x400555 |    Will do:rax=rdi=9    |
|  L2   | 0x400543 |    imul     |  9   |  11  |  9   | 0x7fffffffe810 | 0x400555 | Will do: rax=rsi*rax=99 |
|  L3   | 0x400547 |    retq     |  9   |  11  |  99  | 0x7fffffffe810 | 0x400555 |  Will return to first   |
|  F4   | 0x400555 |  repz retq  |  9   |  11  |  99  | 0x7fffffffe818 | 0x400565 |   Will return to main   |
|  M2   | 0x400565 |     mov     |  9   |  11  |  99  | 0x7fffffffe820 |    -     |         in main         |


# 3.7.3 Data Transfer
* In addition to passing control to a procedure when called, procedure calls may involve **passing data as arguments**, and returning from a procedure may also involve **returning a value**.
![[3_7 Procedures.assets/image-20240523090718505.png|400]]
* With x86-64, up to six integral arguments can be passed via registers.
* Arguments smaller than 64 bits can be accessed using the appropriate subsection of the 64-bit register.
* When a function has more than six integral arguments:
	* Suppose P calls Q with n integral arguments (n > 6):
	* The code for P must allocate a stack frame with enough storage for arguments 7 through n.
	* The code for P copies arguments 1–6 into the appropriate registers, and it puts arguments 7 through n onto the stack, with argument 7 at the top of the stack.
		* When passing parameters on the stack, all data sizes are rounded up to be multiples of eight.
	* Procedure Q can access its arguments via registers and possibly from the stack.

## Example
```c
#include<stdio.h>

void proc(long a1, long *a1p, int a2, int *a2p, short a3, short *a3p, char a4, char *a4p)
{
	*a1p += a1;
	*a2p += a2;
	*a3p += a3;
	*a4p += a4;
}

int main()
{
	long a1 = 1;
	long* a1p = &a1;
	int a2 = 2;
	int* a2p = &a2;
	short a3 = 3;
	short* a3p = &a3;
	char a4 = 4;
	char* a4p = &a4;
	proc(long a1, long *a1p, int a2, int *a2p, short a3, short *a3p, char a4, char *a4p);

	return 0;
}
```
* After `gcc -Og -S proc.c`, we get the assembly code below:
```z80
.globl  proc
.type   proc, @function
proc:
        movq    16(%rsp), %rax    # 8th arg: rax = rsp + 16 = *a4p
        addq    %rdi, (%rsi)      # *rsi += rdi: *a1p += a1
        addl    %edx, (%rcx)      # *rcx += edx: *a2p += a2
        addw    %r8w, (%r9)       # *r9  += r8w: *a3p += a3
        movl    8(%rsp), %edx     # edx = *(rsp+8) = a4
        addb    %dl, (%rax)       # *rax += dl: *a4p += a4
        ret

.globl  main
.type   main, @function
main:
        subq    $40, %rsp         # allocate stack frame
        movq    %fs:40, %rax            
        movq    %rax, 24(%rsp)    
        xorl    %eax, %eax        # clear eax
        movq    $1, 16(%rsp)      # *(16+rsp) = 1
        movl    $2, 12(%rsp)      # *(12+rsp) = 2
        movw    $3, 10(%rsp)      # *(10+rsp) = 3
        movb    $4, 9(%rsp)       # *(9 +rsp) = 4
        leaq    12(%rsp), %rcx    # 4th arg: rcx = rsp + 12
        leaq    16(%rsp), %rsi    # 2nd arg: rsi = rsp + 16
        leaq    9(%rsp), %rax     # rax = rsp + 9
        pushq   %rax              # 8th arg: rsp=rsp-8, *rsp = rax
        pushq   $4                # 7th arg: rsp=rsp-8, *rsp = 4
        leaq    26(%rsp), %r9     # 6th arg: r9 = 26 + rsp
        movl    $3, %r8d          # 5th arg: r8d = 3
        movl    $2, %edx          # 3rd arg: edx = 2
        movl    $1, %edi          # 1st arg: edi = 1
        call    proc              # push return address (rsp=rsp-8,*rsp=return address)
        addq    $16, %rsp
        movq    24(%rsp), %rax
        subq    %fs:40, %rax
        jne     .L5
        movl    $0, %eax
        addq    $40, %rsp
        ret
```

# Practice Problem 3.33
A C function procprob has four arguments u, a, v, and b. Each is either a signed number or a pointer to a signed number, where the numbers have different sizes. The function has the following body:
```c
	*u += a;
	*v += b;
	return sizeof(a) + sizeof(b);
```
It compiles to the following x86-64 code:
```z80
procprob:
	movslq %edi, %rdi
	addq %rdi, (%rdx)
	addb %sil, (%rcx)
	movl $6, %eax
	ret
```
Determine a valid ordering and types of the four parameters. There are two correct answers.

**Solution**:
```z80
procprob:
	movslq %edi, %rdi      # rdi = edi
	addq %rdi, (%rdx)      # *rdx += rdi
	addb %sil, (%rcx)      # *rcx +=%sil
	movl $6, %eax          # eax = 6
	ret
```
Answer1 - suppose `addq %rdi, (%rdx)` is `*u += a`, then:
* u is rdx, which is a 64bit address.
* a is edi, which is a 32bit signed integer.
* v is rcx, which is a 64bit address.
* b is sil, because eax = 6 and a is 4bytes, b is a 16bit signed integer.
Answer2 - suppose `addq %rdi, (%rdx)` is `*v += b`, then:
* v is rdx, which is a 64bit address.
* b is edi, which is a 32bit signed integer.
* u is rcx, which is a 64bit address.
* a is sil, because eax = 6 and b is 4bytes, a is a 16bit signed integer.

# 3.7.4 Local Storage on the Stack
* Local data must be stored in memory.
* Typically, a procedure allocates space on the stack frame by decrementing the stack pointer.
## Example:
```c
#include<stdio.h>

long swap_add(long *xp, long *yp)
{
	long x = *xp;
	long y = *yp;
	*xp = y;
	*yp = x;
	return x + y;
}

long caller()
{
	long arg1 = 534;
	long arg2 = 1057;
	long sum = swap_add(&arg1, &arg2);
	long diff = arg1 - arg2;
	return sum * diff;
}

int main()
{
	long a = caller();
	printf("The result is %ld\n",a);
	return 0;
}
```
* Then we do `gcc -Og caller.c -o prog` and 'objdump -d prog'. 
```z80
prog:     file format elf64-x86-64

Disassembly of section .text:

# long swap_add(long *xp, long *yp)
0000000000001169 <swap_add>:
    116d:       48 8b 07                mov    (%rdi),%rax
    1170:       48 8b 16                mov    (%rsi),%rdx
    1173:       48 89 17                mov    %rdx,(%rdi)
    1176:       48 89 06                mov    %rax,(%rsi)
    1179:       48 01 d0                add    %rdx,%rax
    117c:       c3                      ret    

000000000000117d <caller>:
    1181:       48 83 ec 28             sub    $0x28,%rsp
    1185:       64 48 8b 04 25 28 00    mov    %fs:0x28,%rax
    118c:       00 00 
    118e:       48 89 44 24 18          mov    %rax,0x18(%rsp)
    1193:       31 c0                   xor    %eax,%eax
    1195:       48 c7 44 24 08 16 02    movq   $0x216,0x8(%rsp)
    119c:       00 00 
    119e:       48 c7 44 24 10 21 04    movq   $0x421,0x10(%rsp)
    11a5:       00 00 
    11a7:       48 8d 74 24 10          lea    0x10(%rsp),%rsi
    11ac:       48 8d 7c 24 08          lea    0x8(%rsp),%rdi
    11b1:       e8 b3 ff ff ff          call   1169 <swap_add>
    11b6:       48 8b 54 24 08          mov    0x8(%rsp),%rdx
    11bb:       48 2b 54 24 10          sub    0x10(%rsp),%rdx
    11c0:       48 0f af c2             imul   %rdx,%rax
    11c4:       48 8b 54 24 18          mov    0x18(%rsp),%rdx
    11c9:       64 48 2b 14 25 28 00    sub    %fs:0x28,%rdx
    11d0:       00 00 
    11d2:       75 05                   jne    11d9 <caller+0x5c>
    11d4:       48 83 c4 28             add    $0x28,%rsp
    11d8:       c3                      ret    

00000000000011de <main>:
    11e2:       48 83 ec 08             sub    $0x8,%rsp
    11e6:       b8 00 00 00 00          mov    $0x0,%eax
    11eb:       e8 8d ff ff ff          call   117d <caller>
    11f0:       48 89 c2                mov    %rax,%rdx
    11f3:       48 8d 35 0a 0e 00 00    lea    0xe0a(%rip),%rsi        # 2004 <_IO_stdin_used+0x4>
    11fa:       bf 01 00 00 00          mov    $0x1,%edi
    11ff:       b8 00 00 00 00          mov    $0x0,%eax
    1204:       e8 67 fe ff ff          call   1070 <__printf_chk@plt>
    1209:       b8 00 00 00 00          mov    $0x0,%eax
    120e:       48 83 c4 08             add    $0x8,%rsp
    1212:       c3                      ret    
```
* Next we do annotation on `caller` and `swap_add` of the assembly code to understand how the local storage works.
```z80
prog:     file format elf64-x86-64

Disassembly of section .text:

# long swap_add(long *xp, long *yp)
0000000000001169 <swap_add>:
    116d:       48 8b 07                mov    (%rdi),%rax          # rax = *(rdi) - *xp
    1170:       48 8b 16                mov    (%rsi),%rdx          # rdx = *(rsi) - *yp
    1173:       48 89 17                mov    %rdx,(%rdi)          # *(rdi) = rdx - y
    1176:       48 89 06                mov    %rax,(%rsi)          # *(rsi) = rax - x
    1179:       48 01 d0                add    %rdx,%rax            # rax = x + y
    117c:       c3                      ret    

# long caller()
000000000000117d <caller>:
    1181:       48 83 ec 28             sub    $0x28,%rsp           # rsp-=0x28 - allocate 0x28 bytes for stack frame
    1185:       64 48 8b 04 25 28 00    mov    %fs:0x28,%rax        # stack protection - get a special value to rax
    118c:       00 00 
    118e:       48 89 44 24 18          mov    %rax,0x18(%rsp)      # save the special value to rsp+0x18
    1193:       31 c0                   xor    %eax,%eax            # clear eax
    1195:       48 c7 44 24 08 16 02    movq   $0x216,0x8(%rsp)     # *(rsp+0x8)=0x216  - arg1
    119c:       00 00 
    119e:       48 c7 44 24 10 21 04    movq   $0x421,0x10(%rsp)    # *(rsp+0x10)=0x421 - arg2
    11a5:       00 00 
    11a7:       48 8d 74 24 10          lea    0x10(%rsp),%rsi      # rsi=rsp+0x10 - &arg2
    11ac:       48 8d 7c 24 08          lea    0x8(%rsp),%rdi       # rdi=rsp+0x8  - &arg1
    11b1:       e8 b3 ff ff ff          call   1169 <swap_add>      # call swap_add
    11b6:       48 8b 54 24 08          mov    0x8(%rsp),%rdx       # rdx = *(rsp+0x8) - arg1
    11bb:       48 2b 54 24 10          sub    0x10(%rsp),%rdx      # rdx = rdx-*(rsp+0x10) - arg1-arg2
    11c0:       48 0f af c2             imul   %rdx,%rax            # rax = rax*rdx - sum*(arg1-arg2)
    11c4:       48 8b 54 24 18          mov    0x18(%rsp),%rdx      # stack protection - get the special value to rdx
    11c9:       64 48 2b 14 25 28 00    sub    %fs:0x28,%rdx        # compare the special value, if changed, means the stack has been broken.
    11d0:       00 00 
    11d2:       75 05                   jne    11d9 <caller+0x5c>
    11d4:       48 83 c4 28             add    $0x28,%rsp           # rsp+=0x28 - deallocate 0x28 bytes for stack frame
    11d8:       c3                      ret    

00000000000011de <main>:
    11e2:       48 83 ec 08             sub    $0x8,%rsp
    11e6:       b8 00 00 00 00          mov    $0x0,%eax
    11eb:       e8 8d ff ff ff          call   117d <caller>
    11f0:       48 89 c2                mov    %rax,%rdx
    11f3:       48 8d 35 0a 0e 00 00    lea    0xe0a(%rip),%rsi        # 2004 <_IO_stdin_used+0x4>
    11fa:       bf 01 00 00 00          mov    $0x1,%edi
    11ff:       b8 00 00 00 00          mov    $0x0,%eax
    1204:       e8 67 fe ff ff          call   1070 <__printf_chk@plt>
    1209:       b8 00 00 00 00          mov    $0x0,%eax
    120e:       48 83 c4 08             add    $0x8,%rsp
    1212:       c3                      ret    
```
* Ok, now let's detect how the program deal with the local variables:
	* for swap_add function, the program store the local varibles into registers:
	```
	116d:       48 8b 07                mov    (%rdi),%rax          # rax = *(rdi) - *xp
	1170:       48 8b 16                mov    (%rsi),%rdx          # rdx = *(rsi) - *yp
	```
	* for caller function, the program store the local variables into stack and also register:
	```
	1195:       48 c7 44 24 08 16 02    movq   $0x216,0x8(%rsp)     # *(rsp+0x8)=0x216  - arg1
    119c:       00 00 
    119e:       48 c7 44 24 10 21 04    movq   $0x421,0x10(%rsp)    # *(rsp+0x10)=0x421 - arg2
	```

	```
    11b1:       e8 b3 ff ff ff          call   1169 <swap_add>      # call swap_add
	```

# 3.7.5 Local Storage in Registers
* The set of program registers acts as a single resource shared by all of the procedures.
	* When one procedure (the caller) calls another (the callee), the callee must **not overwrite** some register value that the caller planned to use later.
* By convention: 
	* Registers `%rbx`, `%rbp`, and `%r12–%r15` are classified as **calleesaved registers**.
		* When procedure P calls procedure Q, Q must preserve the values of these registers, ensuring that they have the same values when Q returns to P as they did when Q was called. 
		* Procedure Q can preserve a register value by either not changing it at all or by **pushing** the original value on the stack, altering it, and then **popping** the old value from the stack before returning.
		* The pushing of register values has the effect of creating the portion of the **stack frame labeled “Saved registers”**.
		![[3_7 Procedures.assets/image-20240528125251661.png|400]]
		* P can safely store a value in a callee-saved register, call Q, and then use the value in the register without risk of it having been corrupted.
	* All other registers, except for the stack pointer `%rsp`, are classified as **callersaved registers**.
		* **callersaved registers** can be modified by any function.
		* Since Q is free to alter **callersaved registers** register, P needs to first save the data before it makes the call.
## Example:
```c
long P(long x, long y)
{
	long u = Q(y);
	long v = Q(x);
	return u + v;
}
```
* we do `gcc -Og -S p.c`, get the assembly code below:
```z80

    .text
    .globl  P
        .type   P, @function
	P:
        pushq   %rbp        # save rbp - calleesaved registers
        pushq   %rbx        # save rbx - calleesaved registers
        subq    $8, %rsp
        movq    %rdi, %rbp
        movq    %rsi, %rdi
        call    Q@PLT
        movq    %rax, %rbx
        movq    %rbp, %rdi
        call    Q@PLT
        addq    %rbx, %rax
        addq    $8, %rsp
        popq    %rbx       # restore rbx - calleesaved registers
        popq    %rbp       # restore rbp - calleesaved registers
        ret
```

# Practice Problem 3.34
Consider a function P, which generates local values, named `a0–a8`. It then calls function Q using these generated values as arguments. Gcc produces the following code for the first part of P:
```z80
# long P(long x)
# x in %rdi
P: 
	pushq %r15 
	pushq %r14 
	pushq %r13 
	pushq %r12 
	pushq %rbp 
	pushq %rbx 
	subq $24, %rsp 
	movq %rdi, %rbx 
	leaq 1(%rdi), %r15 
	leaq 2(%rdi), %r14 
	leaq 3(%rdi), %r13 
	leaq 4(%rdi), %r12 
	leaq 5(%rdi), %rbp 
	leaq 6(%rdi), %rax 
	movq %rax, (%rsp) 
	leaq 7(%rdi), %rdx 
	movq %rdx, 8(%rsp) 
	movl $0, %eax 
	call Q
	...
```
A. Identify which local values get stored in callee-saved registers.

B. Identify which local values get stored on the stack.

C. Explain why the program could not store all of the local values in calleesaved registers.

**Solution**:
```z80
# long P(long x)
# x in %rdi
P: 
	pushq %r15          # callee-saved register
	pushq %r14          # callee-saved register 
	pushq %r13          # callee-saved register 
	pushq %r12          # callee-saved register 
	pushq %rbp          # callee-saved register 
	pushq %rbx          # callee-saved register 
	subq $24, %rsp 
	movq %rdi, %rbx     # rbx = rdi: rbx=x
	leaq 1(%rdi), %r15  # r15 = rdi+1: r15=x+1
	leaq 2(%rdi), %r14  # r14 = rdi+2: r14=x+2
	leaq 3(%rdi), %r13  # r13 = rdi+3: r13=x+3
	leaq 4(%rdi), %r12  # r12 = rdi+4: r12=x+4
	leaq 5(%rdi), %rbp  # rbp = rdi+5: rbp=x+5
	leaq 6(%rdi), %rax  # rax = rdi+6: rax=x+6
	movq %rax, (%rsp)   # *rsp = rax: *rsp=x+6
	leaq 7(%rdi), %rdx  # rdx = rdi+7: rdx=x+7
	movq %rdx, 8(%rsp)  # *(rsp+8) = rdx: *(rsp+8)=x+7
	movl $0, %eax 
	call Q
	...
```
A.
We get in this function, the callee-saved registers are: `rbx rbp r12 r13 r14 r15`
Thus local values are: `x, x+1, x+2, x+3, x+4, x+5`

B.
Local values stored on the stack are: `x+6, x+7`

C.
Limited quantity of callee-saved registers.

# 3.7.6 Recursive Procedures (递归过程)
* The conventions we have described for using the registers and the stack allow x86-64 procedures to call themselves recursively, and so the local variables of the multiple outstanding calls do not interfere with one another.
## Example:
```c
long rfact(long n)
{
	long result;
	if (n <= 1)
		result = 1;
	else
		result=n* rfact(n-1);
	return result;
}
```
* We do `gcc -Og -S rfact.c` and get the assembly code below:
```z80
.text
.globl  rfact
.type   rfact, @function
rfact:
        cmpq    $1, %rdi
        jg      .L8              # if rdi(n)>1, jump to .L8
        movl    $1, %eax         # eax = 1: result = 1
        ret
.L8:
        pushq   %rbx             # callee-saved register
        movq    %rdi, %rbx       # rbx = rdi: rbx=n
        leaq    -1(%rdi), %rdi   # rdi = rdi-1: rdi=n-1
        call    rfact            # rfact(n-1)
        imulq   %rbx, %rax       # rax = rax*rbx: rax=n*rfact(n-1)
        popq    %rbx
        ret
```
* Our stack discipline provides a mechanism where each invocation of a function has its own private storage for state information (saved values of the return location and callee-saved registers).
	* If need be, it can also provide storage for local variables.

# Practice Problem 3.35
For a C function having the general structure
```c
long rfun(unsigned long x) {
	if ( ______ ) return;
	unsigned long nx = ______ ;
	long rv = rfun(nx);
	return ______ ;
}
```
gcc generates the following assembly code:
```z80
rfun: 
	pushq %rbx 
	movq %rdi, %rbx 
	movl $0, %eax 
	testq %rdi, %rdi 
	je .L2 
	shrq $2, %rdi 
	call rfun 
	addq %rbx, %rax 
.L2: 
	popq %rbx 
	ret
```
A. What value does `rfun` store in the callee-saved register `%rbx`?

B. Fill in the missing expressions in the C code shown above.

**Solution**:
```z80
rfun: 
	pushq %rbx         # callee-saved register
	movq %rdi, %rbx    # rbx = rdi: rbx=x
	movl $0, %eax      # eax = 0
	testq %rdi, %rdi   
	je .L2             # if rdi(x) == 0, jump to .L2
	shrq $2, %rdi      # rdi = rdi >> 2: rdi=x>>2
	call rfun          # rfun(rdi): rfun(x>>2)
	addq %rbx, %rax    # rax = rax + rbx: rax=rfun(x>>2)+x
.L2: 
	popq %rbx 
	ret
```
A. `x`
B.
```c
long rfun(unsigned long x) {
	if ( x == 0 ) return;
	unsigned long nx = x >> 2 ;
	long rv = rfun(nx);
	return rv + x ;
}
```