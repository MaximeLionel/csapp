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
* In this figure, we show the assembly-code representation of the instructions on the left and the byte encodings on the right.
* The set of Y86-64 instructions is largely a subset of the x86-64 instruction set.
* Y86-64 Instruction encodings range between 1 and 10 bytes.
* An instruction consists of a 1-byte instruction specifier, possibly a 1-byte register specifier, and possibly an 8-byte constant word.
* Field `fn` specifies a particular integer operation (`OPq`), data movement condition (`cmovXX`), or branch condition (`jXX`). 
* All numeric values are shown in hexadecimal.


## `MOVQ` - data transfer instruction
![[Pasted image 20240730223317.png|500]]
* The Y86-64 `movq` instruction is split into four different instructions: `irmovq`, `rrmovq`, `mrmovq`, and `rmmovq`, explicitly indicating the form of the source and destination.
* The source is either immediate (i), register (r), or memory (m). It is designated by the first character in the instruction name.
* The destination is either register (r) or memory (m). It is designated by the second character in the instruction name.
* The memory references for the two memory movement instructions have a simple base and displacement format.
* As with x86-64, we do not allow direct transfers from one memory location to another. In addition, we do not allow a transfer of immediate data to memory.

## `OPq` - integer operation instructions
![[Pasted image 20240730225333.png|300]]

![[Pasted image 20240730225244.png|100]]
* 4 integer operation instructions: `addq`, `subq`, `andq`, and `xorq`.
* Y86-64 `OPq` operate only on register data, whereas x86-64 `OPq` also allows operations on memory data.
* Y86-64 `OPq` set the three condition codes ZF, SF, and OF (zero, sign, and overflow).

## `JXX` - jump instructions
* 7 jump instructions: `jmp`, `jle`, `jl`, `je`, `jne`, `jge`, and `jg`.
* Branches are taken according to the type of branch and the settings of the condition codes.

## `CMOV` - conditional move instructions
* 6 conditional move instructions: `cmovle`, `cmovl`, `cmove`, `cmovne`, `cmovge`, and `cmovg`.


















