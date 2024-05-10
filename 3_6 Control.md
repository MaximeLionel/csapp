* Machine code provides two basic low-level mechanisms for implementing conditional behavior: it tests data values and then alters either the ==control flow== or the ==data flow== based on the results of these tests.
* The execution order of a set of machine code instructions can be altered with a `jump` instruction, indicating that control should pass to some other part of the program, possibly contingent on the result of some test.

# 3.6.1 Condition Codes
* CPU maintains a set of single-bit condition code registers describing attributes of the most recent arithmetic or logical operation.
* These condition codes are the most useful:
	* CF: Carry flag. The most recent operation generated a carry out of the most significant bit. Used to ==detect overflow for unsigned operations==.
	* ZF: Zero flag. The most recent operation yielded zero.
	* SF: Sign flag. The most recent operation yielded a negative value.
	* OF: Overflow flag. The most recent operation caused ==a two’s-complement overflow—either negative or positive==.
* Example:
	* suppose: `t = a + b` using `add` instruction
	* CF (unsigned) t < (unsigned) a: Unsigned overflow
	* ZF (t == 0): Zero
	* SF (t < 0): Negative
	* OF (a < 0 == b < 0) && (t < 0 != a < 0): Signed overflow
* `leaq` instruction does not alter any condition codes, since it is intended to be used in address computations.
* all of the instructions listed below cause the condition codes to be set while altering registers at the same time:
	* xor - cf = 0, of = 0
	* For the shift operations, cf = the last bit shifted out, of = 0.
	![[3_6 Control.assets/image-20240510155445965.png|600]]
* 2 instruction classes (having 8-, 16-, 32-, and 64-bit forms) that set condition codes without altering any other registers:
	* `cmp` instructions set the condition codes according to the differences of their two operands:
		* behave in the same way as the sub instructions, except that they set the condition codes without updating their destinations.
	* `test` instructions behave in the same manner as the and instructions, except that they set the condition codes without altering their destinations.
	![[3_6 Control.assets/image-20240510160218283.png|500]]


# 3.6.2 Accessing the Condition Codes
* 3 common ways of using the condition codes:
	1. set a single byte to 0 or 1 depending on some combination of the condition codes 
		* `set` instruction
	2. conditionally jump to some other part of the program 
	3. conditionally transfer data
* The `set` instructions. Each instruction sets a single byte to 0 or 1 based on some combination of the condition codes.
	![[3_6 Control.assets/image-20240510161458720.png]]
* Example:
	* code
	```z80
	# int comp(data_t a, data_t b)
	# a in %rdi, b in %rsi
	comp:
		cmpq %rsi, %rdi  # Compare a:b
		setl %al         # Set low-order byte of %eax to 0 or 1
		movzbl %al, %eax # Clear rest of %eax (and rest of %rax)
		ret
	```
	* The comparison order of the `cmpq` instruction. Although the arguments are listed in the order %rsi (b), then %rdi (a), the comparison is really between a and b.
	* `movzbl` instruction clears not just the high-order 3 bytes of `%eax`, but the upper 4 bytes of the entire register, %rax, as well.
	* Condition 1 (signed a - b):
		* `sete` or `setz`: set when a == b
		* `setl`: set when a < b if sf^of
			* case1 (sf == 1, of == 0): $a-^t_wb<0$ 
			* case2 (sf == 0, of == 1): $a-^t_wb>0$ means a<0, b>0, negative overflow
		* `setge`: set when a >= b if ~(sf^of)
			* case1 (sf == 0, of == 0): $a-^t_wb>0$ 
			* case2 (sf == 1, of == 1): $a-^t_wb<0$ means a>0, b<0, positve overflow
		* `sete`: set when a == b if ~zf
	* Condition 2 (unsigned a - b):
		* `seta`: set when a > b if ~cf & ~zf
			* `~zf` is to exclude a special case: when a == b, cf = o and zf = 1
		* `setb`: set when a < b if cf
		* `sete` or `setz`: set when a == b

# Practice Problem 3.13
The C code
```c
int comp(data_t a, data_t b) {
	return a COMP b;
}
```
shows a general comparison between arguments a and b, where data_t, the data type of the arguments, is defined (via typedef) to be one of the integer data types listed below and either signed or unsigned. The comparison COMP is defined via #define.
![[3_6 Control.assets/image-20240510171846633.png|500]]

Suppose a is in some portion of %rdi while b is in some portion of %rsi. For each of the following instruction sequences, determine which data types data_t and which comparisons COMP could cause the compiler to generate this code. (There can be multiple correct answers; you should list them all.)
A.
cmpl %esi, %edi
setl %al

B.
cmpw %si, %di
setge %al

C.
cmpb %sil, %dil
setbe %al

D.
cmpq %rsi, %rdi
setne %a

**Solution**:
A. 














































