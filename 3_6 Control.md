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
setne %al

**Solution**:
A. 
int, a < b
B.
short, a >= b
C.
unsigned char, a <= b
D.
long or unsigned long, a != b
# Practice Problem 3.14
The C code
```c
int test(data_t a) {
	return a TEST 0;
}
```
shows a general comparison between argument a and 0, where we can set the data type of the argument by declaring data_t with a typedef, and the nature of the comparison by declaring TEST with a #define declaration. The following instruction sequences implement the comparison, where a is held in some portion of register %rdi. For each sequence, determine which data types data_t and which comparisons TEST could cause the compiler to generate this code. (There can be multiple correct answers; list all correct ones.)
A.
testq %rdi, %rdi
setge %al

B.
testw %di, %di
sete %al

C.
testb %dil, %dil
seta %al

D.
testl %edi, %edi
setle %al

**Solution**:
A.
long, a >= 0
B.
short or unsigned short, a == 0
C.
unsigned char, a > 0
D.
int, a <= 0

# 3.6.3 Jump Instructions
* A `jump` instruction can cause the execution to switch to a completely new position in the program.
	* `jump` destinations are generally indicated in assembly code by a label.
![[3_6 Control.assets/image-20240512095009257.png|500]]

```z80
	movq $0,%rax # Set %rax to 0
jmp .L1.         # Goto .L1
	movq (%rax),%rdx # Null pointer dereference (skipped)
.L1:
	popq %rdx    # Jump target
```
* Example:
	* The instruction `jmp` .L1 will cause the program to skip over the `movq` instruction and instead resume execution with the popq instruction.
	* In generating the object-code file, the assembler **determines the addresses of all labeled instructions** and **encodes the jump targets** (the addresses of the destination instructions) as part of the jump instructions.
* Unconditional jump:
	* direct jump - the jump target is encoded as part of the instruction.
	* indirect jump - the jump target is read from a register or a memory location.
		* Indirect jumps are written using `*` followed by an operand specifier using one of the memory operand formats.
		```z80
		jmp *%rax   # uses the value in register %rax as the jump target
		
		jmp *(%rax) # reads the jump target from memory, using the value in %rax as the read address
		```
* Conditional jumps can only be direct.

# 3.6.4 Jump Instruction Encodings
* In assembly code, jump targets are written using symbolic labels.
	* The assembler, and later the linker, generate the proper encodings of the jump targets.
* different encodings for jump:
	* PC relative - encode the difference between the address of the target instruction and the address of the instruction immediately following the jump.
	* absolute address - using 4 bytes to directly specify the target.
	* The assembler and linker select the appropriate encodings of the jump destinations.
* Example:
```z80
	movq %rdi, %rax
	jmp .L2
.L3:
	sarq %rax
.L2:
	testq %rax, %rax
jg .L3
	rep; ret
```

























