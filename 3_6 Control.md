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
	sarq %rax。         # right shift 1 bit arithmetically
.L2:
	testq %rax, %rax   # if rax&rax > 0, jump L3
jg .L3
	rep; ret # why?
```

```
rep; ret
recommend using the combination of rep followed by ret to avoid making the ret instruction the destination of a conditional jump instruction.
Without the rep instruction, the jg instruction (line 7 of the assembly code) would proceed to the ret instruction when the branch is not taken. 
According to AMD, their processors cannot properly predict the destination of a ret instruction when it is reached from a jump instruction. 
The rep instruction serves as a form of no-operation here,
```

* Object file:
![[3_6 Control.assets/image-20240512222356179.png|500]]
* The target of the first jump instruction is encoded (in the second byte) as 0x03. 
* Adding this to 0x5, the address of the following instruction, we get jump target address 0x8.
* The target of the second jump instruction is encoded as 0xf8.
* Adding this to 0xd (decimal 13), the address of the instruction on line 6, we get 0x5, the address of the instruction on line 3.
* The value of the program counter when performing PC-relative addressing is the address of **the instruction following the jump**, not that of the jump itself.
	* the processor would update the program counter as its first step in executing an instruction.
* the disassembled version of the program after linking:
![[3_6 Control.assets/image-20240512222857761.png|500]]
* The instructions have been relocated to different addresses, but the encodings of the jump targets in lines 2 and 5 remain ==unchanged==.
# Practice Problem 3.15
In the following excerpts from a disassembled binary, some of the information has been replaced by X’s. Answer the following questions about these instructions.

A. What is the target of the `je` instruction below? (You do not need to know anything about the `callq` instruction here.)
```
	4003fa: 74 02   je XXXXXX
	4003fc: ff d0   callq *%rax
```
B. What is the target of the `je` instruction below?
```
	40042f: 74 f4   je XXXXXX
	400431: 5d      pop %rbp
```
C. What is the address of the ja and pop instructions?
```
	XXXXXX: 77 02   ja 400547
	XXXXXX: 5d      pop %rbp
```
D. In the code that follows, the jump target is encoded in PC-relative form as a 4-byte two’s-complement number. The bytes are listed from least significant to most, reflecting the little-endian byte ordering of x86-64. What is the address of the jump target?
```
	4005e8: e9 73 ff ff ff jmpq XXXXXXX
	4005ed: 90             nop
```

**Solution**:
A. `4003fc + 2 = 4003fe`
B. `400431 - c = 400425`
C. `2ndXXXXXX + 2 = 400547` -> `2ndXXXXXX = 400545`
	`1stXXXXXX = 400545 - 2 = 400543`
D. We extract the number: 0xffffff73 = -0x8d
	Then, 0x4005ed - 0x8d = 0x400560

# 3.6.5 Implementing Conditional Branches with Conditional Control
* The most general way to translate conditional expressions and statements from C into machine code is to use combinations of conditional and unconditional jumps.
* Example:
* Original C code:
```c
long lt_cnt = 0;
long ge_cnt = 0;

long absdiff_se(long x, long y)
{
	long result;
	if (x < y) {
		lt_cnt++;
		result = y - x;
	}
	else {
		ge_cnt++;
		result = x - y;
	}
	return result;
}
```
* Equivalent goto version:
![[3_6 Control.assets/image-20240513124106617.png|300]]
* Generated assembly code:
![[3_6 Control.assets/image-20240513124203080.png|400]]
* The control flow of the assembly code generated for `absdiff_se` closely follows the goto code of `gotodiff_se`.
* The general form of an if-else statement in C is given by the template:
```c
if (test-expr)
	then-statement
else
	else-statement
```
* C syntax to describe the control flow:
```
	t = test-expr;
	if (!t)
	goto false;
	then-statement
	goto done;
false:
	else-statement
done:
```

# Practice Problem 3.16
When given the C code:
```c
void cond(short a, short *p)
{
	if (a && *p < a)
		*p = a;
}
```
gcc generates the following assembly code:
![[3_6 Control.assets/image-20240513130005518.png|250]]
A. Write a goto version in C that performs the same computation and mimics the control flow of the assembly code, in the style shown in Figure 3.16(b). You might find it helpful to first annotate the assembly code as we have done in our examples.

B. Explain why the assembly code contains two conditional branches, even though the C code has only one if statement.

**Solution**:
A.
```c
void cond(short a, short *p)
{
	if (a && *p < a)
		*p = a;
}
```

```
void cond(short a, short *p){
	t = test-expr;
	if (!a)
		goto done;
	if (*p >= a)
		goto done;
	*p = a;
done:
	return;
}
```

B.
It exactly follows the executing sequence, which examine a firstly and test (\*p >= a) secondly.

# Practice Problem 3.17
An alternate rule for translating if statements into goto code is as follows:
```
	t = test-expr;
	if (t)
		goto true;
	else-statement
		goto done;
true:
	then-statement
done:
```
A. Rewrite the goto version of absdiff_se based on this alternate rule.

B. Can you think of any reasons for choosing one rule over the other?

**Solution**:
A. 
```c
long lt_cnt = 0;
long ge_cnt = 0;

long absdiff_se(long x, long y)
{
	long result;
	if (x < y) {
		lt_cnt++;
		result = y - x;
	}
	else {
		ge_cnt++;
		result = x - y;
	}
	return result;
}
```

```
long absdiff_se(long x, long y)
{
	long result;
	if (x < y)
		goto true;
	ge_cnt++;
	result = x - y;
	return result;
true:
	lt_cnt++;
	result = y - x;
	return result;
}
```

B. Same

# Practice Problem 3.18
Starting with C code of the form:
```c
short test(short x, short y, short z) {
	short val = ______ ;
	if (______) {
		if (______)
			val = ______ ;
		else
			val = ______ ;
	} else if ( ______ )
		val = ______ ;
	return val;
}
```
gcc generates the following assembly code:
```z80
# short test(short x, short y, short z)
# x in %rdi, y in %rsi, z in %rdx

test:
	leaq (%rdx,%rsi), %rax
	subq %rdi, %rax
	cmpq $5, %rdx
jle .L2
	cmpq $2, %rsi
jle .L3
	movq %rdi, %rax
	idivq %rdx, %rax
	ret
.L3:
	movq %rdi, %rax
	idivq %rsi, %rax
	ret
.L2:
	cmpq $3, %rdx
jge .L4
	movq %rdx, %rax
	idivq %rsi, %rax
.L4:
	rep; ret
```
* Actually, the assembly code is wrong, because `idivq` instruction only has 1 operand in law. 
* Therefore, I re-compile and get the code below. We will process the practice problem with the code below as well.
```z80
# short test(short x, short y, short z)
# x in %rdi, y in %rsi, z in %rdx

.globl  test
test:
        leal    (%rdx,%rsi), %eax
        subl    %edi, %eax
        cmpw    $5, %dx
        jle     .L2
        cmpw    $2, %si
        jle     .L3
        movswl  %di, %eax
        movswl  %dx, %ecx
        cltd
        idivl   %ecx
        ret
.L3:
        movswl  %di, %eax
        movswl  %si, %esi
        cltd
        idivl   %esi
        ret
.L2:
        cmpw    $2, %dx
        jg      .L1
        movswl  %dx, %eax
        movswl  %si, %esi
        cltd
        idivl   %esi
.L1:
        ret
```

**Solution**:
```z80
# short test(short x, short y, short z)
# x in %rdi, y in %rsi, z in %rdx

.globl  test
test:
        leal    (%rdx,%rsi), %eax  # eax = rdx(z) + rsi(y) = z + y
        subl    %edi, %eax         # eax = z + y - x
        cmpw    $5, %dx            # compare 5 and dx(z)
        jle     .L2                # if(signed) dx(z) <= 5, go to .L2
		
		                           # if(signed) dx(z) > 5,
        cmpw    $2, %si            # compare 2 and si(y)
        jle     .L3                # if(signed) si(y) <= 2, go to .L3

		                           # if(signed) si(y) > 2
        movswl  %di, %eax          # eax = di(x)
        movswl  %dx, %ecx          # ecx = dx(z)
        cltd                       # Sign-extend EAX(x) into EDX:EAX
        idivl   %ecx               # EDX:EAX / ecx: x/z
		                           # eax - quotient
        ret
.L3:
        movswl  %di, %eax          # eax = di(x)
        movswl  %si, %esi          # esi = si(y)
        cltd                       # Sign-extend EAX(x) into EDX:EAX
        idivl   %esi               # EDX:EAX / esi: x/y
		                           # eax - quotient
        ret
.L2:
        cmpw    $2, %dx            # compare dx(z) and 2
        jg      .L1                # if(signed) dx(z) > 2, go to .L1
		                           # return rax(z+y-x)

								   # if(signed) dx(z) <= 2
        movswl  %dx, %eax          # eax = dx(z)
        movswl  %si, %esi          # esi = si(y)
        cltd                       # Sign-extend EAX(z) into EDX:EAX
        idivl   %esi               # EDX:EAX / esi: z/y
		                           # eax - quotient
.L1:
        ret
```
* `movsw` - Move Signed Word to Long. Move and sign-extend a 16-bit word to a 32-bit long value.
* `cltd` - Convert Long to Double. Sign-extend the value from the `EAX` register (32-bit) into the `EDX:EAX` register pair.


* draft:
```c
short test(short x, short y, short z)
{
	short rax = y + z - x;
	if(z>2 && z<=5) return rax;
	if(z>5 && y<=2) return x/y;
	if(z>5 && y>2) return x/z;
	if(z<=2) return z/y;
}

```
* final:
```c
short test(short x, short y, short z) {
	short val = y + z - x ;
	if (z > 5) {
		if (y <= 2)
			val = x/y ;
		else
			val = x/z ;
	} else if ( z <= 2 )
		val = z/y ;
	return val;
}
```

# 3.6.6 Implementing Conditional Branches with Conditional Moves
* The conventional way to implement conditional operations is through a conditional transfer of control.
* An alternate strategy is through a **conditional transfer of data**, which is implemented by a simple **conditional move** instruction.
![[3_6 Control.assets/image-20240515122043471.png|700]]
* `cmovge` - Move if greater or equal (SF=OF).
* why code based on conditional data transfers can outperform code based on conditional control transfers？
	* Processors achieve high performance through ==pipelining==.
	* To do this requires being able to determine the sequence of instructions to be executed well ahead of time in order to **keep the pipeline full of instructions to be executed**.
	* If a conditional jump, the branch will only go until it has evaluated the branch condition. 
		* Processors employ sophisticated branch prediction logic to try to guess whether or not each jump instruction will be followed.
		* Mispredicting a jump, on the other hand, requires that the processor discard much of the work it has already done on future instructions and then begin filling the pipeline with instructions starting at the correct location.
		* Example:
			* For x86-64 code with conditional jumps, it takes:
				* 8 clock cycles per call when the branching pattern is easily predictable.
				* around 17.50 clock cycles per call when the branching pattern is random.
				* so the branch misprediction penalty is around 19 clock cycles: (17.5 - 8) x 2 = 19.
				* it means time required by the function ranges between around 8 and 27 cycles.
			* The code compiled using conditional moves requires around 8 clock cycles regardless of the data being tested.
* How to determine the penalty?
	* Formula - $T_{avg}(p) = (1− p)T_{OK} + p(T_{OK} + T_{MP}) = T_{OK} + pT_{MP}$
	* p - the probability of misprediction
	* $T_{OK}$ - the time to execute the code without misprediction
	* $T_{MP}$ - the misprediction penalty.
	* $T_{avg}$ - the average time to execute the code.

# Practice Problem 3.19
Running on a new processor model, our code required around 45 cycles when the branching pattern was random, and around 25 cycles when the pattern was highly predictable.

A. What is the approximate miss penalty?

B. How many cycles would the function require when the branch is mispredicted?

**Solution**:
A.
When the branching pattern was random, p = 0.5
We know $T_{OK}=25$
$45 = 0.5 \times 25 + 0.5 \times (25 + T_{MP})$
$T_{MP} = 40$

B. $T_{OK} + T_{MP}$ = 40 + 25 = 65


![[3_6 Control.assets/image-20240515125554967.png|700]]
* Each of these instructions has two operands: a source register or memory location S, and a destination register R.
	* The source value is read from either memory or the source register, but it is copied to the destination only if the specified condition holds.
	* The source and destination values can be 16, 32, or 64 bits long.
* General form of conditional expression and assignment: `v = test-expr ? then-expr : else-expr`
	* The standard way to compile this expression:
	```c
	if (!test-expr)
		goto false;
	v = then-expr;
	goto done;
	false:
		v = else-expr;
	done:
	```
	* Based on conditional move, it can also be:
	```c
	v = then-expr;
	ve = else-expr;
	t = test-expr;
	if (!t) v = ve; //implemented with a conditional move
	```
	* The final statement in this sequence is implemented with a conditional move — value ve is copied to v only if test condition t does not hold.

* Not all conditional expressions can be compiled using conditional moves. Example:
	```c
	long cread(long *xp) {
		return (xp ? *xp : 0);
	}
	```
	* compile using conditional move:
		![[3_6 Control.assets/image-20240515162529473.png]]
	* The dereferencing of `xp` by the `movq` instruction (line 2) occurs even when the test fails, causing a null pointer dereferencing error.

* Using conditional moves also does not always improve code efficiency.
	* if either the then-expr or the else-expr evaluation requires a significant computation, then this effort is wasted when the corresponding condition does not hold.

# Practice Problem 3.20
In the following C function, we have left the definition of operation OP incomplete:
```c
#define OP ____________ /* Unknown operator */

short arith(short x) {
	return x OP 16;
}
```
When compiled, gcc generates the following assembly code:
```z80
# short arith(short x)
# x in %rdi

arith:
	leaq    15(%rdi), %rbx
	testq   %rdi, %rdi
	cmovns  %rdi, %rbx
	sarq    $4, %rbx
	ret
```
A. What operation is OP?
B. Annotate the code to explain how it works.

**Solution**:
```z80
# short arith(short x)
# x in %rdi

arith:
	leaq    15(%rdi), %rbx  # rbx = 15 + rdi(x)
	testq   %rdi, %rdi      # test rdi & rdi
	cmovns  %rdi, %rbx      # if rdi(x) >= 0, rbx = rdi(x)
	sarq    $4, %rbx        # rbx >> 4
	ret
```
So we get:
```c
short arith(short x)
{
	int rbx = 15 + x;
	if(x >= 0) rbx = x;
	rbx = rbx / 16;
	return rbx;
}
```
$x/2^4=(x<0? x+(1<<4)-1 : x)>>4$

We easily find it's a Two’s-complement division by a power of 2.
So we get the operation is `/`.

# Practice Problem 3.21
Starting with C code of the form
```c
short test(short x, short y) {
	short val = ________ ;
	if ( _______ ) {
		if ( _______ )
			val = _______ ;
		else
			val = _______ ;
	} else if ( _______ )
		val = _______ ;
	return val;
}
```
GCC generates the following assembly code:
```z80
# short test(short x, short y)
# x in %rdi, y in %rsi
.globl  test
test:
        leal    12(%rsi), %eax
        testw   %di, %di
        js      .L5
        cmpw    $10, %si
        jle     .L1
        movswl  %di, %eax
        movswl  %si, %esi
        cltd
        idivl   %esi
.L1:
        ret
.L5:
        cmpw    %di, %si
        jle     .L3
        movl    %edi, %eax
        imull   %esi, %eax
        ret
.L3:
        movl    %esi, %eax
        orl     %edi, %eax
        ret
```
Fill in the missing expressions in the C code.

**Solution**:
```z80
# short test(short x, short y)
# x in %rdi, y in %rsi
.globl  test
test:
        leal    12(%rsi), %eax   # eax = 12 + rsi(y)
        testw   %di, %di
        js      .L5              # if di(x) < 0, go to .L5
		                         # if di(x) >= 0
        cmpw    $10, %si         # compare si(x) and 10
        jle     .L1              # if x <= 10, go to .L1(return)
		                         # if x > 10
        movswl  %di, %eax        # eax = x
        movswl  %si, %esi        # y = y 
        cltd
        idivl   %esi             # x / y
		                         # eax = quotient
.L1:
        ret
.L5:
        cmpw    %di, %si         # compare si(y) and di(x)
        jle     .L3              # if y<=x, go to .L3
		                         # if y>x
        movl    %edi, %eax       # eax = x
        imull   %esi, %eax       # eax = x * y
        ret
.L3:
        movl    %esi, %eax       # eax = y
        orl     %edi, %eax       # eax = x | y
        ret
```
* `cltd` - Convert Long to Double. Sign-extend the value from the `EAX` register (32-bit) into the `EDX:EAX` register pair.

```c
short test(short x, short y)
{
	if(x>=0 && x<=10) return 12+y;
	if(x<0 && y<=x) return x | y;
	if(x<0 && y>x) return x*y;
    if(x>10) return x/y;
}
```

```c
short test(short x, short y) {
	short val = 12 + y ;
	if ( x < 0 ) {
		if ( y <= x )
			val = x|y ;
		else
			val = x*y ;
	} else if ( x > 10 )
		val = x/y ;
	return val;
}
```


# 3.6.7 Loops
* Combinations of conditional tests and jumps are used to implement the effect of loops.
## Do-While Loops
* General form of a do-while statement:
```c
do
	body-statement
	while (test-expr);
```
* Translate into conditionals and goto statements:
```c
loop:
	body-statement
	t = test-expr;
	if (t)
		goto loop;
```
* Example:
	![[3_6 Control.assets/image-20240516105259745.png]]

# Practice Problem 3.22
A. Try to calculate 14! with a 32-bit int. Verify whether the computation of 14! overflows.

B. What if the computation is done with a 64-bit long int?

**Solution**:
A.
* Code(a.s)
```z80
.extern test_a
.extern printf

.section .data
value:
        .int 14

output:
        .asciz  "The result is 0x %08x\n"

error_stat:
        .asciz  "Overflow detected!\n"

.section .bss
        .lcomm  result, 4
        .lcomm  ofvalue1, 4
        .lcomm  ofvalue2, 4

.section .text
.globl _start
_start:
        nop
        movl    value, %edi
        call    test_a
        movl    %eax, result

        pushl   %eax
        pushl   $output
        call    printf

        # compute 14! / 14
        movl    result, %eax
        cdq     # signed expand eax to edx:eax
        movl    value, %edi
        idivl   %edi
        movl    %eax, ofvalue1  # save quotient on stack

        # compute 13!
        movl    value, %edi
        decl    %edi
        call    test_a
        movl    %eax, ofvalue2

        movl    ofvalue1, %edi  # pop quotient to stack
        movl    ofvalue2, %eax
        cmpl    %edi, %eax
        je      .done
        pushl   $error_stat
        call    printf

.done:
        movl    $0, %ebx
        movl    $1, %eax
        int     $0x80
```
* test_a.s
```z80
# int test_a(int n)
# Parameters:
#       - edi: n
# Return: eax

.globl test_a
test_a:

.section .text
        pushl   %ebp
        movl    %esp, %ebp
        movl    $1, %eax

.loop:
        imull   %edi, %eax
        decl    %edi
        testl   %edi, %edi
        jg      .loop

        leave 
        ret
```
* makefile
```shell
a: a.o test_a.o
        ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 a.o test_a.o -o a -lc
a.o: a.s
        as --32 -gstabs a.s -o a.o
test_a.o: test_a.s
        as --32 -gstabs test_a.s -o test_a.o
clean:
        rm -f *.o a
```
* run
```shell
root@ml:~/csapp/chap3/prac3_22/a # ./a
The result is 0x 4c3b2800
Overflow detected!
```

B.
* Code (b.s)
```z80
.extern test_b
.extern printf

.section .data
value:
        .quad   14

output:
        .asciz  "The result is 0x %016llx\n"

error_stat:
        .asciz  "Overflow detected!\n"

.section .bss
        .lcomm  result, 8
        .lcomm  ofvalue1, 8
        .lcomm  ofvalue2, 8

.section .text
.globl _start
_start:
        movq    value, %rdi
        call    test_b
        movq    %rax, result

        movq    %rax, %rsi
        movq    $output, %rdi
        call    printf

        # compute 14! / 14
        movq    result, %rax
        cqo     # signed expand rax to rdx:rax
        movq    value, %rdi
        idivq   %rdi
        movq    %rax, ofvalue1  # save quotient on stack

        # compute 13!
        movq    value, %rdi
        decq    %rdi
        call    test_b
        movq    %rax, ofvalue2

        movq    ofvalue1, %rdi  # pop quotient to stack
        movq    ofvalue2, %rax
        cmpq    %rdi, %rax
        je      .done
        movq    $error_stat, %rdi
        call    printf

.done:
        movq    $60, %rax
        syscall
```
* Code (test_b.s)
```z80
# longlong test_b(longlong n)
# Parameters:
#       - rdi: n
# Return: rax

.globl test_b
test_b:

.section .text
        pushq   %rbp
        movq    %rsp, %rbp
        movq    $1, %rax

.loop:
        imulq   %rdi, %rax
        decq    %rdi
        testq   %rdi, %rdi
        jg      .loop

        leave 
        ret
```
* makefile:
```z80
b: b.o test_b.o
        ld -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 b.o test_b.o -o b -lc
b.o: b.s
        as --64 -gstabs b.s -o b.o
test_b.o: test_b.s
        as --64 -gstabs test_b.s -o test_b.o
clean:
        rm -f *.o b
```
* run
```shell
root@ml:~/csapp/chap3/prac3_22/b# ./b
The result is 0x 000000144c3b2800
```

# Practice Problem 3.23
For the C code
```c
short dw_loop(short x) {
	short y = x/9;
	short *p = &x;
	short n = 4*x;

	do {
		x += y;
		(*p) += 5;
		n -= 2;
	} while (n > 0);

	return x;
}
```
gcc generates the following assembly code:
```z80
# short dw_loop(short x)
# x initially in %rdi
.globl  dw_loop
.type   dw_loop, @function
dw_loop:
        movl    %edi, %edx
        movswl  %di, %ecx
        imull   $7282, %ecx, %ecx
        shrl    $16, %ecx
        movl    %edi, %eax
        sarw    $15, %ax
        subl    %eax, %ecx
        sall    $2, %edi
.L2:
        leal    5(%rcx,%rdx), %edx
        leal    -2(%rdi), %eax
        movl    %eax, %edi
        testw   %ax, %ax
        jg      .L2
        movl    %edx, %eax
        ret
```

A. Which registers are used to hold program values x, y, and n?
B. How has the compiler eliminated the need for pointer variable p and the pointer dereferencing implied by the expression (*p)+=5?
C. Add annotations to the assembly code describing the operation of the program, similar to those shown in Figure 3.19(c).

**Solution**:
* Add annotations:
```z80
# short dw_loop(short x)
# x initially in %rdi
.globl  dw_loop
.type   dw_loop, @function
dw_loop:
	    movl    %edi, %edx  # edx = edi(x) = x
        movswl  %di, %ecx   # ecx = di(x)
        imull   $7282, %ecx, %ecx  # ecx = ecx x 7282 = 7282*x
        shrl    $16, %ecx   # ecx = 1/9 * x = y
        movl    %edi, %eax  # eax = edi(x)
        sarw    $15, %ax    # ax = 0 or -1: get the sign bit
        subl    %eax, %ecx  # adjust the deviation by sign extention and arithmetic shifts, now ecx = y
        sall    $2, %edi    # edi = 4*edi = 4*x = n
.L2:
        leal    5(%rcx,%rdx), %edx  # edx = 5 + rcx + rdx = 5+y+x
        leal    -2(%rdi), %eax      # eax = n - 2
        movl    %eax, %edi          # edi = eax = n - 2
        testw   %ax, %ax
        jg      .L2                 # if eax(n-2) > 0, go to .L2
        movl    %edx, %eax
        ret
```
* Why 7282？
	* If we want to realize x/9, it's impossible to find the exact number with only shift operations.
	* Thus we need to find the closest number using shift operations.
	* To simplify, we need to find a constant number to make c/2^n $\approx$ 1/9. 
	* Let's find a proper number n = 16, so c = 1/9 $\times$ 65536 = 7281.78 $\approx$ 7282.
	* therefore, we get that 1/9 is 65536 logically shift right by 16 bits.

A. Which registers are used to hold program values x, y, and n?
edx - x
ecx - y
edi - n
B. How has the compiler eliminated the need for pointer variable p and the pointer dereferencing implied by the expression (*p)+=5?
It just simply assumes that p always points to x
C. Add annotations to the assembly code describing the operation of the program, similar to those shown in Figure 3.19(c).

## While Loops
* General form:
```c
while (test-expr)
	body-statement
```
* It differs from do-while in that test-expr is evaluated and the loop is potentially terminated before the first execution of body-statement.
### 1st translation method - jump to middle
* **Jump to middle** performs the initial test by performing an unconditional **jump to the test at the end of the loop**.
```c
	goto test;
loop:
	body-statement
test:
	t = test-expr;
	if (t)
		goto loop;
```
	![[3_6 Control.assets/image-20240517144442908.png|500]]

### 2nd translation method - guarded do
* Guarded do first transforms the code into a do-while loop by using a conditional branch to **skip over the loop if the initial test fails**.
* Gcc follows this strategy when compiling with higher levels of optimization, for example, with **command-line option -O1**.
* Using this implementation strategy, the compiler can often optimize the initial test.
* General form:
```c
t = test-expr;
if (!t)
	goto done;
do
	body-statement
while (test-expr);
done:
```
* goto code:
```c
t = test-expr;
if (!t)
	goto done;
loop:
	body-statement
	t = test-expr;
	if (t)
		goto loop;
done:
```
![[3_6 Control.assets/image-20240519091323183.png|600]]




# Practice Problem 3.24
For C code having the general form
```c
short loop_while(short a, short b)
{
	short result = ______ ;
	while ( ______ ) {
		result = ______ ;
		a = ______ ;
	}
	return result;
}
```
GCC, run with command-line option -Og, produces the following code:
```z80
# short loop_while(short a, short b)
# a in %rdi, b in %rsi

loop_while:
	movl $0, %eax
	jmp .L2
.L3:
	leaq (,%rsi,%rdi), %rdx
	addq %rdx, %rax
	subq $1, %rdi
.L2:
	cmpq %rsi, %rdi
	jg .L3
	rep; ret
```
We can see that the compiler used a jump-to-middle translation, using the jmp instruction on line 3 to jump to the test starting with label .L2. Fill in the missing parts of the C code.

**Solution**:
```z80
# short loop_while(short a, short b)
# a in %rdi, b in %rsi

loop_while:
	movl $0, %eax     # eax = 0
	jmp .L2
.L3:
	leaq (,%rsi,%rdi), %rdx # rdx = rdi(a) * rsi(b)
	addq %rdx, %rax         # rax = rax + rdx = rax + a * b
	subq $1, %rdi           # rdi = rdi - 1 = a - 1
.L2:
	cmpq %rsi, %rdi   # compare rdi(a) and rsi(b)
	jg .L3            # if rdi(a) > rsi(b), go to .L3
	rep; ret
```
* then we get below:
```
short loop_while(short a, short b)
{
	int eax = 0;
	while(a > b)
	{
		rax += (a*b);
		a--;
	}
	return eax;
}
```
* fill in the code below:
```c
short loop_while(short a, short b)
{
	short result = 0;
	while ( a > b ) {
		result = result + a * b;
		a = a - 1;
	}
	return result;
}
```

# Practice Problem 3.25
For C code having the general form
```c
long loop_while2(long a, long b)
{
	long result = ______ ;
	while ( ______ ) {
		result = ______ ;
		b = ______ ;
	}
	return result;
}
```
gcc, run with command-line option -O1, produces the following code:
```z80
# long loop_while2(long a, long b)
# a in %rdi, b in %rsi

loop_while2:
	testq %rsi, %rsi
	jle .L8
	movq %rsi, %rax
.L7:
	imulq %rdi, %rax
	subq %rdi, %rsi
	testq %rsi, %rsi
	jg .L7
	rep; ret
.L8:
	movq %rsi, %rax
	ret
```
We can see that the compiler used a guarded-do translation, using the `jle` instruction on line 3 to skip over the loop code when the initial test fails. Fill in the missing parts of the C code. Note that the control structure in the assembly code does not exactly match what would be obtained by a direct translation of the C code according to our translation rules. In particular, it has two different ret instructions (lines 10 and 13). However, you can fill out the missing portions of the C code in a way that it will have equivalent behavior to the assembly code.

**Solution**:
```z80
# long loop_while2(long a, long b)
# a in %rdi, b in %rsi

loop_while2:
	testq %rsi, %rsi
	jle .L8            # if b <=0, go to .L8
					   # if b>0
	movq %rsi, %rax    # rax = rsi(b)
.L7:
	imulq %rdi, %rax   # rax = rax*rdi = b*a
	subq %rdi, %rsi    # rsi = rsi-rdi: b=b-a
	testq %rsi, %rsi   
	jg .L7             # if b>0, go to .L7
	rep; ret           # if b<=0, return
.L8:                   
	movq %rsi, %rax    # rax = rsi(b)
	ret
```
* we get the draft code logically:
```c
# long loop_while2(long a, long b)
# a in %rdi, b in %rsi

long loop_while2(long a, long b)
{
	if(b<=0) return b;
	if(b>0)
	{
		rax = b;
		do{
			rax = rax*a;
			b = b-a;
		}while(b>0)
		return rax;
	}
}
```
* Finalise the final code:
```c
# long loop_while2(long a, long b)
# a in %rdi, b in %rsi
long loop_while2(long a, long b)
{
	long result = b;
	while (b > 0) {
		result = result*a;
		b = b-a;
	}
	return result;
}
```

# Practice Problem 3.26
A function test_one has the following overall structure:
```c
short test_one(unsigned short x) {
	short val = 1;
	while ( ... ) {
		.
		.
		.
	}
	return ...;
}
```
The gcc C compiler generates the following assembly code:
```z80
# short test_one(unsigned short x)
# x in %rdi

test_one:
	movl $1, %eax
	jmp .L5
.L6:
	xorq %rdi, %rax
	shrq %rdi        # Shift right by 1
.L5:
	testq %rdi, %rdi
	jne .L6
	andl $0, %eax
	ret
```
Reverse engineer the operation of this code and then do the following:
A. Determine what loop translation method was used.
B. Use the assembly-code version to fill in the missing parts of the C code.
C. Describe in English what this function computes.

**Solution**:
```z80
# short test_one(unsigned short x)
# x in %rdi

test_one:
	movl $1, %eax    # eax = 1
	jmp .L5
.L6:
	xorq %rdi, %rax  # rax = rax^x
	shrq %rdi        # x=x>>1
.L5:
	testq %rdi, %rdi 
	jne .L6          # if x != 0, go to .L6
	                 # if x == 0, 
	andl $0, %eax    # eax = 0
	ret
```
* Draft code:
```c
# short test_one(unsigned short x)
# x in %rdi

short test_one(unsigned short x)
{
	eax = 1;
	if(x==0) return 0;
	if(x!=0)
	{
		rax=rax^x;
		
	}

}
```



















































