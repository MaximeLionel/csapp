# 4.1.1 Programmer-Visible State
## Registers
![[Pasted image 20240727231255.png|300]]
* 15 program registers: %rax, %rcx, %rdx, %rbx, %rsp, %rbp, %rsi, %rdi, and %r8 through %r14.
	* omit the x86-64 register %r15 to simplify the instruction encoding.
* Each of the registers stores a 64-bit word.
* Register %rsp is used as a stack pointer by the push, pop, call, and return instructions.
* The program counter (PC) holds the address of the instruction currently being executed.


