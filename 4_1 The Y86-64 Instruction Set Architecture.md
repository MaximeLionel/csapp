# 4.1.1 Programmer-Visible State
## Registers
![[Pasted image 20240727231255.png|300]]
* 15 program registers: %rax, %rcx, %rdx, %rbx, %rsp, %rbp, %rsi, %rdi, and %r8 through %r14.
	* omit the x86-64 register %r15 to simplify the instruction encoding.
* Each of the registers stores a 64-bit word.
* Register %rsp is used as a stack pointer by the push, pop, call, and return instructions.
* The program counter (PC) holds the address of the instruction currently being executed.

## Memory
* The memory is conceptually **a large array of bytes**, holding both program and data.
* Y86-64 programs reference memory locations using ==virtual addresses==.
	* A combination of hardware and operating system software translates these into the **actual, or physical, addresses** indicating where the values are actually stored in memory.

## Status code (Stat)
* A final part of the program state is a **status code Stat**, indicating the overall state of program execution.
* It will indicate either normal operation or that some sort of ==exception== has occurred, such as when an instruction attempts to read from an invalid memory address.
* There are three single-bit condition codes, ZF, SF, and OF, storing information about the effect of the most recent arithmetic or logical instruction.
	![[Pasted image 20240728220428.png|100]]

# 4.1.2 Y86-64 Instructions
![[Pasted image 20240728220633.png|500]]
* The set of Y86-64 instructions is largely a subset of the x86-64 instruction set.









