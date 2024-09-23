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
* Potential serious errors on PIPE-:
	* Our sequential implementations SEQ and SEQ+ only process one instruction at a time, and so there are unique values for signals such as valC, srcA, and valE. 
	* In our pipelined design, there will be multiple versions of these values associated with the different instructions flowing through the system.
	* For example, in the detailed structure of PIPE−, there are 4 white boxes labeled “Stat” that hold the status codes for four different instructions.
	* We need to take great care to make sure we use the proper version of a signal, or else we could have serious errors, such as storing the result computed for one instruction at the destination register specified by another instruction.























































