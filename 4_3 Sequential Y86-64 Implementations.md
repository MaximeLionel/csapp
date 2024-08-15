* Our processor is called **SEQ** (for “sequential” processor).

# 4.3.1 Organizing Processing into Stages
* In general, processing an instruction involves a number of operations.
## Operation Stages
* ==Fetch==. 
	* The fetch stage reads the bytes of an instruction from memory, using the **program counter (PC)** as the memory address. 
	* `Fetch` operation extracts the two 4-bit portions of the instruction specifier byte, referred to as `icode` (the instruction code) and `ifun` (the instruction function). 
	* `Fetch` operation possibly fetches a register specifier byte, giving one or both of the register operand specifiers rA and rB. 
	* `Fetch` operation also possibly fetches an 8-byte constant word `valC`. 
	* `Fetch` operation computes `valP` to be the address of the instruction following the current one in sequential order. `valP` equals the value of the PC plus the length of the fetched instruction.
* ==Decode==. 
	* The decode stage reads up to two operands from the register file, giving values `valA` and/or `valB`. 
	* `Decode` operation reads the registers designated by instruction fields `rA` and `rB`, but for some instructions it reads register `%rsp`.
* ==Execute==. 
	* In the execute stage, the `arithmetic/logic unit (ALU)` either performs the operation specified by the instruction (according to the value of `ifun`), computes the effective address of a memory reference, or increments or decrements the stack pointer. 
	* We refer to the resulting value as `valE`. 
	* The `condition codes` are possibly set. For a conditional move instruction, the stage will evaluate the condition codes and move condition (given by `ifun`) and enable the updating of the destination register only if the condition holds. 
	* For a `jump` instruction, it determines whether or not the branch should be taken.
* ==Memory==. 
	* The memory stage may **write data** to memory, or it may **read data** from memory. 
	* We refer to the value read as `valM`.
* ==Write back==. 
	* The write-back stage writes up to 2 results to the register file.
* ==PC update==. 
	* The PC is set to **the address of the next instruction**.
## Logics
* The processor loops indefinitely, performing these stages. 
* In our simplified implementation, the processor will stop when any **exception** occurs—that is, when it executes a **halt or invalid instruction**, or it attempts to **read or write an invalid address**. 
* In a more complete design, the processor would enter an **exception-handling** mode and begin executing special code determined by the type of exception.
* There is a surprising amount of processing required to execute a single instruction. 
	* Not only must we perform the stated operation of the instruction, we must also compute addresses, update stack pointers, and determine the next instruction address. 
* Fortunately, the overall flow can be similar for every instruction.
* Using a very simple and uniform structure is important when designing hardware, since we want to minimize the total amount of hardware.
	* We must ultimately map it onto the two-dimensional surface of an integrated-circuit chip. 
	* One way to minimize the complexity is to have the different instructions share as much of the hardware as possible. 
		* For example, each of our processor designs contains a single arithmetic/logic unit that is used in different ways depending on the type of instruction being executed. 
		* The cost of duplicating blocks of logic in hardware is much higher than the cost of having multiple copies of code in software. 
		* It is also more difficult to deal with many special cases and idiosyncrasies in a hardware system than with software.

## Example - processing Y86-64 instructions
```
0x000: 30f20900000000000000    |   irmovq $9, %rdx
0x00a: 30f31500000000000000    |   irmovq $21, %rbx
0x014: 6123                    |   subq %rdx, %rbx # subtract
0x016: 30f48000000000000000    |   irmovq $128,%rsp # Problem 4.13
0x020: 40436400000000000000    |   rmmovq %rsp, 100(%rbx) # store
0x02a: a02f                    |   pushq %rdx # push
0x02c: b00f                    |   popq %rax # Problem 4.14
0x02e: 734000000000000000      |   je done # Not taken
0x037: 804100000000000000      |   call proc # Problem 4.18
0x040:                         | done:
0x040: 00                      |   halt
0x041:                         | proc:
0x041: 90                      |   ret # Return
```
### OPq, rrmovq and irmovq instructions
![[Pasted image 20240815103811.png|500]]
* Encodings of Y86-64 instructions:
	![[Pasted image 20240728220633.png|500]]







Let us first consider the integer operations. Examining Figure 4.2,
we can see that we have carefully chosen an encoding of instructions so that the
four integer operations (addq, subq, andq, and xorq) all have the same value of
icode. We can handle them all by an identical sequence of steps, except that the
ALU computation must be set according to the particular instruction operation,
encoded in ifun.