* We are finally ready for the major task of this chapter — designing a pipelined Y86-64 processor.
* Steps on modification:
	* Shift the computation of the PC into the fetch stage.
	* Then add pipeline registers between the stages.

# 4.5.1 SEQ+: Rearranging the Computation Stages
* Rearrange the order of the 5 stages in SEQ so that the PC update stage comes at the beginning of the clock cycle, rather than at the end. We refer to this modified design as **SEQ+**.
## Shifting the timing of the PC computation
* We can move the PC update stage so that its logic is active at the beginning of the clock cycle by making it compute the PC value for the current instruction.
	![[Pasted image 20240923153716.png|400]]
	* With SEQ, the PC computation takes place at the end of the clock cycle, computing the new value for the PC register based on the values of signals computed during the current clock cycle.
	* With SEQ+, we create **state registers** to hold the signals computed during an instruction.
		* As a new clock cycle begins, the values propagate through the exact same logic to compute the PC for the now-current instruction.
		* The state registers are labeled as **pIcode, pCnd, and so on**.
* No hardware PC in SEQ+:
	* The PC is computed dynamically based on some state information stored from the previous instruction.

## SEQ+ hardware structure
![[Pasted image 20240923155237.png|400]]
* SEQ+ contains the exact same hardware units and control blocks that we had in SEQ, but with the PC logic shifted from the top, where it was active at the end of the clock cycle, to the bottom, where it is active at the beginning.
* The shift of state elements from SEQ to SEQ+ is an example of a general transformation known as **circuit retiming**.
	* Retiming changes the state representation for a system without changing its logical behavior. 
	* Retiming is often used to balance the delays between the different stages of a pipelined system.

# 4.5.2 Inserting Pipeline Registers
* In our first attempt at creating a pipelined Y86-64 processor, we insert **pipeline registers** between the stages of SEQ+ and rearrange signals somewhat, yielding the **PIPE−** processor, where the “−” in the name signifies that this processor has somewhat **less performance** than our ultimate processor design.

## Hardware structure of PIPE−
![[Pasted image 20240923161539.png|400]]
* The pipeline registers are shown in this figure as blue boxes, each containing different fields that are shown as white boxes.
* PIPE− uses nearly the same set of hardware units as our sequential design SEQ, but with the pipeline registers separating the stages.
* The pipeline registers of PIPE-:
	* F holds a predicted value of the program counter.
	* D holds information about the most recently fetched instruction for processing by the decode stage.
	* E holds information about the most recently decoded instruction and the values read from the register file for processing by the execute stage.
	* M holds:
		* Results of the most recently executed instruction for processing by the memory stage.
		* Information about branch conditions and branch targets for processing conditional jumps.
	* W supplies the computed results to the register file for writing and the return address to the PC selection logic when completing a `ret` instruction.

## Example of instruction flow through pipeline
![[Pasted image 20240923163336.png|450]]
* This diagram shows the progression of each instruction through the pipeline stages, with time increasing from left to right.
* Details:
	* Instruction I1 is fetched In cycle 1, and it then proceeds through the pipeline stages, with its result being written to the register file after the end of cycle 5. 
	* Instruction I2 is fetched in cycle 2, and its result is written back after the end of cycle 6, and so on. 
	* At the bottom, we show an expanded view of the pipeline for cycle 5. At this point, there is one instruction in each of the pipeline stages.

# 4.5.3 Rearranging and Relabeling Signals
![[Pasted image 20240923161539.png|500]]

* Potential serious errors on PIPE-:
	* Our sequential implementations SEQ and SEQ+ only process one instruction at a time, and so there are unique values for signals such as valC, srcA, and valE. 
	* In our pipelined design, there will be multiple versions of these values associated with the different instructions flowing through the system.
	* For example, in the detailed structure of PIPE−, there are 4 white boxes labeled “Stat” that hold the status codes for four different instructions.
	* We need to take great care to make sure we use the proper version of a signal, or else we could have serious errors, such as storing the result computed for one instruction at the destination register specified by another instruction.

## Naming scheme
* Naming scheme:
	* We adopt a naming scheme where a signal stored in a pipeline register can be uniquely identified by **prefixing its name with that of the pipe register written in uppercase**.
		* For example, the 4 status codes are named `D_stat`, `E_stat`, `M_stat`, and `W_stat`.
	* Some signals that have just been computed within a stage, are labeled by prefixing the signal name with the first character of the stage name, written in lowercase.
		* Using the status codes as examples, we can see control logic blocks labeled “Stat” in the fetch and memory stages. The outputs of these blocks are therefore named `f_stat` and `m_stat`.
## Rearranging
* `Stat` - We can also see that the actual status of the overall processor `Stat` is computed by a block in the write-back stage, based on the status value in pipeline register `W`.
* `dstE` and `dstM` - The decode stages of SEQ+ and PIPE− both generate signals `dstE` and `dstM` indicating the destination register for values `valE` and `valM`. 
	* In SEQ+, we could connect `dstE` and `dstM` signals directly to the address inputs of the register file write ports. 
		![[Pasted image 20240925142430.png|300]]
	* With PIPE−, `dstE` and `dstM` signals are carried along in the pipeline through the execute and memory stages and are directed to the register file only once they reach the write-back stage.
		![[Pasted image 20240925142612.png|300]]
		* We do this to make sure the write port address and data inputs hold values from the same instruction. 
		* Otherwise, the write back would be writing the values for the instruction in the write-back stage, but with register IDs from the instruction in the decode stage. 
		* As a general principle, we want to keep all of the information about a particular instruction contained within a single pipeline stage.
* `SelectA`:
	![[Pasted image 20240925144046.png|400]]
	* One block of PIPE− that is not present in SEQ+ in the exact same form is the block labeled “Select A” in the decode stage.
	* `SelectA` block:
		* Output: generates the value `valA` for the pipeline register E
		* Input: choosing either `valP` from pipeline register D or the value `d_rvalA` read from the A port of the register file.
	* `SelectA` block is included to reduce the amount of state that must be carried forward to pipeline registers E and M.
	* Why be confident to choose either `valP` or `d_rvalA`?
		* Of all the different instructions, only the `call` requires `valP` in the memory stage. Only the `jump` instructions require the value of `valP` in the execute stage (in the event the jump is not taken). 
		* None of these instructions requires a value read from the register file.
		* Therefore, we can reduce the amount of pipeline register state by merging these two signals and carrying them through the pipeline as a single signal `valA`.
* `Stat` - the pipeline registers include a field for the status code `stat`, initially computed during the fetch stage and possibly modified during the memory stage.

# 4.5.4 Next PC Prediction
* Our goal in the pipelined design is to issue a new instruction on every clock cycle, meaning that on each clock cycle, a new instruction proceeds into the execute stage and will ultimately be completed.
* Achieving this goal would yield a throughput of 1 instruction per cycle.
* To do this, we must determine the location of the next instruction right after fetching the current instruction. However:
	* If the fetched instruction is a conditional branch, we will not know whether or not the branch should be taken until several cycles later, after the instruction has passed through the execute stage. 
	* If the fetched instruction is a `ret`, we cannot determine the return location until the instruction has passed through the memory stage.
## Conditional Jump - Branch Prediction

| Stage     | jXX Dest                                                           | ret                                            | jump Dest                                       |
| --------- | ------------------------------------------------------------------ | ---------------------------------------------- | ----------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[PC]$<br>$valC ← M_8[PC + 1]$<br>$valP ← PC + 9$ | $icode :ifun ← M_1[PC]$<br><br>$valP ← PC + 9$ | $icode :ifun ← M_1[PC]$<br>$valC ← M_8[PC + 1]$ |
| Decode    | -                                                                  | $valA ← R[\%rsp]$<br>$valB ← R[\%rsp]$         | -                                               |
| Execute   | <br>$Cnd ← Cond(CC, ifun)$                                         | $valE ← valB + 8$                              | -                                               |
| Memory    | -                                                                  | $valM ← M_8[valA]$                             | -                                               |
| Writeback | -                                                                  | $R[\%rsp] ← valE$                              | -                                               |
| PC update | $PC ← Cnd ? ~valC : valP$                                          | $PC ← valM$                                    | $PC←valC$                                       |
* With the exception of conditional jump instructions and `ret`, we can determine the address of the next instruction based on information computed during the fetch stage.
	* For `call` and `jmp` (unconditional jump), it will be valC, the constant word in the instruction, while for all others it will be valP, the address of the next instruction.
* For conditional jumps, we can predict either that a jump will be taken, so that the new PC value would be `valC`, or that it will not be taken, so that the new PC value would be `valP`.
* This technique of guessing the branch direction and then initiating the fetching of instructions according to our guess is known as **branch prediction**.
* In our design, we will use the simple strategy of predicting that conditional branches are always taken, and so we predict the new value of the PC to be `valC`.

## ret - simply hold off
* We will simply hold off processing any more instructions until the `ret` instruction passes through the write-back stage.

## Predict PC block
![[Pasted image 20240925224905.png|400]]
* The PIPE− fetch stage, diagrammed at the bottom of hardware structure, is responsible for both predicting the next value of the PC and selecting the actual PC for the instruction fetch.
* The block labeled “Predict PC” can choose either `valP` (as computed by the PC incrementer) or `valC` (from the fetched instruction).
* This value is stored in pipeline register F as the predicted value of the program counter.
* The block labeled “Select PC” is similar to the block labeled “PC” in the SEQ+ PC selection stage. 
	* It chooses one of 3 values to serve as the address for the instruction memory: 
		* the predicted PC 
		* the value of `valP` for a not-taken branch instruction that reaches pipeline register M (stored in register M_valA)
		* the value of the return address when a `ret` instruction reaches pipeline register W (stored in W_valM).

# 4.5.5 Pipeline Hazards
* Introducing pipelining into a system with feedback can lead to problems when there are **dependencies** between successive instructions.
* Dependencies can take 2 forms:
	* data dependencies - the results computed by one instruction are used as the data for a following instruction.
	* control dependencies - one instruction determines the location of the following instruction, such as when executing a `jump`, `call`, or `ret`.
* When such dependencies have the potential to cause an erroneous computation by the pipeline, they are called hazards. Like dependencies, hazards can be classified as either **data hazards** or **control hazards**.

## Data hazard Examples
### Example 1 - 3 nops - No data hazard
![[Pasted image 20240926110638.png|500]]

| Stage     | 0x00a: irmovq $3,%rax<br>V  = $3<br>rB = %rax                                                         | 0x017: addq %rdx,%rax<br>rA = %rdx<br>rB = %rax                                                                                                          |
| --------- | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=3$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br><br>$valP ← PC + 2$                                                                                 |
| Decode    | $E\_valC ← D\_valC$<br>$E\_dstE←D\_dstE$                                                              | $E\_valA ← R[\%rdx]$:<br> - d_srcA = %rdx<br> - E_valA <- d_rvalA<br>$E\_valB ← R[\%rax]$:<br> - d_srcB = %rax<br> - E_valB <- d_rvalB<br>E_dstE <- %rax |
| Execute   | $M\_valE ← 0 + E\_valC ~= 3$<br>$M\_dstE←E\_dstE$                                                     | M_valE <- E_valB + E_valA<br>M_dstE <- E_dstE = %rax<br>$Set~CC$                                                                                         |
| Memory    | W_valE <- M_valE = 3<br>W_dstE <- M_dstE                                                              | W_valE <- M_valE<br>W_dstE <- M_dstE = %rax                                                                                                              |
| Writeback | %rax <- W_valE                                                                                        | %rax <- W_valE                                                                                                                                           |

* Details:
	* Cycle 6: 
		* `0x00a: irmovq $3,%rax`:  Writeback stage
			* %rax <- W_valE = 3
		* `0x017: addq %rdx,%rax`:  Fetch stage
			* $icode :ifun ← M_1[0x017]$
			* $\%rdx :\%rax ← M_1[0x018]$
			* $valP ← 0x019$
		* In this cycle: %rdx = 10, %rax will be set to 3 at the start of cycle 7 as the clock rises.
	* Cycle 7:
		* `0x00a: irmovq $3,%rax`:  Finish writeback stage
		* `0x017: addq %rdx,%rax`:  Decode stage
			* $E\_valA ← R[\%rdx]$:
			 - d_srcA = %rdx
			 - E_valA <- d_rvalA
			* $E\_valB ← R[\%rax]$:
			 - d_srcB = %rax
			 - E_valB <- d_rvalB
			* E_dstE <- %rax
		* In this cycle: %rdx = 10, %rax = 3
* As the `addq` instruction passes through the decode stage during cycle 7, it will therefore read the correct values for its source operands. 
* The data dependencies between the 2 `irmovq` instructions and the `addq` instruction have not created data hazards in this example.

### Example 2 - 2 nops - data hazard: 1 operand wrong
![[Pasted image 20240926155610.png|500]]

| Stage     | 0x000: irmovq $10,%rdx<br>irmovq V,rB<br>V = $10<br>rB = %rdx                                          | 0x00a: irmovq $3,%rax<br>irmovq V,rB<br>V  = $3<br>rB = %rax                                          | 0x016: addq %rdx,%rax<br>OPq rA,rB<br>rA = %rdx<br>rB = %rax                                                                                             |
| --------- | ------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=10$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=3$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br><br>$valP ← PC + 2$                                                                                 |
| Decode    | $E\_valC ← D\_valC=10$<br>$E\_dstE←D\_dstE=\%rdx$                                                      | $E\_valC ← D\_valC$<br>$E\_dstE←D\_dstE=\%rax$                                                        | $E\_valA ← R[\%rdx]$:<br> - d_srcA = %rdx<br> - E_valA <- d_rvalA<br>$E\_valB ← R[\%rax]$:<br> - d_srcB = %rax<br> - E_valB <- d_rvalB<br>E_dstE <- %rax |
| Execute   | $M\_valE ← 0 + E\_valC~=10$<br>$M\_dstE←E\_dstE=\%rdx$                                                 | $M\_valE ← 0 + E\_valC ~= 3$<br>$M\_dstE←E\_dstE=\%rax$                                               | M_valE <- E_valB + E_valA<br>M_dstE <- E_dstE = %rax<br>$Set~CC$                                                                                         |
| Memory    | W_valE <- M_valE = 10<br>W_dstE <- M_dstE = %rdx                                                       | W_valE <- M_valE = 3<br>W_dstE <- M_dstE                                                              | W_valE <- M_valE<br>W_dstE <- M_dstE = %rax                                                                                                              |
| Writeback | %rdx <- W_valE = 10                                                                                    | %rax <- W_valE = 3                                                                                    | %rax <- W_valE                                                                                                                                           |

* Details:
	* Cycle 6: 
		* `0x000: irmovq $10,%rdx`: finish writeback stage
		* `0x00a: irmovq $3,%rax`:  Writeback stage
			* %rax <- W_valE = 3 - ==occurs at the start of cycle 7 as the clock rises==.
		* `0x016: addq %rdx,%rax`:  Decode stage
			* $E\_valA ← R[\%rdx]$:
			 - d_srcA = %rdx
			 - E_valA <- d_rvalA
			* $E\_valB ← R[\%rax]$: - ==wrong, as %rax haven't been set to 3 yet==.
			 - d_srcB = %rax
			 - E_valB <- d_rvalB
			* E_dstE <- %rax
		* In this cycle: %rdx = 10, %rax = 3
* The second `irmovq` instruction is in the writeback stage during this cycle, and so the write to program register `%rax` only occurs at the start of cycle 7 as the clock rises. 
* As a result, the incorrect value zero would be read for register `%rax` (recall that we assume all registers are initially zero), since the pending write for this register has not yet occurred. 

### Example 3 - 1 nop - data hazard - 2 operands wrong
![[Pasted image 20240926163921.png|500]]

| Stage     | 0x000: irmovq $10,%rdx<br>irmovq V,rB<br>V = $10<br>rB = %rdx                                          | 0x00a: irmovq $3,%rax<br>irmovq V,rB<br>V  = $3<br>rB = %rax                                          | 0x016: addq %rdx,%rax<br>OPq rA,rB<br>rA = %rdx<br>rB = %rax                                                                                             |
| --------- | ------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=10$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=3$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br><br>$valP ← PC + 2$                                                                                 |
| Decode    | $E\_valC ← D\_valC=10$<br>$E\_dstE←D\_dstE=\%rdx$                                                      | $E\_valC ← D\_valC$<br>$E\_dstE←D\_dstE=\%rax$                                                        | $E\_valA ← R[\%rdx]$:<br> - d_srcA = %rdx<br> - E_valA <- d_rvalA<br>$E\_valB ← R[\%rax]$:<br> - d_srcB = %rax<br> - E_valB <- d_rvalB<br>E_dstE <- %rax |
| Execute   | $M\_valE ← 0 + E\_valC~=10$<br>$M\_dstE←E\_dstE=\%rdx$                                                 | $M\_valE ← 0 + E\_valC ~= 3$<br>$M\_dstE←E\_dstE=\%rax$                                               | M_valE <- E_valB + E_valA<br>M_dstE <- E_dstE = %rax<br>$Set~CC$                                                                                         |
| Memory    | W_valE <- M_valE = 10<br>W_dstE <- M_dstE = %rdx                                                       | W_valE <- M_valE = 3<br>W_dstE <- M_dstE                                                              | W_valE <- M_valE<br>W_dstE <- M_dstE = %rax                                                                                                              |
| Writeback | %rdx <- W_valE = 10                                                                                    | %rax <- W_valE = 3                                                                                    | %rax <- W_valE                                                                                                                                           |

* Details:
	* Cycle 5: 
		* `0x000: irmovq $10,%rdx`: Writeback stage
			* %rdx <- W_valE = 10 - ==occurs at the start of cycle 6 as the clock rises==
		* `0x00a: irmovq $3,%rax`:  Memory stage
			* W_valE <- M_valE = 3
			* W_dstE <- M_dstE = %rax
		* `0x017: addq %rdx,%rax`:  Decode stage
			* $valA ← R[\%rdx]~=~???$ - error
			* $valB ← R[\%rax]~=~???$ - error
		* In this cycle: %rdx = ???, %rax = ???


### Example 4 - no nop - data hazard - 2 operands wrong
![[Pasted image 20240926165348.png|500]]

| Stage     | 0x000: irmovq $10,%rdx<br>irmovq V,rB<br>V = $10<br>rB = %rdx                                          | 0x00a: irmovq $3,%rax<br>irmovq V,rB<br>V  = $3<br>rB = %rax                                          | 0x014: addq %rdx,%rax<br>OPq rA,rB<br>rA = %rdx<br>rB = %rax                                                                                             |
| --------- | ------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=10$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$D\_valC ← M_8[PC + 2]=3$<br>$D\_valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br><br>$valP ← PC + 2$                                                                                 |
| Decode    | $E\_valC ← D\_valC=10$<br>$E\_dstE←D\_dstE=\%rdx$                                                      | $E\_valC ← D\_valC$<br>$E\_dstE←D\_dstE=\%rax$                                                        | $E\_valA ← R[\%rdx]$:<br> - d_srcA = %rdx<br> - E_valA <- d_rvalA<br>$E\_valB ← R[\%rax]$:<br> - d_srcB = %rax<br> - E_valB <- d_rvalB<br>E_dstE <- %rax |
| Execute   | $M\_valE ← 0 + E\_valC~=10$<br>$M\_dstE←E\_dstE=\%rdx$                                                 | $M\_valE ← 0 + E\_valC ~= 3$<br>$M\_dstE←E\_dstE=\%rax$                                               | M_valE <- E_valB + E_valA<br>M_dstE <- E_dstE = %rax<br>$Set~CC$                                                                                         |
| Memory    | W_valE <- M_valE = 10<br>W_dstE <- M_dstE = %rdx                                                       | W_valE <- M_valE = 3<br>W_dstE <- M_dstE                                                              | W_valE <- M_valE<br>W_dstE <- M_dstE = %rax                                                                                                              |
| Writeback | %rdx <- W_valE = 10                                                                                    | %rax <- W_valE = 3                                                                                    | %rax <- W_valE                                                                                                                                           |
| PC update | $PC ← valP~=0x00a$                                                                                     | $PC ← valP~=0x014$                                                                                    | $PC ← valP~=0x016$                                                                                                                                       |
* Details:
	* Cycle 4: 
		* `0x000: irmovq $10,%rdx`: Memory stage
			* W_valE <- M_valE = 10
			* W_dstE <- M_dstE = %rdx
		* `0x00a: irmovq $3,%rax`:  Execute stage
			* $M\_valE ← 0 + E\_valC ~= 3$
			* $M\_dstE←E\_dstE=\%rax$
		* `0x014: addq %rdx,%rax`:  Decode stage
			* $valA ← R[\%rdx]~=~???$ - error
			* $valB ← R[\%rax]~=~???$ - error
		* In this cycle: %rdx = ???, %rax = ???
* The `addq` instruction would get the incorrect values for both operands.

### Summary
* These examples above illustrate that a data hazard can arise for an instruction when one of its operands is updated by any of the three preceding instructions.
* These data hazards occur because our pipelined processor reads the operands for an instruction from the register file in the **decode stage** but does not write the results for the instruction to the register file until 3 cycles later, after the instruction passes through the **write-back** stage.

## Avoiding Data Hazards by Stalling (暂停)
* One very general technique for avoiding hazards involves **stalling**, where the processor holds back one or more instructions in the pipeline until the hazard condition no longer holds.
	* Our processor can avoid data hazards by holding back an instruction in the **decode stage** until the instructions generating its source operands have passed through the **write-back** stage
### Example 1 - Pipelined execution of prog2 using stalls
![[Pasted image 20240927141354.png|500]]
* In cycle 6:
	* When the `addq` instruction is in the decode stage, the pipeline control logic detects that at least one of the instructions in the execute, memory, or write-back stage will update either register `%rdx` or register `%rax`.
		* The stall control logic detects a data hazard due to the pending write to register `%rax` in the write-back stage.
	* Rather than letting the `addq` instruction pass through the stage with the incorrect results, it stalls the instruction, holding it back in the decode stage for either one (for prog2) extra cycle.
* In cycle 7:
	* The stall logic control injects a **bubble** into the execute stage and repeats the decoding of the `addq` instruction.
	* A **bubble** is like a dynamically generated `nop` instruction — it does not cause any changes to the registers, the memory, the condition codes, or the program status.

### Example 2 - Pipelined execution of prog4 using stalls
![[Pasted image 20240927143343.png|500]]
* In cycle 4:
	* The **stall control logic** detects data hazards for both source registers.
* In cycle 5, 6, 7:
	* The **stall control logi**c injects a bubble into the execute stage and repeats the decoding of the `addq` instruction.
* In holding back the `addq` instruction in the decode stage, we must also hold back the `halt` instruction following it in the fetch stage. We can do this by keeping the **program counter** at a fixed value, so that the `halt` instruction will be fetched repeatedly until the stall has completed.
* Injecting one bubble for prog2 and three for prog4 has the same effect as having 3 `nop` instructions between the second irmovq instruction and the `addq` instruction.
* There are numerous cases in which one instruction updates a register and a closely following instruction uses the same register. This will cause the pipeline to stall for up to 3 cycles, reducing the overall throughput significantly.

## Avoiding Data Hazards by Forwarding (转发)
* Rather than stalling until the write has completed, it can simply pass the value that is about to be written to pipeline register E as the source operand.
* **Data forwarding** - passing a result value directly from one pipeline stage to an earlier one (or simply forwarding, and sometimes bypassing).
* Data forwarding requires adding additional data connections and control logic to the basic hardware structure.

### Example 1 - Pipelined execution of prog2 using forwarding
![[Pasted image 20240927154155.png|500]]

| Stage     | 0x000: irmovq $10,%rdx                                                                                       | 0x00a: irmovq $3,%rax                                                                                        | 0x016: addq %rdx,%rax                                                             |
| --------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[0x000]$<br>$rA :rB ← M_1[0x000 + 1]$<br>$valC ← M_8[0x000 + 2]$=10<br>$valP ← 0x000 + 10$ | $icode :ifun ← M_1[0x00a]$<br>$rA :rB ← M_1[0x00a + 1]$<br>$valC ← M_8[0x00a + 2]~=3$<br>$valP ← 0x00a + 10$ | $icode :ifun ← M_1[0x016]$<br>$rA :rB ← M_1[0x016 + 1]$<br><br>$valP ← 0x016 + 2$ |
| Decode    | -                                                                                                            | -                                                                                                            | $valA ← R[\%rdx]$<br>$valB ← R[\%rax]$                                            |
| Execute   | $valE ← 0 + valC~=10$                                                                                        | $valE ← 0 + valC~=3$                                                                                         | $valE ← valB~+~valA$                                                              |
| Memory    | -                                                                                                            | -                                                                                                            | -                                                                                 |
| Writeback | $R[\%rdx] ← valE~=10$                                                                                        | $R[\%rax] ← valE~=3$                                                                                         | $R[\%rax] ← valE$                                                                 |
| PC update | $PC ← valP~=0x00a$                                                                                           | $PC ← valP~=0x014$                                                                                           | $PC ← valP~=0x018$                                                                |
* In cycle 6:
	* `0x000: irmovq $10,%rdx`: Finish writeback
	* `0x00a: irmovq $3,%rax`: Writeback stage
		![[Pasted image 20240928225327.png|400]]
		* about to execute:
			* $R[\%rax] ← valE~=3$
				* W_dstE = %rax
				* W_valE = 3
	* `0x016: addq %rdx,%rax`: decode stage
		![[Pasted image 20240928223124.png|400]]
		* about to execute:
			* $valA ← R[\%rdx]~=10$
				* srcA = %rdx
			* $valB ← R[\%rax]~=3$
				* srcB = %rax
				* $valB ← W\_valE~=3$
	* The decode-stage logic detects that register `%rax` is the source register for operand `valB`.
	* There is also a pending write to `%rax` on write port E. 
	* It can therefore avoid stalling by simply using the data word supplied to port E (signal `W_valE`) as the value for operand `valB`.


### Example 2 - Pipelined execution of prog3 using forwarding
![[Pasted image 20240927170344.png|500]]

| Stage     | 0x000: irmovq $10,%rdx                                                                                       | 0x00a: irmovq $3,%rax                                                                                        | 0x016: addq %rdx,%rax                                                             |
| --------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[0x000]$<br>$rA :rB ← M_1[0x000 + 1]$<br>$valC ← M_8[0x000 + 2]$=10<br>$valP ← 0x000 + 10$ | $icode :ifun ← M_1[0x00a]$<br>$rA :rB ← M_1[0x00a + 1]$<br>$valC ← M_8[0x00a + 2]~=3$<br>$valP ← 0x00a + 10$ | $icode :ifun ← M_1[0x016]$<br>$rA :rB ← M_1[0x016 + 1]$<br><br>$valP ← 0x016 + 2$ |
| Decode    | -                                                                                                            | -                                                                                                            | $valA ← R[\%rdx]$<br>$valB ← R[\%rax]$                                            |
| Execute   | $valE ← 0 + valC~=10$                                                                                        | $valE ← 0 + valC~=3$                                                                                         | $valE ← valB~+~valA$                                                              |
| Memory    | -                                                                                                            | -                                                                                                            | -                                                                                 |
| Writeback | $R[\%rdx] ← valE~=10$                                                                                        | $R[\%rax] ← valE~=3$                                                                                         | $R[\%rax] ← valE$                                                                 |
| PC update | $PC ← valP~=0x00a$                                                                                           | $PC ← valP~=0x014$                                                                                           | $PC ← valP~=0x018$                                                                |
* In cycle 5:
	* `0x000: irmovq $10,%rdx`: Writeback stage
		![[Pasted image 20240928225327.png|400]]
		* about to execute:
			* $R[\%rdx] ← valE~=10$
				* W_desE = %rdx
				* W_valE = 10
	* `0x00a: irmovq $3,%rax`: Memory stage
		![[Pasted image 20240928225548.png|400]]
		* about to execute:
			* M_desE = %rax
			* M_valE = 3
	* `0x016: addq %rdx,%rax`: decode stage
		![[Pasted image 20240928223124.png|400]]
		* about to execute:
			* $icode :ifun ← M_1[0x016]$
			* $rA :rB ← M_1[0x016 + 1]$
			* $valP ← 0x016 + 2$
			* $valA ← R[\%rdx]~=10$
				* In D register:
					* srcA = %rdx
					* $valA ← W_valE = 10$
			* $valB ← R[\%rax]~=3$
				* In D register:
					* srcB = %rax
					* $valB ← M\_valE = 3$
* In cycle 5, the decodestage logic detects a pending write to register `%rdx` in the write-back stage and to register `%rax` in the memory stage. 
* Rather than stalling until the writes have occurred, it can use the value in the write-back stage (signal `W_valE`) for operand `valA` and the value in the memory stage (signal `M_valE`) for operand `valB`.
* Data forwarding can also be used when there is a pending write to a register in the memory stage, avoiding the need to stall for program prog3.

### Example 3 - Pipelined execution of prog4 using forwarding
![[Pasted image 20240928231833.png|500]]

| Stage     | 0x000: irmovq $10,%rdx                                                                                       | 0x00a: irmovq $3,%rax                                                                                        | 0x016: addq %rdx,%rax                                                             |
| --------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------- |
| Fetch     | $icode :ifun ← M_1[0x000]$<br>$rA :rB ← M_1[0x000 + 1]$<br>$valC ← M_8[0x000 + 2]$=10<br>$valP ← 0x000 + 10$ | $icode :ifun ← M_1[0x00a]$<br>$rA :rB ← M_1[0x00a + 1]$<br>$valC ← M_8[0x00a + 2]~=3$<br>$valP ← 0x00a + 10$ | $icode :ifun ← M_1[0x016]$<br>$rA :rB ← M_1[0x016 + 1]$<br><br>$valP ← 0x016 + 2$ |
| Decode    | -                                                                                                            | -                                                                                                            | $srcA←\%rdx$<br>$valA ← R[\%rdx]$<br>$srcB←\%rax$<br>$valB ← R[\%rax]$            |
| Execute   | $valE ← 0 + valC~=10$                                                                                        | $valE ← 0 + valC~=3$                                                                                         | $valE ← valB~+~valA$                                                              |
| Memory    | -                                                                                                            | -                                                                                                            | -                                                                                 |
| Writeback | $R[\%rdx] ← valE~=10$                                                                                        | $R[\%rax] ← valE~=3$                                                                                         | $R[\%rax] ← valE$                                                                 |
| PC update | $PC ← valP~=0x00a$                                                                                           | $PC ← valP~=0x014$                                                                                           | $PC ← valP~=0x018$                                                                |

* In cycle 4:
	* `0x000: irmovq $10,%rdx`: Memory stage
		![[Pasted image 20240928225548.png|400]]
		* about to execute:
			* W_desE = %rdx
			* W_valE = 10
	* `0x00a: irmovq $3,%rax`: Execute stage
		![[Pasted image 20240929084824.png|400]]
		* about to execute:
			* $valE ← 0 + valC~=3$
				* M_desE = %rax
			* E_valE = 3
	* `0x014: addq %rdx,%rax`: decode stage
		![[Pasted image 20240928223124.png|400]]
		* about to execute:
			* $icode :ifun ← M_1[0x016]$
			* $rA :rB ← M_1[0x016 + 1]$
			* $valP ← 0x014 + 2$
			* $valA ← R[\%rdx]~=10$
				* In D register:
					* srcA = %rdx
					* $valA ← M\_valE = 10$
			* $valB ← R[\%rax]~=3$
				* In D register:
					* srcB = %rax
					* $valB ← E\_valE = 3$
* In cycle 4, the decode stage logic detects a pending write to register `%rdx` in the memory stage. It also detects that a new value is being computed for register `%rax` in the execute stage. It uses these as the values for `valA` and `valB` rather than the values read from the register file.
* To exploit data forwarding to its full extent, we can also pass newly computed values from the execute stage to the decode stage, avoiding the need to stall for program prog4.
* To exploit data forwarding to its full extent, we can also pass newly computed values from the execute stage to the decode stage, avoiding the need to stall for program prog4.

### Summary
* Types of forwarding sources and destinations:
	* sources: e_valE, m_valM, M_valE, W_valM, and W_valE
	* destinations: valA and valB
* How the decode stage logic can determine whether to use a value from the register file or to use a forwarded value.
	* Associated with every value that will be written back to the register file is the **destination register ID**. 
	* The logic can compare these IDs with the **source register** IDs `srcA` and `srcB` to detect a case for forwarding. 
	* It is possible to have multiple destination register IDs match one of the source IDs. 
	* We must establish a priority among the different forwarding sources to handle such cases.
* Extention PIPE- structure to handle forwarding:
	![[Pasted image 20240929102830.png|450]]
	* The values from the 5 forwarding sources are fed back to the two blocks labeled “**Sel+Fwd A**” and “**Fwd B**” in the decode stage.
	* The block labeled “Sel+Fwd A” combines the role of the block labeled “Select A” in PIPE− with the forwarding logic.
		* It allows valA for pipeline register E to be either the incremented program counter `valP`, the value read from the A port of the register file, or one of the forwarded values.
	* The block labeled “Fwd B” implements the forwarding logic for source operand `valB`.

## Load/Use Data Hazards
* One class of data hazards cannot be handled purely by forwarding, because memory reads occur late in the pipeline.
![[Pasted image 20240930093130.png|500]]

| Stage     | irmovq V,rB<br>V  = 10<br>rB = %rbx                                                           | mrmovq D(rB), rA<br>rB - %rdx<br>rA - %rax                                                    | addq %rdx,%rax                                                           |
| --------- | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| Fetch     | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$valC ← M_8[PC + 2]$<br>$valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br>$valC ← M_8[PC + 2]$<br>$valP ← PC + 10$ | $icode :ifun ← M_1[PC]$<br>$rA :rB ← M_1[PC + 1]$<br><br>$valP ← PC + 2$ |
| Decode    | -                                                                                             | $srcB←\%rdx$<br>$valB ← R[\%rdx]$<br>                                                         | $srcA←\%rdx$<br>$valA ← R[\%rdx]$<br>$srcB←\%rax$<br>$valB ← R[\%rax]$   |
| Execute   | $valE ← 0 + valC$                                                                             | $valE ← valB + valC$                                                                          | $valE ← valB~+~valA$                                                     |
| Memory    | -                                                                                             | $valM ← M_8[valE]$                                                                            | -                                                                        |
| Writeback | $R[rB] ← valE$                                                                                | $R[rA] ← valM$                                                                                | $R[\%rax] ← valE$                                                        |


* In cycle 7:
	* `0x01e: irmovq $10,%rbx`: Memory Stage
		![[Pasted image 20240928225548.png|400]]
		* about to execute:
			* 
	* `0x028: mrmovq 0(%rdx),%rax`: Execute stage
		![[Pasted image 20240929084824.png|400]]
		* about to execute:
			* $M\_valE ← E\_valB + E\_valC$
			* $M\_desE = \%rax$































