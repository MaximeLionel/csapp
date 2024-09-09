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

## Processing Y86-64 instructions
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
* 4 integer operations (`addq`, `subq`, `andq`, and `xorq`) all have the same value of `icode`. We can handle them all by an identical sequence of steps, except that the **ALU** computation must be set according to the particular instruction operation, encoded in `ifun`.
#### Details of processing OPq
```
OPq rA, rB
```
* Fetch - In the **fetch** stage, we do not require a constant word, and so `valP` is computed as PC + 2. 
	* The encoding of `OPq`: ![[Pasted image 20240815110341.png|150]]
	* The operations of `OPq` on Fetch stage:
	```
		icode:ifun <- M1[PC]    
		rA:rB <- M1[PC+1]       // read register IDs
		valP <- PC+2
	```
* Decode - During the **decode** stage, we read both operands. 
	* The operations of `OPq` on Decode stage:
	```
		valA <- rA  // read register rA and give value to valA
		valB <- rB  // read register rB and give value to valB
	```
* Execute - Both operands are supplied to the ALU in the **execute** stage, along with the function specifier `ifun`, so that `valE` becomes the instruction result. 
	* This computation is shown as the expression `valB OP valA`, where `OP` indicates the operation specified by `ifun`. 
	* Note the ordering of the two arguments—this order is consistent with the conventions of Y86-64 (and x86-64). 
		* For example, the instruction `subq %rax,%rdx` is supposed to compute the value `R[%rdx] − R[%rax]`. 
	```
		valE <- valB OP valA    // ALU performs computations and 
		Set CC
	```
* Memory - Nothing happens in the memory stage for these instructions.
* Write back - `valE` is written to register `rB` in the write-back stage.
	```
		R[rB] <- valE
	```
* PC update - the PC is set to `valP` to complete the instruction execution.
	```
		PC <- valP
	```
#### Details of processing rrmovq
```
rrmovq rA, rB
```
The encoding of `rrmovq`: ![[Pasted image 20240815150537.png|150]]
* Fetch
	* extracts the two 4-bit portions of the instruction specifier byte: `icode` and `ifun`.
	* fetches a register specifier byte: `rA` and `rB`.
	* computes `valP`.
	```
	icode :ifun ← M1[PC]
	rA :rB ← M1[PC + 1]
	valP ← PC + 2
	```
* Decode
	* reads up to 1 operand from the register file, giving value `valA`.
	```
	valA ← R[rA]
	```
* Execute
	* `arithmetic/logic unit (ALU)` performs the operation specified by the instruction.
	```
	valE ← 0 + valA
	```
* Memory - Nothing happens in the memory stage for these instructions.
* Write back - `valE` is written to register `rB` in the write-back stage.
	```
	R[rB] <- valE
	```
* PC update - the PC is set to `valP` to complete the instruction execution.
	```
	PC <- valP
	```

#### Details of processing irmovq
```
irmovq V, rB
```
The encoding of `irmovq`: ![[Pasted image 20240815155517.png|400]]
* Fetch
	* extracts the two 4-bit portions of the instruction specifier byte: `icode` and `ifun`.
	* fetches a register specifier byte: `rA` and `rB`.
	* fetches an 8-byte constant word `valC`.
	* computes `valP`.
	```
	icode :ifun ← M1[PC]
	rA :rB ← M1[PC + 1]
	valC ← M8[PC + 2]
	valP ← PC + 10
	```
* Decode - Nothing happens in the memory stage for these instructions.
* Execute - `arithmetic/logic unit (ALU)` performs the operation specified by the instruction.
	```
	valE ← 0 + valC
	```
* Memory - Nothing happens in the memory stage for these instructions.
* Write back - `valE` is written to register `rB` in the write-back stage.
	```
	R[rB] ← valE
	```
* PC update - the PC is set to `valP` to complete the instruction execution.
	```
	PC <- valP
	```

### rmmovq and mrmovq - memory write and read instructions
![[Pasted image 20240815161351.png|400]]
* Some notices: 
	* On execute stage, use **ALU** to add `valC` to `valB`, giving the effective address (the sum of the displacement and the base register value) for the memory operation. 
	* On the memory stage, either write the register value `valA` to memory or read `valM` from memory.

### pushq and popq instructions
![[Pasted image 20240815165649.png|400]]
* Some notices on `pushq rA`:
	* On **decode** stage, use `%rsp` as the identifier for the second register operand, giving the stack pointer as value `valB`. 
	* On **execute** stage, use the `ALU` to decrement the stack pointer by 8. 
	* On **write-back** stage, the decremented value is used for the memory write address and is also stored back to `%rsp`.
	* By using `valE` as the address for the write operation, we adhere to the Y86-64 (and x86-64) convention that `pushq` should decrement the stack pointer before writing, even though the actual updating of the stack pointer does not occur until after the memory operation has completed.
* Some notices on `popq rA`:
	* On **decode** stage, read 2 copies of the stack pointer to enhance the overall uniformity of the design. 
		```
		valA ← R[%rsp]
		valB ← R[%rsp]
		```
	* On **execute** stage, use the ALU to increment the stack pointer by 8 in the execute stage.
		```
		valE ← valB + 8
		```
	* On **memory** stage, use the un-incremented value as the address for the memory operation.
		```
		valM ← M8[valA]
		```
	* On **write-back** stage, update both the stack pointer register with the incremented stack pointer and register `rA` with the value read from memory. 
		```
		 R[%rsp] ← valE
		 R[rA] ← valM
		```
	* Using the un-incremented stack pointer as the memory read address preserves the Y86-64 (and x86-64) convention that **`popq` should first read memory and then increment the stack pointer**.

### jXX, call, and ret instructions
![[Pasted image 20240816090343.png|500]]
* Some notices on `jXX Dest`:
	* Process all of the jumps in a uniform manner, since they differ only when determining whether or not to take the branch. 
	* On **fetch** and **decode** stage, a jump instruction proceeds through fetch and decode much like the previous instructions, except that it does not require a register specifier byte.
	* On the **execute** stage, check the condition codes and the jump condition to determine whether or not to take the branch, yielding a 1-bit signal `Cnd`. 
	* On the **PC update** stage, test this `Cnd`flag and set the `PC` to `valC` (the jump target) if the flag is 1 and to `valP` (the address of the following instruction) if the flag is 0.
* Some notices on `call` and `ret`:
	* With instruction `call`, we `push valP`, the address of the instruction that follows the call instruction. 
		* On the **PC update** stage, we set the PC to `valC`, the call destination. 
	* With instruction `ret`, we assign `valM`, the value popped from the stack, to the PC in the **PC update** stage.

## Tracing Y86-64 instructions
### Tracing the execution of a subq instruction
```
0x000: 30f20900000000000000    |   irmovq $9, %rdx
0x00a: 30f31500000000000000    |   irmovq $21, %rbx
0x014: 6123                    |   subq %rdx, %rbx # subtract
```

![[Pasted image 20240801144810.png|250]]
![[Pasted image 20240728220428.png|70]]

| Stage      | OPq rA, rB                                                  | subq %rdx, %rbx                                                       |
| ---------- | ----------------------------------------------------------- | --------------------------------------------------------------------- |
| Fetch      | icode:ifun ← M1[PC]<br>rA :rB ← M1[PC + 1]<br>valP ← PC + 2 | icode:ifun ← 6:1<br>rA:rB ← 2:3 = %rdx:%rbx<br>valP ← 0x14 + 2 = 0x16 |
| Decode     | valA ← R[rA]<br>valB ← R[rB]                                | valA ← R[rA] = 9<br>valB ← R[rB] = 21                                 |
| Execute    | valE ← valB OP valA<br>Set CC                               | valE ← 21-9 = 12<br>OF ← 0, ZF ← 0, SF ← 0                            |
| Memory     |                                                             |                                                                       |
| Write-back | R[rB] ← valE                                                | R(%rbx) ← 12                                                          |
| PC update  | PC ← valP                                                   | PC ← 0x16                                                             |

![[Pasted image 20240816111320.png|400]]



# Practice Problem 4.13
Fill in the right-hand column of the following table to describe the processing of the `irmovq` instruction below:
```
0x016: 30f48000000000000000    |   irmovq $128,%rsp # Problem 4.13
```

| Stage      | Generic<br>irmovq V, rB                                                           | Specific<br>irmovq $128, %rsp |
| ---------- | --------------------------------------------------------------------------------- | ----------------------------- |
| Fetch      | icode:ifun ← M1[PC]<br>rA :rB ← M1[PC + 1]<br>valC ← M8[PC + 2]<br>valP ← PC + 10 |                               |
| Decode     | -                                                                                 |                               |
| Execute    | valE ← 0 + valC                                                                   |                               |
| Memory     | -                                                                                 |                               |
| Write-back | R[rB] ← valE                                                                      |                               |
| PC-update  | PC ← valP                                                                         |                               |
**Solution**:
PC = 0x016

| Stage      | Generic<br>irmovq V, rB                                                           | Specific<br>irmovq $128, %rsp                                                                                                                                                            |
| ---------- | --------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Fetch      | icode:ifun ← M1[PC]<br>rA :rB ← M1[PC + 1]<br>valC ← M8[PC + 2]<br>valP ← PC + 10 | $M_1$[0x016] = 0x30<br>icode:ifun ← 3:0<br><br>$M_1$[0x017] = 0xf4<br>rA :rB ← f:4<br><br>$M_8$[0x018] = 0x 0000000000000080<br>(little-endian)<br>valC ← 0x80 = 128<br><br>valP ← 0x020 |
| Decode     | -                                                                                 |                                                                                                                                                                                          |
| Execute    | valE ← 0 + valC                                                                   | valE ← 0 + 0x80 = 128                                                                                                                                                                    |
| Memory     | -                                                                                 |                                                                                                                                                                                          |
| Write-back | R[rB] ← valE                                                                      | %rsp ← 128                                                                                                                                                                               |
| PC-update  | PC ← valP                                                                         | PC ← 0x020                                                                                                                                                                               |

# Practice Problem 4.14
Fill in the right-hand column of the following table to describe the processing of
the `popq` instruction on line 7 of the code below:

```
0x000: 30f20900000000000000    |   irmovq $9, %rdx
0x00a: 30f31500000000000000    |   irmovq $21, %rbx
0x014: 6123                    |   subq %rdx, %rbx            # subtract
0x016: 30f48000000000000000    |   irmovq $128,%rsp           # Problem 4.13
0x020: 40436400000000000000    |   rmmovq %rsp, 100(%rbx)     # store
0x02a: a02f                    |   pushq %rdx                 # push
0x02c: b00f                    |   popq %rax                  # Problem 4.14
0x02e: 734000000000000000      |   je done                    # Not taken
0x037: 804100000000000000      |   call proc                  # Problem 4.18
0x040:                         | done:
0x040: 00                      |   halt
0x041:                         | proc:
0x041: 90                      |   ret                        # Return
```

| Stage      | Generic<br>popq rA                                                | Specific<br>popq %rax |
| ---------- | ----------------------------------------------------------------- | --------------------- |
| Fetch      | icode:ifun ← $M_1$[PC]<br>rA :rB ← $M_1$[PC + 1]<br>valP ← PC + 2 |                       |
| Decode     | valA ← R[%rsp]<br>valB ← R[%rsp]                                  |                       |
| Execute    | valE ← valB + 8                                                   |                       |
| Memory     | valM ← $M_8$[valA]                                                |                       |
| Write-back | R[%rsp] ← valE<br>R[rA] ← valM                                    |                       |
| PC-update  | PC ← valP                                                         |                       |
What effect does this instruction execution have on the registers and the PC?

**Solution**:
`0x02c: b00f | popq %rax`

| Stage      | Generic<br>popq rA                                                | Specific<br>popq %rax                                                             |
| ---------- | ----------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| Fetch      | icode:ifun ← $M_1$[PC]<br>rA :rB ← $M_1$[PC + 1]<br>valP ← PC + 2 | icode:ifun ← $M_1$[0x02c] = 0xb0<br>rA :rB ← $M_1$[0x02d] = 0x0f<br>valP ← PC + 2 |
| Decode     | valA ← R[%rsp]<br>valB ← R[%rsp]                                  | valA ← R[%rsp] = <br>valB ← R[%rsp]                                               |
| Execute    | valE ← valB + 8                                                   |                                                                                   |
| Memory     | valM ← $M_8$[valA]                                                |                                                                                   |
| Write-back | R[%rsp] ← valE<br>R[rA] ← valM                                    |                                                                                   |
| PC-update  | PC ← valP                                                         |                                                                                   |





























