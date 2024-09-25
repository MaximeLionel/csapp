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
* Naming scheme:
	* We adopt a naming scheme where a signal stored in a pipeline register can be uniquely identified by **prefixing its name with that of the pipe register written in uppercase**.
		* For example, the 4 status codes are named `D_stat`, `E_stat`, `M_stat`, and `W_stat`.
	* Some signals that have just been computed within a stage, are labeled by prefixing the signal name with the first character of the stage name, written in lowercase.
		* Using the status codes as examples, we can see control logic blocks labeled “Stat” in the fetch and memory stages. The outputs of these blocks are therefore named `f_stat` and `m_stat`.
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





















































