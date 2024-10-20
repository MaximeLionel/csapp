* Machine code provides two basic low-level mechanisms for implementing conditional behavior: it tests data values and then alters either the ==control flow== or the ==data flow== based on the results of these tests.
* The execution order of a set of machine code instructions can be altered with a `jump` instruction, indicating that control should pass to some other part of the program, possibly contingent on the result of some test.

# 3.6.1 Condition Codes
* CPU maintains a set of single-bit condition code registers describing attributes of the most recent arithmetic or logical operation.
* These condition codes are the most useful:
	* CF: Carry flag. The most recent operation generated a carry out of the most significant bit (MSB). Used to ==detect overflow for unsigned operations==.
	* ZF: Zero flag. The most recent operation yielded zero.
	* SF: Sign flag. The most recent operation yielded a negative value.
	* OF: Overflow flag. The most recent operation caused ==a two’s-complement overflow—either negative or positive==.
* Example:
	* suppose: `t = a + b` using `add` instruction
	* CF is set while (unsigned) t < (unsigned) a: Unsigned overflow
	* ZF is set while (t == 0): Zero
	* SF is set while (t < 0): Negative
	* OF is set while (a < 0 == b < 0) && (t < 0 != a < 0): Signed overflow
* `leaq` instruction does not alter any condition codes, since it is intended to be used in address computations.
* all of the instructions listed below cause the condition codes to be set while altering registers at the same time:
	* xor - cf = 0, of = 0
	* For the shift operations, cf = the last bit shifted out, of = 0.
<<<<<<< HEAD
		![[3_6 Control.assets/image-20240510155445965.png|400]]
* 2 instruction classes (having 8-, 16-, 32-, and 64-bit forms) that set condition codes without altering any other registers:
	* `cmp` instructions set the condition codes according to the differences of their two operands:
		* behave in the same way as the `sub` instructions, except that they set the condition codes without updating their destinations.
	* `test` instructions behave in the same manner as the `and` instructions, except that they set the condition codes without altering their destinations.
	![[3_6 Control.assets/image-20240510160218283.png|400]]
=======
	![[3_6 Control.assets/image-20240510155445965.png|400]]
* 2 instruction classes (having 8-, 16-, 32-, and 64-bit forms) that set condition codes without altering any other registers:
	* `cmp` instructions set the condition codes according to the differences of their two operands:
		* behave in the same way as the sub instructions, except that they set the condition codes without updating their destinations.
	* `test` instructions behave in the same manner as the and instructions, except that they set the condition codes without altering their destinations.
	![[3_6 Control.assets/image-20240510160218283.png|300]]
>>>>>>> origin/main


# 3.6.2 Accessing the Condition Codes
* 3 common ways of using the condition codes:
	1. set a single byte to 0 or 1 depending on some combination of the condition codes 
		* `set` instruction
	2. conditionally jump to some other part of the program 
	3. conditionally transfer data
* The `set` instructions. Each instruction sets a single byte to 0 or 1 based on some combination of the condition codes.
	![[3_6 Control.assets/image-20240510161458720.png]]
* Example `setg`:
	* If we want ~(SF^OF)&~ZF == 1, then ~ZF == 1 and ~(SF^OF) == 1
	* for ~ZF == 1, easy to understand, if `greater than`, the result cannot be zero.
	* for ~(SF^OF) == 1, then SF^OF == 0, which means SF and OF must be same value (both 1 or both 0).
		* if sf == of == 0, of == 0 means no signed overflow, and sf == 0, means that the result is greater than 0. Easy to get 'greater than'.
		* if sf == of == 1, of == 1 means signed overflow, and sf == 1, means that the result is negative. Suppose the original operation is a - b, we get that a > 0, b < 0. It is also `greater than`.
* Example `setl`:
	* suppose the original operation is `cmpq b, a`, which change the condition codes according to `a - b`.
	* for `setl D`, if we want to set D to 1, which means a < b.
	* If SF\^OF == 1, then sf == 1, of == 0 or sf == 0, of == 1
		* if sf == 1, of == 0, means a - b < 0 and no signed overflow, we can get a < b easily.
		* if sf == 0, of == 1, means a - b > 0 and signed overflow, we can get that a < 0 and b > 0, same as a < b
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
		* even though `l` of `movzbl` means 4 bytes (32bits), it actually equal to `movzbq` in reality, which means clear high 7 bytes of `%rax` register.
	* Condition 1 (signed a - b) - `setl`:
		* `sete` or `setz`: set when a == b
		* `setl`: set when a < b if `sf^of`
			* case1 (sf == 1, of == 0): $a-^t_wb<0$ 
			* case2 (sf == 0, of == 1): $a-^t_wb>0$ means a<0, b>0, negative overflow
		* `setge`: set when a >= b if ~(sf^of)
			* case1 (sf == 0, of == 0): $a-^t_wb>0$ 
			* case2 (sf == 1, of == 1): $a-^t_wb<0$ means a>0, b<0, positive overflow
		* `sete`: set when a == b if zf
	* Condition 2 (unsigned a - b) - replace `setl` with `setb`:
		* `seta`: set when a > b if `~cf & ~zf`
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
![[3_6 Control.assets/image-20240510171846633.png|400]]

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
cmpl %esi, %edi
setl %al

data_t - int
COMP - a < b

B.
cmpw %si, %di
setge %al

data_t - short
COMP - a >= b

C.
cmpb %sil, %dil
setbe %al

data_t - unsigned char
COMP - a <= b

D.
cmpq %rsi, %rdi
setne %al

data_t - long or unsigned long
COMP - a != b

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
testq %rdi, %rdi
setge %al

data_t - long
TEST - a >= 0

B.
testw %di, %di
sete %al

data_t - short or unsigned short
TEST - a == 0

C.
testb %dil, %dil
seta %al

data_t  - unsigned char
TEST - a > 0

D.
testl %edi, %edi
setle %al

data_t - int
TEST - a <= 0

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
* different encodings for jump targets:
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
According to AMD, their processors cannot properly predict the destination of a ret instruction when it is reached from a jump instruction. RAS (Return Address Prediction Stack) would be invalid.
The rep instruction serves as a form of no-operation here,
```

* Object file:
![[3_6 Control.assets/image-20240512222356179.png|300]]
* The target of the first jump instruction is encoded (in the second byte) as 0x03. 
* Adding this to 0x5, the address of the following instruction, we get jump target address 0x8.
* The target of the second jump instruction is encoded as 0xf8.
* Adding this to 0xd (decimal 13), the address of the instruction on line 6, we get 0x5, the address of the instruction on line 3.
* The value of the program counter when performing PC-relative addressing is the address of **the instruction following the jump**, not that of the jump itself.
	* the processor would update the program counter as its first step in executing an instruction.
* the disassembled version of the program after linking:
![[3_6 Control.assets/image-20240512222857761.png|400]]
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
A. 0x4003fc + 0x2 = 0x4003fe
B. 0x400431 + 0xf4 = 0x =0x400425
C. 
0x400547 - 0x02 = 0x400545
0x400545 - 2 = 0x400543
So we get:
```
	400543: 77 02   ja 400547
	400545: 5d      pop %rbp
```
D.
the encoding of `73 ff ff ff` should be 0xffffff73, which is `-0x8d`.
So XXXXXXX = 0x4005ed + (-0x8d) = 0x400560

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
* C syntax to describe the control flow using goto expression:
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
```
# void cond(short a, short *p)
# a in %di, p in %rsi
    testw   %di, %di
    je      .L1
    cmpw    %di, (%rsi)
    jge     .L1
    movw    %di, (%rsi)
.L1
	ret
```
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
	t = (x < y)
	if (t)
		goto true;
	ge_cnt++;
	result = x - y;
	goto done;
true:
	lt_cnt++;
	result = y - x;
done:
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
        leal    (%rdx,%rsi), %eax   # eax=rdx+rsi: eax = z + y
        subl    %edi, %eax          # eax=eax-edi: eax = z + y - x
        cmpw    $5, %dx             # cmp dx,5: cmp z - 5
        jle     .L2                 # if(z-5)<=0, goto .L2
        cmpw    $2, %si             # cmp si,2: cmp y - 2
        jle     .L3                 # if(y-2)<=0, goto .L3
        movswl  %di, %eax           # eax=di: eax = x
        movswl  %dx, %ecx           # ecx=dx: ecx = z
        cltd                        # sign-extend eax into edx:eax: x
        idivl   %ecx                # edx:eax/ecx: x/z - eax=quotient
        ret
.L3:                                # if(z>5 && y<=2)
        movswl  %di, %eax           # eax=di: eax = x
        movswl  %si, %esi           # esi=si: esi = y
        cltd                        # sign-extend eax into edx:eax: x
        idivl   %esi                # edx:eax/esi: x/y - eax=quotient
        ret
.L2:                                # if(z<=5)
        cmpw    $2, %dx             # cmp dx,2: cmp z-2
        jg      .L1                 # if(z-2 > 0) return z + y - x
                                    # if(z-2 <= 0)
        movswl  %dx, %eax           # eax=dx: eax = z
        movswl  %si, %esi           # y
        cltd                        # sign-extend eax into edx:eax: z
        idivl   %esi                # edx:eax/esi: z/y - eax=quotient
.L1:
        ret
```

```c
short test(short x, short y, short z) {
	if(z>5 && y>2) return x/z;
	if(z>5 && y<=2) return x/y;
	if(z<=5 && z>2) return z + y - x;
	if(z<=2) return z/y
	return val;
}
```


```c
short test(short x, short y, short z) {
	short val = z + y - x ;
	if (z > 5) {
		if (y > 2)
			val = x/z ;
		else
			val = x/y ;
	} else if ( z <= 2 )
		val = z/y ;
	return val;
}
```
# 3.6.6 Implementing Conditional Branches with Conditional Moves
* The conventional way to implement conditional operations is through a conditional transfer of control.
* An alternate strategy is through a **conditional transfer of data**, which is implemented by a simple **conditional move** instruction.
![[3_6 Control.assets/image-20240515122043471.png|500]]
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
$T_{avg}(p)$ = 45
$T_{OK}$ = 25
p = 0.5
A.
$T_{avg}(p) = (1− p)T_{OK} + p(T_{OK} + T_{MP}) = T_{OK} + pT_{MP}$
we get $T_{MP}$ = 40

B.
$T_{OK} + T_{MP}$ = 40 + 25 = 65

![[3_6 Control.assets/image-20240515125554967.png|500]]
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
		![[3_6 Control.assets/image-20240515162529473.png|300]]
	* The dereferencing of `xp` by the `movq` instruction (line 2) occurs **even when the test fails**, causing a null pointer dereferencing error.

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
	leaq    15(%rdi), %rbx  # rbx=rdi+15: rbx = x + 15
	testq   %rdi, %rdi      # test x
	cmovns  %rdi, %rbx      # if x >= 0, rbx=rdi: rbx = x
	sarq    $4, %rbx        # rbx=rbx>>4
	ret
```
A.
$x/2^4$ = (x < 0? x+(1<<4)-1:x)>>4
B.
Two's compliment division operation.

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
        leal    12(%rsi), %eax    # eax=rsi+12: eax = y + 12
        testw   %di, %di
        js      .L5               # if x < 0, jump to .L5
							      # if x >=0 
        cmpw    $10, %si          # cmp 10,y: compare y,10
        jle     .L1               # if y-10<=0, jump to .L1
                                  # if y-10>0
        movswl  %di, %eax         # eax=di: eax = x
        movswl  %si, %esi         # esi=si: esi = y
        cltd                      # edx:eax=eax: edx:eax = x
        idivl   %esi              # edx:eax / esi: x/y
                                  # eax = quotient
.L1:
        ret
.L5:
        cmpw    %di, %si          # cmp x,y: compare y,x
        jle     .L3               # if y <= x, jump to .L3
                                  # if y > x
        movl    %edi, %eax        # eax=edi: eax = x
        imull   %esi, %eax        # eax=eax*esi: eax = x*y
        ret
.L3:
        movl    %esi, %eax        # eax=esi: eax = y
        orl     %edi, %eax        # eax=eax|edi: eax = y | x
        ret
```

```c
short test(short x, short y)
{
	if(x<0 && y<=x) return x|y;
	if(x>=0 && y<=10) return y+12;
	if(x>=0 && y>10) return x/y;
	if(x<0 && y>x) return x*y;
}
```

```c
short test(short x, short y) {
	short val = y + 12 ;
	if ( x < 0 ) {
		if ( y <= x )
			val = x | y ;
		else
			val = x*y ;
	} else if ( y > 10 )
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
![[3_6 Control.assets/image-20240519091323183.png|500]]

## For Loops
* General form of a for loop：
```c
for (init-expr; test-expr; update-expr)
	body-statement
```
* Transform to `while` loop:
```c
init-expr;            // evaluates the initialization expression init-expr
while (test-expr) {   // enters a loop where it first evaluates the test condition test-expr,
	body-statement    // executes the body of the loop body-statement
	update-expr;      // evaluates the update expression update-expr
}
```
* Goto code - jump to middle strategy:
```c
	init-expr;
	goto test;
loop:
	body-statement
	update-expr;
test:
	t = test-expr;
	if (t)
		goto loop;
```
* Goto code - guarded-do strategy:
```c
	init_expr;
	t = test-expr;
	if(!t) 
		goto done;
loop:
	body-statement
	update-expr;
	t = test-expr;
	if(t) goto loop;
done:
	return
```
* Example:
	```c
	long fact_for(long n)
	{
		long i;
		long result = 1;
		for (i = 2; i <= n; i++)
			result *= i;
		return result;
	}
	```
	* Find key element:
		```
			init_expr: i = 2
			test-expr: i <= n
			body-statement: result *= i
			update-expr: i++
		```
	* Transform to `while` loop:
		```c
		long fact_for(long n)
		{
			long i = 2;
			long result = 1;
			while(i <= n)
			{
				result *= i;
				i++;
			}
			return result;
		}
		```

	* Goto code - jump to middle strategy:
		```c
		long fact_for(long n)
		{
			long i = 2;
			long result = 1;
			goto test;
			
		loop:
			result *= i;
			i++;
			
		test:
			if(i <= n) goto loop;
			return result;
		}
		```
	* Goto code - guarded-do strategy:
		```c
		long fact_for(long n)
		{
			long i = 2;
			long result = 1;
			if(i > n) goto done;
			
		loop:
			result *= i;
			i++;
			if(i <= n) goto loop;
			
		done:
			return result;
		}
		```
	* GCC -Og compile:
		```z80
		fact_for:
		        movl    $1, %edx
		        movl    $2, %eax
		        jmp     .L2
		.L3:
		        imulq   %rax, %rdx
		        addq    $1, %rax
		.L2:
		        cmpq    %rdi, %rax
		        jle     .L3
		        movq    %rdx, %rax
		        ret
		```
		* we easily find that gcc actually use jump-to-middle strategy with `-Og` option.


# Practice Problem 3.22
A. Try to calculate 14! (14 factorial - 14的阶乘) with a 32-bit int. Verify whether the computation of 14! overflows.

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
        idivl   %edi       # edx:eax / edi
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

# Side - explain on assembling and linking command
`ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 a.o test_a.o -o a -lc`
* ld - GNU linker
* -m elf_i386 - force the liner to produce elf_i386 format (x86)
* -dynamic-linker /lib/ld-linux.so.2 - tells the linker to use dynamic linker and also the path to the specific dynamic linker.
* a.o test_a.o -o a - use a.o and test_a.o to produce the executable file a.
* -lc - tells the linker to link the standard C library.

`as --32 -gstabs a.s -o a.o`
* as - GNU assembler
* --32 - force the assembler to produce a 32bit object file.
* -gstabs - include the debugging information in produced object file.
* a.s -o a.o - use a.s to produce the object file a.o
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
B. How has the compiler eliminated the need for pointer variable p and the pointer dereferencing implied by the expression (\*p)+=5?
C. Add annotations to the assembly code describing the operation of the program, similar to those shown in Figure 3.19(c).

**Solution**:
```z80
# short dw_loop(short x)
# x initially in %rdi
.globl  dw_loop
.type   dw_loop, @function
dw_loop:
        movl    %edi, %edx          # edx=edi: edx = x
        movswl  %di, %ecx           # ecx=(dw)di: ecx = x
        imull   $7282, %ecx, %ecx   # ecx=ecx*7282: ecx = x * 7282
        shrl    $16, %ecx           # ecx=ecx>>16: ecx = (x * 7282)>>16 = x / 9 = y
        movl    %edi, %eax          # eax=edi: eax = x
        sarw    $15, %ax            # ax=ax>>15: ax = 0 or -1 to indicate sign bit of x
        subl    %eax, %ecx          # ecx=ecx-eax: deviation adjustment
        sall    $2, %edi            # edi=edi*4: edi = x*4 = n
.L2:
        leal    5(%rcx,%rdx), %edx  # edx=5+rcx+rdx: edx = 5 + y + x
        leal    -2(%rdi), %eax      # eax=rdi-2: eax = n - 2
        movl    %eax, %edi          # edi=eax: edi = n - 2
        testw   %ax, %ax            # test n-2
        jg      .L2                 # if n - 2 > 0, go to .L2
        movl    %edx, %eax          # eax=edx: eax = 5 + y + x
        ret
```
* Why 7282?
	* We get from next instruction: ecx = (x * 7282)>>16
	* To focus on (x * 7282)>>16 = (x * 7282) / $2^{16}$ = x * (7282/$2^{16}$) = x / 9

A. 
x - edx
y - ecx
n - edi

B. 
Because the code does not use pointer p alone, but always \*p directly, the compiler directly use x to replace \*p for convenience. 
For example, to realize `(*p) += 5`, the compiler simply add 5 to x directly by `leal    5(%rcx,%rdx), %edx`.

C.
Done.

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
	movl $0, %eax      # eax=0
	jmp .L2
.L3:
	leaq (,%rsi,%rdi), %rdx    # rdx=rsi*rdi: rdx = b * a
	addq %rdx, %rax    # rax=rax+rdx: rax = rax + a*b
	subq $1, %rdi      # rdi=rdi-1: a--
.L2:
	cmpq %rsi, %rdi    # compare rdi and rsi - cmp a,b
	jg .L3             # if a > b, jump to .L3
	rep; ret
```

```
short loop_while(short a, short b)
{
	int result = 0;   // result - eax
	while(a > b)
	{
		result += a*b;
		a--;
	}
	return result;
}
```

```c
short loop_while(short a, short b)
{
	short result = 0;
	while ( a > b ) {
		result = result + a*b ;
		a = a - 1 ;
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
	testq %rsi, %rsi   # test b,b
	jle .L8            # if(b<=0), go to .L8
	                   # if(b > 0)
	movq %rsi, %rax    # rax=rsi: rax = b
.L7:
	imulq %rdi, %rax   # rax=rax*rdi: rax = b*a
	subq %rdi, %rsi    # rsi=rsi-rdi: rsi = b - a
	testq %rsi, %rsi   # test rsi,rsi
	jg .L7             # if(rsi>0), go to .L7
	                   # if(rsi<=0)
	rep; ret
.L8:
	movq %rsi, %rax    # rax=rsi: rax = b
	ret
```
* Create a draft code:
```
long loop_while2(long a, long b)
{
	if(b <= 0) return b;
	long result = b;
	{
		result = result * a;
		b = b - a;
	}while(b > 0);
	return result;
}
```
* Finalize the final code:
```c
long loop_while2(long a, long b)
{
	long result = b;
	while ( b > 0 ) {
		result = result * a;
		b = b - a ;
	}
	return result;
}
```

# Practice Problem 3.26
* Original problem has a mistake!
A function test_one has the following overall structure:
```c
short test_one(unsigned short x) {
	short val = 0;
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
* This is a problem because `andl $0, %eax` always makes `eax` 0
* So we rewrite c program and assemble as below:
```z80
# short test_one(unsigned short x)
# x in %rdi

test_one:
        movl    $0, %eax
        jmp     .L2
.L3:
        movl    %edi, %edx
        andl    $1, %edx
        xorl    %edx, %eax
        shrw    %di
.L2:
        testw   %di, %di
        jne     .L3
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
        movl    $0, %eax       # eax=0
        jmp     .L2            # jump to middle
.L3:
        movl    %edi, %edx     # edx=edi: edx = x
        andl    $1, %edx       # edx=edx&1: edx = x & 1
        xorl    %edx, %eax     # eax=eax^edx: eax = eax ^ (x&1)
        shrw    %di            # di=di>>1: x = x >> 1
.L2:
        testw   %di, %di       # test di: test x,x
        jne     .L3            # if(x != 0) jump to .L3
        ret
```
* Create the draft code:
```c
short test_one(unsigned short x)
{
	int eax = 0;
	while(x != 0)
	{
		eax = eax ^ (x&1);
		x >> 1;
	}
	return eax;
}
```
* Finalize the C code:
```c
short test_one(unsigned short x) {
	short val = 0;
	while (x != 0) {
		val = val^(x&1);
		x = x >> 1;
	}
	return val;
```

A. Jump to middle.
B. Done.
C. Find out the parity of x. 
If there's an odd number of 1s, the function return 0.
If there's an even number of 1s, the function return 1.

# Practice Problem 3.27
Write goto code for a function called fibonacci to print fibonacci numbers using a while loop. Apply the guarded-do transformation.

**Solution**:
```c
// void fibo_gd_goto(int n)
// parameter - n: the number of fibo numbers you want to print
void fibo_gd_goto(int n)
{
	int num1 = 0;
	int num2 = 1;
	int i = 3;
	int new_fibo;

	if(i > n) goto done;
	print(num1)
	print(num2)

loop:
	new_fibo = num1 + num2;
	num1 = num2;
	num2 = new_fibo;
	print(new_fibo);
	i++;
	if(i < n) goto loop;
	
done:
	return;
}
```

# Practice Problem 3.28
A function test_two has the following overall structure:
```c
unsigned long test_two(unsigned long x) {
	unsigned long val = 0;
	long i;
	for ( ... ; ... ; ... ) {
		.
		.
		.
	}
	return val;
}
```
The gcc C compiler generates the following assembly code:
```z80
	movl	$64, %edx
	movl	$0, %eax
	jmp	.L2            # could be deleted
.L3:
	addq	%rax, %rax
	movq	%rdi, %rcx
	andl	$1, %ecx
	orq	%rcx, %rax
	shrq	%rdi
	subq	$1, %rdx
.L2:
	testq	%rdx, %rdx
	jne	.L3
	ret
```
Reverse engineer the operation of this code and then do the following:

A. Use the assembly-code version to fill in the missing parts of the C code.

B. Explain why there is neither an initial test before the loop nor an initial jump to the test portion of the loop.

C. Describe in English what this function computes.

**Solution**:
```z80
# unsigned long test_two(unsigned long x)
# x - %rdi
	movl	$64, %edx     # edx=64
	movl	$0, %eax      # eax=0
	jmp	.L2
.L3:
	addq	%rax, %rax    # rax=rax+rax: rax *= 2
	movq	%rdi, %rcx    # rcx=rdi: rcx = x
	andl	$1, %ecx      # ecx=ecx&1: get the LSB of ecx
	orq	%rcx, %rax        # rax=rax|rcx: rax = rax | ecx&1
	shrq	%rdi          # rdi=rdi>>1: rdi = rdi/2
	subq	$1, %rdx      # rdx--
.L2:
	testq	%rdx, %rdx   # test rdx
	jne	.L3              # if rdx!=0, goto .L3
	ret
```

A.
```c
unsigned long test_two(unsigned long x) {
	unsigned long val = 0;
	long i;
	for ( i = 64 ; i != 0 ; i-- ) {
		val = (val<<1) | (x&1);
		x = x >> 1;
	}
	return val;
}
```

B.
Because the rdx has just been assign an immediate number 64, it's obviously unequal to 0. Therefore, this step is skipped to improve efficiency.

C.
Reverse the bits of x and return.

# Practice Problem 3.29
Executing a `continue` statement in C causes the program to jump to the end of the current loop iteration. The stated rule for translating a `for` loop into a `while` loop needs some refinement when dealing with `continue` statements. For example, consider the following code:
```c
/* Example of for loop containing a continue statement */
/* Sum even numbers between 0 and 9 */

long sum = 0;
long i;

for (i = 0; i < 10; i++) {
	if (i & 1)
		continue;
	sum += i;
}
```
A. What would we get if we naively applied our rule for translating the `for` loop into a `while` loop? What would be wrong with this code?

B. How could you replace the `continue` statement with a `goto` statement to ensure that the `while` loop correctly duplicates the behavior of the `for` loop?

**Solution**:
A.
```c
/* Example of for loop containing a continue statement */
/* Sum even numbers between 0 and 9 */

long sum = 0;
long i = 0;

while(i < 10)
{
	if (i & 1)
		continue;
	sum += i;    // if execute 'continue', this line will be skipped.
	i++;         // if execute 'continue', this line will be skipped.
	             // Thus, causes infinite loop because `i++` will not be executed.
}

/*
for (i = 0; i < 10; i++) {
	if (i & 1)
		continue;
	sum += i;     # if execute 'continue', this line will be skipped.
	              # but i++ will be executed as usual.
}*/
```


B.
```c
/* Example of for loop containing a continue statement */
/* Sum even numbers between 0 and 9 */

long sum = 0;
long i = 0;

while(i < 10)
{
	if (i & 1)
		goto i_label;
	sum += i;    // if execute 'continue', this line will be skipped.
	
i_label:
	i++;         // cannot skipped         
}
```

# 3.6.8 Switch Statement
* A ==jump table== is an array where entry i is the address of a code segment implementing the action the program should take when the switch index equals i.
	* The advantage of using a jump table over a long sequence of if-else statements is that the time taken to perform the switch is independent of the number of switch cases.
	* Jump tables are used when there are a number of cases and they span a small range of values.
## Example:
### Original C code:
```c
# rdi - x, rsi - n, rdx - dest
void switch_eg(long x, long n, long *dest)
{
	long val = x;
	switch (n) {
		case 100:
			val *= 13;
			break;
		case 102:
			val += 10;
			/* Fall through */
		case 103:
			val += 11;
			break;
		case 104:
		case 106:
			val *= val;
			break;
		default:
			val = 0;
	}
	*dest = val;
}
```
### Translate to extended C form
```c
void switch_eg_impl(long x, long n, long *dest)
{
	/* Table of code pointers */
	static void *jt[7] = {
		&&loc_A, &&loc_def, &&loc_B, &&loc_C, &&loc_D, &&loc_def, &&loc_D 
	};

	unsigned long index=n- 100;
	long val;
	if (index > 6)    goto loc_def;
	/* Multiway branch */
	
	goto *jt[index]; // Key Step
	
	loc_A: /* Case 100 */
		val = x * 13;
		goto done;
	
	loc_B: /* Case 102 */
		x = x + 10;
	/* Fall through */
	loc_C: /* Case 103 */
		val = x + 11;
		goto done;
	loc_D: /* Cases 104, 106 */
		val = x * x;
		goto done;
	loc_def: /* Default case */
		val = 0;
	done:
		*dest = val;
}
```
* This code makes use of **support provided by gcc for jump tables**, as an extension to the C language.
* The array `jt` contains 7 entries, each of which is the address of a block of code.
* These locations are defined by labels in the code and indicated in the entries in `jt` by code pointers, consisting of the labels prefixed by `&&`.
	* `&&` - in GCC, `&&label` is a way to get the address of a label.
* The compiler first shifts the range to between 0 and 6 by subtracting 100 from n, creating a new program variable that we call `index` in our C version.
	* treating `index` as an unsigned value.
* It can therefore test whether index is outside of the range 0–6 by testing whether it is greater than 6.
* There are 5 distinct locations to jump to, based on the value of index.
* The key step in executing a switch statement is to access a code location through the jump table. This occurs in line 16 in the C code, with a `goto` statement that references the jump table `jt`.
* Our C code declares the jump table as an array of 7 elements, each of which is a pointer to a code location. These elements span values 0–6 of index, corresponding to values 100–106 of n. Observe that the jump table handles duplicated cases by simply having the same code label (loc_D) for entries 4 and 6, and it handles missing cases by using the label for the default case (loc_def) as entries 1 and 5.

### Assembly Code
```z80
# void switch_eg(long x, long n, long *dest)
# x in %rdi, n in %rsi, dest in %rdx

switch_eg:
	subq $100, %rsi   # Compute index = n-100
	cmpq $6, %rsi     # Compare index:6
	ja .L8            # If >, goto loc_def
	jmp *.L4(,%rsi,8) # Goto *jg[index]
.L3:                  # loc_A:
	leaq (%rdi,%rdi,2), %rax # 3*x
	leaq (%rdi,%rax,4), %rdi # val = 13*x
	jmp .L2                  # Goto done
.L5:                         # loc_B:
	addq $10, %rdi           # x = x + 10
.L6:                         # loc_C:
	addq $11, %rdi           # val = x + 11
	jmp .L2                  # Goto done
.L7:                         # loc_D:
	imulq %rdi, %rdi         # val = x * x
	jmp .L2                  # Goto done
.L8:                         # loc_def:
	movl $0, %edi            # val = 0
.L2:                         # done:
	movq %rdi, (%rdx)        # *dest = val
	ret                      # Return
```
* Jump table:
```z80
.section .rodata
.align 8             # Align address to multiple of 8

.L4:
	.quad .L3        # Case 100: loc_A
	.quad .L8        # Case 101: loc_def
	.quad .L5        # Case 102: loc_B
	.quad .L6        # Case 103: loc_C
	.quad .L7        # Case 104: loc_D
	.quad .L8        # Case 105: loc_def
	.quad .L7        # Case 106: loc_D
```
* These declarations state that within the segment of the object-code file called `.rodata`.
* There should be a sequence of seven “quad” (8-byte) words, where the value of each word is given by the instruction address associated with the indicated assembly-code labels.


### Assembly Code in lab

```z80
# void switch_eg(long x, long n, long *dest)
# rdi - x, rsi - n, rdx - dest
        .file   "switch_eg.c"
.text
        .globl  switch_eg
        .type   switch_eg, @function
switch_eg:
.LFB0:
        subq    $100, %rsi
        cmpq    $6, %rsi
        ja      .L8
        leaq    .L4(%rip), %rcx         # rcx = .L4 + rip = .L4 absolute address
        movslq  (%rcx,%rsi,4), %rax     # rax = *(4*rsi + rcx) = *(4*n + .L4 absolute address) = some entry value of jump table
        addq    %rcx, %rax              # rax = rcx + rax = switch case absolute address
        notrack                         # pseudo-instruction, just ignore
		jmp     *%rax                   # jump to the address contained in rax  
.section        .rodata
.L4:                                    # jump table
        .long   .L7-.L4
        .long   .L8-.L4
        .long   .L6-.L4
        .long   .L5-.L4
        .long   .L3-.L4
        .long   .L8-.L4
        .long   .L3-.L4
        .text
.L7:
        leaq    (%rdi,%rdi,2), %rax
        leaq    (%rdi,%rax,4), %rdi
        jmp     .L2
.L6:
        addq    $10, %rdi
.L5:
        addq    $11, %rdi
.L2:
        movq    %rdi, (%rdx)
        ret
.L3:
        imulq   %rdi, %rdi
        jmp     .L2
.L8:                               # done
        movl    $0, %edi
        jmp     .L2
```
* `leaq .L4(%rip), %rcx` - calculate the absolute address of the label `.L4`. Now it's only assembly file so `.L4` is only relative address. After adding current instruction address, the result would be absolute address of `.L4`.
* `movslq` - move Signed Long to Quad.
* **`jmp *%rax`**: The asterisk `*` before the register `%rax` indicates an **indirect jump**. The processor will jump to the address contained in the `%rax` register, rather than to a fixed address.

# Practice Problem 3.30
In the C function that follows, we have omitted the body of the switch statement. In the C code, the case labels did not span a contiguous range, and some cases had multiple labels.
```c
void switch2(short x, short *dest) {
	short val = 0;
	switch (x) {
		.
		.
		.
		Body of switch statement omitted
	}
	*dest = val;
}
```
In compiling the function, gcc generates the assembly code that follows for the initial part of the procedure, with variable `x` in `%rdi`:
```z80
# void switch2(short x, short *dest)
# x in %rdi

switch2:
	addq $2, %rdi
	cmpq $8, %rdi
	ja .L2
	jmp *.L4(,%rdi,8)
```
It generates the following code for the jump table:
```z80
.L4:
	.quad .L9
	.quad .L5
	.quad .L6
	.quad .L7
	.quad .L2
	.quad .L7
	.quad .L8
	.quad .L2
	.quad .L5
```
Based on this information, answer the following questions:

A. What were the values of the case labels in the switch statement?

B. What cases had multiple labels in the C code?

**Solution**:
```z80
# void switch2(short x, short *dest)
# x in %rdi

switch2:
	addq $2, %rdi      # rdi=rdi+2: x+=2
	cmpq $8, %rdi      # compare x,8
	ja .L2             # if x > 8, jump to .L2
	jmp *.L4(,%rdi,8)  # jump to *(.L4 + 8*rdi)
```
We can get the following tips from the reverse-engineering work above:
* .L2 must be the label of default case.
* We need to look into the jump table based on the expression `jmp *.L4(,%rdi,8)`.
	* for sure, x has been already added by 2. So we need to get the original x value by subtracting by 2.
```z80
.L4:
	.quad .L9    # rdi=0: x = -2
	.quad .L5    # rdi=1: x = -1
	.quad .L6    # rdi=2: x = 0
	.quad .L7    # rdi=3: x = 1
	.quad .L2    # rdi=4: x = 2 - default case
	.quad .L7    # rdi=5: x = 3
	.quad .L8    # rdi=6: x = 4
	.quad .L2    # rdi=7: x = 5 - default case
	.quad .L5    # rdi=8: x = 6
```

A.
-2, -1, 0, 1, 3, 4, 6

B.
.L2, .L5, .L7

# Practice Problem 3.31
For a C function switcher with the general structure
```c
void switcher(long a, long b, long c, long *dest)
{
	long val;
	switch(a) {
		case ______ : /* Case A */
			c = ______ ;
			/* Fall through */
		case ______ : /* Case B */
			val = ______ ;
			break;
		case ______ : /* Case C */
		case ______ : /* Case D */
			val = ______ ;
			break;
		case ______ : /* Case E */
			val = ______ ;
			break;
		default:
			val = ______ ;
	}
	*dest = val;
}
```
gcc generates the assembly code and jump table shown in figure below.
Fill in the missing parts of the C code. Except for the ordering of case labels C and D, there is only one way to fit the different cases into the template.
```z80
# void switcher(long a, long b, long c, long *dest) 
# a in %rdi, b in %rsi, c in %rdx, d in %rcx 

switcher: 
	cmpq $7, %rdi 
	ja .L2 
	jmp *.L4(,%rdi,8) 
	.section .rodata 
	
.L7: 
	xorq $15, %rsi 
	movq %rsi, %rdx 
	
.L3: 
	leaq 112(%rdx), %rdi 
	jmp .L6 
	
.L5: 
	leaq (%rdx,%rsi), %rdi 
	salq $2, %rdi 
	jmp .L6 
	
.L2: 
	movq %rsi, %rdi 
	
.L6: 
	movq %rdi, (%rcx) 
	ret
```

```
.L4:
	.quad .L3
	.quad .L2
	.quad .L5
	.quad .L2
	.quad .L6
	.quad .L7
	.quad .L2
	.quad .L5
```

**Solution**:
```z80
# void switcher(long a, long b, long c, long *dest) 
# a in %rdi, b in %rsi, c in %rdx, d in %rcx 

switcher: 
	cmpq $7, %rdi        # compare a,7
	ja .L2               # if a>7, jump to .L2
	jmp *.L4(,%rdi,8)    # jump to *(.L4+8*rdi): *(.L4 + 8a)
	.section .rodata 
	
.L7: 
	xorq $15, %rsi       # rsi=rsi^15: rsi = b^15
	movq %rsi, %rdx      # rdx=rsi: rdx = b^15
	
.L3: 
	leaq 112(%rdx), %rdi  # rdi = rdx+112
	jmp .L6 
	
.L5: 
	leaq (%rdx,%rsi), %rdi # rdi=rdx+rsi: c + b
	salq $2, %rdi          # rdi=rdi<<2: 
	jmp .L6 
	
.L2: 
	movq %rsi, %rdi        # rdi=rsi: b
	
.L6: 
	movq %rdi, (%rcx)      # *(rcx) = rdi: *dest = a
	ret
```

```
.L4:
	.quad .L3 - a = 0
	.quad .L2 - a = 1 - default
	.quad .L5 - a = 2
	.quad .L2 - a = 3 - default
	.quad .L6 - a = 4
	.quad .L7 - a = 5
	.quad .L2 - a = 6 - default
	.quad .L5 - a = 7
```

Fill in the C code:
```c
void switcher(long a, long b, long c, long *dest)
{
	long val;
	switch(a) {
		case 5 : /* Case A */
			c = b^15 ;
			/* Fall through */
		case 0 : /* Case B */
			val = c + 112 ;
			break;
		case 2 : /* Case C */
		case 7 : /* Case D */
			val = (c + b)<<2 ;
			break;
		case 4 : /* Case E */
			val = a ;
			break;
		default:
			val = b ;
	}
	*dest = val;
}
```



























