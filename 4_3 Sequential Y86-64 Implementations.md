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
	* In the execute stage, the `arithmetic/logic unit (ALU)` either performs the operation specified by the instruction (according to the value of `ifun`), computes the effective address of a memory reference, or increments or decrements the stack pointer. We refer to the resulting value as valE. The
condition codes are possibly set. For a conditional move instruction, the
stage will evaluate the condition codes and move condition (given by ifun)
and enable the updating of the destination register only if the condition
holds. Similarly, for a jump instruction, it determines whether or not the
branch should be taken.
Memory. The memory stage may write data to memory, or it may read data
from memory. We refer to the value read as valM.
Write back. The write-back stage writes up to two results to the register file.
PC update. The PC is set to the address of the next instruction.
