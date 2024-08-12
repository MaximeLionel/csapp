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

## Combinational Circuits
* combinational circuits (组合电路) - by assembling a number of logic gates into a network, we can construct computational blocks known as combinational circuits.
* Some restrictions:
	* Every logic gate input must be connected to exactly one of the following:
		* one of the system inputs (known as a **primary input**). 
		* OR the output connection of some memory element. 
		* OR the output of some logic gate.
	* The outputs of 2 or more logic gates cannot be connected together.
	* The network must be ==acyclic== (非循环的).

### Example - a simple combinational circuit
![[Pasted image 20240806111841.png|280]]
* Combinational circuit to test for bit equality.
*  It has two inputs, a and b. It generates a single output eq, such that the output will equal 1 if either a and b are both 1.
* Write in HCL:
	```c
	bool eq = (a && b) || (!a && !b);
	```
* HCL uses C-style syntax, with ‘=’ associating a signal name with an expression.

## Multiplexor
* A multiplexor (MUX) **selects a value** from among **a set of different data signals**, depending on the value of **a control input signal**.
### Single-bit multiplexor
* Example of single-bit multiplexor circuit:
	![[Pasted image 20240806222140.png|300]]
	* The two data signals are the input bits a and b
	* The control signal is the input bit s. 
	* The output will equal a when s is 1, and it will equal b when s is 0.
		* The upper `AND` gate passes signal b when s is 0.
		* The lower `AND` gate passes signal a when s is 1.
* HCL expression：
	```c
	bool out = (s && a) || (!s && b);
	```

## Differences between combinational logic circuits and logical expressions in C
* In a combinational circuit, the outputs continually respond to changes in the inputs, while a C expression is only evaluated when it is encountered during the execution of a program.
* In a combinational circuit, the logic gates only operate over the bit values 0 and 1, while logical expressions in C allow arguments to be arbitrary integers, interpreting 0 as false and anything else as true. 
* In a combinational circuit, it does not have any partial evaluation rules. The gates simply respond to changing inputs. However, logical expressions in C have the property that they might only be partially evaluated.
	* Example: `(a && !a) && func(b,c)`
	* the function func will not be called, because the expression `(a && !a)` evaluates to 0.


# Practice Problem 4.9
Write an HCL expression for a signal `xor`, equal to the exclusive-or of inputs `a` and `b`. What is the relation between the signals `xor` and `eq` defined above?

**Solution**:
```c
bool eq = (a && !b) || (!a && b)
```


# 4.2.3 Word-Level Combinational Circuits and HCL Integer Expressions
* We design circuits that operate on **data words**, which are groups of bit-level signals that represent an integer or some control pattern.
* Combinational circuits that perform **word-level** computations are constructed using **logic gates** to compute the individual bits of the output word, based on the individual bits of the input words.
* Example - Word-level equality test circuit：
	![[Pasted image 20240810151327.png|400]]
	* The output will equal 1 when each bit from word A equals its counterpart from word B.
	* This circuit is implemented using 64 of the single-bit equality circuits shown below. 
		![[Pasted image 20240811095506.png|200]]
	* The outputs of these single-bit circuits are combined with an `AND` gate to form the circuit output.
	* The functionality of the circuit2 can be expressed at the word level as:
		`bool Eq = (A == B);`
		* Arguments A and B are of type int.
	* As is shown on the right side, we will draw word-level circuits using medium-thickness lines to represent the set of wires carrying the individual bits of the word, and we will show a single-bit signal as a dashed line.
* For simplicity, in HCL, we will declare any word-level signal as an `int`, without specifying the word size.
* In a **full-featured hardware description language**, every word can be declared to have a specific number of bits.

* Example - Word-level multiplexor circuit:
	![[Pasted image 20240811215715.png|400]]
	* This circuit generates a 64-bit word `Out` equal to one of the two input words, `A` or `B`, depending on the control input bit `s`.
	* The circuit consists of 64 identical sub-circuits, each having a structure similar to the bit-level multiplexor.
	* The output will equal input word A when the control signal s is 1, and it will equal B otherwise.
* We will use many forms of multiplexors in our processor designs. They allow us to **select a word from a number of sources** depending on some control condition.
* Multiplexing functions are described in HCL using ==case expressions==. A case expression has the following general form:
	```
	[
		select1 : expr1;
		select2 : expr2;
		.
		.
		.
		selectk : exprk;
	]
	```
	* The expression contains a series of cases, where each case i consists of a Boolean expression $select_i$, indicating when this case should be selected, and an integer expression $expr_i$, indicating the resulting value.
	* Logically, the selection expressions are evaluated in sequence, and the case for the first one yielding 1 is selected.
* Example:
	```
		word Out = [
			s: A;
			1: B;
		];
	```
	* The second selection expression is simply 1, indicating that this case should be selected if no prior one has been. This is the way to specify a default case in HCL.
* Allowing ==nonexclusive== selection expressions makes the HCL code more readable.
* To translate an HCL case expression into hardware, a logic synthesis program would need to analyze the set of selection expressions and resolve any possible conflicts by **making sure that only the first matching case would be selected**.
s













# Practice Problem 4.10
Suppose you want to implement a word-level equality circuit using the exclusive-or circuits from Problem 4.9 rather than from bit-level equality circuits. Design such a circuit for a 64-bit word consisting of 64 bit-level exclusive-or circuits and two additional logic gates.

**Solution**:
* The exclusive-or in C as below:
	```c
	bool eq = (a && !b) || (!a && b)
	```
* Single-bit `xor` circuit:
	![[Pasted image 20240811212742.png|250]]
* Word-level `xor` circuit:
	![[Pasted image 20240811214515.png|400]]



	















