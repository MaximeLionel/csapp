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

## Example - Word-level multiplexor circuit:
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
## Example - Four-way multiplexor
![[Pasted image 20240814083746.png|200]]
* The selection expressions can be arbitrary Boolean expressions, and there can be an **arbitrary number of cases**. This allows case expressions to describe blocks where there are many choices of input signals with complex selection criteria.
* The circuit above selects from among the four input words A, B, C, and D based on the control signals `s1` and `s0`, treating the controls as a 2-bit binary number.
* Express the circuit in HCL using Boolean expressions:
	```C
	word Out4 = [
		!s1 && !s0 : A; # 00
		!s1 : B;        # 01
		!s0 : C;        # 10
		1 : D;          # 11
	];
	```
	* The comments on the right (any text starting with # and running for the rest of the line is a comment) show which combination of `s1` and `s0` will cause the case to be selected.
	* The second expression can be written `!s1`, rather than the more complete `!s1&&s0`.
	* The third expression can be written as `!s0`, rather than the more complete `s1&&!s0`.
	* The fourth expression can be written as `1`, rather than the more complete `s1&&s0`.

## Example - find the minimum value
![[Pasted image 20240814085626.png|200]]
* Expression in HCL:
	```c
	word Min3 = [
		A <= B && A <= C : A;
		B <= A && B <= C : B;
		1 : C;
	];
	```


## Arithmetic/logic unit (ALU)
![[Pasted image 20240814093605.png|500]]
* The circuit has three inputs: 2 data inputs labeled A and B and 1 control input. 
* Depending on the setting of the control input, the circuit will perform different arithmetic or logical operations on the data inputs.
* Observe that the four operations diagrammed for this ALU correspond to the four different integer operations supported by the Y86-64 instruction set, and the control values match the function codes for these instructions.
	![[Pasted image 20240814093823.png|90]]
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

# Practice Problem 4.11
The HCL code given for computing the minimum of 3 words contains 4 comparison expressions of the form X <= Y . Rewrite the code to compute the
same result, but using only three comparisons.

**Solution**:
The original code:
```c
	word Min3 = [
		A <= B && A <= C : A; # 1st expression
		B <= A && B <= C : B; # 2nd expression
		1 : C;                # 3rd expression
	];
```

The logic is like this:
Suppose we implement the 1st expression and it fails, it means that A is not the minimum of the 3 words. Logically, the minimum number is among B and C. Then we go to the 2nd expression, to decide the minimum is B or C, we only need to compare B and C.

Therefore, the rewrited code:
```C
word Min3 = [
	A <= B && A <= C : A; # 1st expression
	B <= C : B;           # 2nd expression - only need to compare B and C
	1 : C;                # 3rd expression
];
```

# Practice Problem 4.12
Write HCL code describing a circuit that for word inputs A, B, and C selects the median of the three values. That is, the output equals the word lying between the minimum and maximum of the three inputs.

**Solution**:
```C
word Min3 = [
	A <= B && B <= C : B; 
	C <= B && B <= A : B; 
	B <= C && C <= A : C; 
	A <= C && C <= B : C;
	1 : A;                
];
```


# 4.2.4 Set Membership (集合关系)
![[Pasted image 20240814100533.png|400]]
* In this circuit, the 2-bit signal code would then control the selection among the 4 data words A, B, C, and D. 
* We can express the generation of signals `s1` and `s0` using equality tests based on the possible values of code:
	```c
	bool s1 = code == 2 || code == 3;
	bool s0 = code == 1 || code == 3;
	```
* A more concise expression can be written that expresses the property that `s1` is 1 when code is in the set {2, 3}, and `s0` is 1 when code is in the set {1, 3}:
	```c
	bool s1 = code in { 2, 3 };
	bool s0 = code in { 1, 3 };
	```
* The general form of a set membership test is 
	```c
	iexpr in {iexpr1, iexpr2, ... , iexprk}
	```
	* The tested (`iexpr`) and the candidate matches ($iexpr_1$ through $iexpr_k$) are all integer expressions.

# 4.2.5 Memory and Clocking(时钟控制)
* Combinational circuits **do not store any information**. Instead, they simply react to the signals at their inputs, generating outputs equal to some function of the inputs.
* To create **sequential circuits**—that is, systems that have state and perform computations on that state—we must introduce devices that store information represented as bits. 
* Our storage devices are all controlled by a **single clock**, a periodic signal that determines when new values are to be loaded into the devices. We consider two classes of memory devices:
	* ==Clocked registers== (or simply registers, or hardware registers) - store individual bits or words. The clock signal controls the loading of the register with the value at its input.
	* ==Random access memories== (or simply memories) - store multiple words, using an address to select which word should be read or written.
		* Examples: (1) the virtual memory system; (2) the register file (or Program Registers), where register identifiers serve as the addresses. 
		* In a Y86-64 processor, the register file holds the 15 program registers (%rax through %r14).

## Hardware Register
![[Pasted image 20240814104017.png|500]]
* In hardware, a register is directly connected to the rest of the circuit by its input and output wires.
* How it operates:
	* For most of the time, the register remains in a fixed state (State = x), generating an output equal to its current state. 
	* Signals propagate through the combinational logic preceding the register, creating a new value for the register input (Input = y), but the register output remains fixed as long as the clock is low.
	* As the clock rises, the input signals are loaded into the register as its next state (State = y), and this becomes the new register output until the next rising clock edge. 
* A key point is that the registers serve as barriers between the combinational logic in different parts of the circuit. 
* Values only propagate from a register input to its output once every clock cycle at the rising clock edge. 
* Our Y86-64 processors will use clocked registers to hold **the program counter (PC), the condition codes (CC), and the program status (Stat)**.

## Program Register
![[Pasted image 20240814105039.png|400]]
* In machine-level programming, the registers represent a small collection of addressable words in the CPU, where the addresses consist of **register IDs**.
* In the register file diagrammed, the circuit can read the values of two program registers and update the state of a third. 
	* Each port has an **address input**, indicating which program register should be selected.
	* Each port has a **data output or input** giving a value for that program register. 
	* The addresses are register identifiers, using the encoding shown below:
		![[Pasted image 20240814110104.png|300]]
* This register file has two read ports, named A and B, and one write port, named W. 
	* The 2 read ports have address inputs srcA and srcB (short for “source A” and “source B”) and data outputs valA and valB (short for “value A” and “value B”). 
	* The write port has address input dstW (short for “destination W”) and data input valW (short for “value W”).
* Such a multi-ported random access memory allows multiple read and write operations to take place simultaneously. 
* The register file is not a combinational circuit, since it has **internal storage**. 
	* In our implementation, data can be read from the register file as if it were a block of combinational logic having addresses as inputs and the data as outputs.
	* When either srcA or srcB is set to some register ID, then, after some delay, the value stored in the corresponding program register will appear on either valA or valB. 
		* For example, setting `srcA` to 3 will cause the value of program register `%rbx` to be read, and this value will appear on output `valA`.














