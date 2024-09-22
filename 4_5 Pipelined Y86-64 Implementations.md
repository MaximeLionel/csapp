* We are finally ready for the major task of this chapter — designing a pipelined Y86-64 processor.
* Steps on modification:
	* Shift the computation of the PC into the fetch stage.
	* Then add pipeline registers between the stages.

# 4.5.1 SEQ+: Rearranging the Computation Stages
*  Rearrange the order of the five stages in SEQ so that the PC update stage comes at the beginning of the clock cycle, rather than at the end.
