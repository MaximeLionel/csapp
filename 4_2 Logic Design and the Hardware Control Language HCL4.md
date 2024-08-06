* In hardware design, electronic circuits are used to **compute functions on bits** and to **store bits** in different kinds of memory elements. 
* Most contemporary circuit technology represents different bit values as high or low voltages on signal wires.
	* Logic value 1 is represented by a high voltage of around 1.0 volt.
	* Logic value 0 is represented by a low voltage of around 0.0 volts. 
* 3 major components are required to implement a digital system: **combinational logic to ==compute== functions on the bits, memory elements to ==store== bits, and ==clock signals== to regulate the updating of the memory elements**.
* HCL (hardware control language) - the language that we use to describe the control logic of the different processor designs.

# 4.2.1 Logic Gates
![[Pasted image 20240806090557.png|300]]
* Logic gates are the basic computing elements for digital circuits.
* Each gate generates output equal to some Boolean function of its inputs.
* HCL expressions are shown below the gates for the operators in C: && for and, || for or, and ! for not.
	* We use these instead of the bit-level C operators &, |, and ~, because logic gates operate on single-bit quantities, not entire words.
* n-way operations for n > 2:
	* Example: 3 input (a,b and c) and gate: a&&b&&c.

# 4.2.2 Combinational Circuits and HCL Boolean Expressions
* combinational circuits (组合电路) - by assembling a number of logic gates into a network, we can construct computational blocks known as combinational circuits.
* Some restrictions:
	* Every logic gate input must be connected to exactly one of the following:
		* one of the system inputs (known as a **primary input**). 
		* OR the output connection of some memory element. 
		* OR the output of some logic gate.
	* The outputs of 2 or more logic gates cannot be connected together.
	* The network must be ==acyclic== (非循环的).

## Example - a simple combinational circuit
![[Pasted image 20240806111841.png|280]]
* Combinational circuit to test for bit equality.
*  It has two inputs, a and b. It generates a single output eq, such that the output will equal 1 if either a and b are both 1.
* Write in HCL:
	```c
	bool eq = (a && b) || (!a && !b);
	```
* HCL uses C-style syntax, with ‘=’ associating a signal name with an expression.

# Practice Problem 4.9
Write an HCL expression for a signal `xor`, equal to the exclusive-or of inputs `a` and `b`. What is the relation between the signals `xor` and `eq` defined above?

**Solution**:
```c
bool eq = (a && !b) || (!a && b)
```


























