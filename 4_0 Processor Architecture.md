* In this section, we will design our own ISA!
* Instruction set architecture (ISA) - The instructions supported by a particular processor and their byte-level encodings are known as its instruction set architecture (ISA).
* ISA provides a conceptual layer of abstraction between compiler writers, who need only know what instructions are permitted and how they are encoded, and processor designers, who must build machines that execute those instructions.
* In this chapter, we start by defining a simple instruction set that we call this the “Y86-64”.
* Some assumptions about Y86-64:
	* Y86-64 processor is based on sequential operation.
	* The processor executes a complete Y86-64 instruction on every clock cycle.
		* The clock must run slowly enough to allow an entire series of actions to complete within one cycle.
	* With the sequential design as a basis, we then apply a series of transformations to create a pipelined processor.
* Pipeline processor:
	* This processor breaks the execution of each instruction into five steps, each of which is handled by a separate section or stage of the hardware.
	* Instructions progress through the stages of the pipeline, with one instruction entering the pipeline on each clock cycle.
	* The processor can be executing the different steps of up to five instructions simultaneously.
	* Making this processor preserve the sequential behavior of the Y86-64 ISA requires handling a variety of hazard conditions, where the location or operands of one instruction depend on those of other instructions that are still in the pipeline.
* A variety of tools for studying and experimenting with our processor designs (Y86-64):
	* Assembler - an assembler for Y86-64.
	* Simulator - a simulator for running Y86-64 programs.
* The control logic for these designs is described by files in HCL (Hardware Control Logic) notation.
* Verilog hardware description language：
	* Verilog is a hardware description language (HDL) used to model and design digital systems.
	* Verilog allows engineers to describe the structure and behavior of electronic circuits in a textual format, which can then be used for simulation, verification, and synthesis of digital designs.



















