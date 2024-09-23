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


























































