**Integer arithmetic operations**:
![[image-20240315162520655.png|500]]
* The operations are divided into four groups: 
	* load effective address
	* unary (一元运算)  - one operand
	* binary (二元运算) - two operand
	* shifts (位移)
# 3.5.1 Load Effective Address
![[image-20240318095849425.png|500]]
* `leaq` (load effective address) - reads from memory to a register, but it does not reference memory at all. Actually, it only **copies the effective address to the destination**.
* Example0: if register `%rdx` contains value x, then the instruction `leaq 7(%rdx,%rdx,4)`, `%rax` will set register `%rax` to `5x + 7`.
* Example1:
	* consider the following C program:
	```C
		long scale(long x, long y, long z) {
		    long t = x + 4 * y + 12 * z;
		    return t;
		}
	```
	* When compiled, the arithmetic operations of the function are implemented by a sequence of three `leaq` functions:
```
  ; long scale(long x, long y, long z)
  ; x in %rdi, y in %rsi, z in %rdx

scale:
  leaq    (%rdi,%rsi,4), %rax   ; x + 4*y
  leaq    (%rdx,%rdx,2), %rdx   ; z + 2*z = 3*z
  leaq    (%rax,%rdx,4), %rax   ; (x+4*y) + 4*(3*z) = x + 4*y + 12*z
  ret
```
# Practice Problem 3.6
Suppose register `%rbx` holds value p and `%rdx` holds value q. Fill in the table below with formulas indicating the value that will be stored in register `%rax` for each of the given assembly-code instructions:

| Instruction                 | Result |
| --------------------------- | ------ |
| `leaq 9(%rdx), %rax`        |        |
| `leaq (%rdx,%rbx), %rax`    |        |
| `leaq (%rdx,%rbx,3), %rax`  |        |
| `leaq 2(%rbx,%rbx,7), %rax` |        |
| `leaq 0xE(,%rdx,3), %rax`   |        |
| `leaq 6(%rbx,%rdx,7), %rax` |        |


**Solution**:

| Instruction                 | Result  |
| --------------------------- | ------- |
| `leaq 9(%rdx), %rax`        | 9+q     |
| `leaq (%rdx,%rbx), %rax`    | q+p     |
| `leaq (%rdx,%rbx,3), %rax`  | q+3*p   |
| `leaq 2(%rbx,%rbx,7), %rax` | p+7*q+2 |
| `leaq 0xE(,%rdx,3), %rax`   | 14+3*q  |
| `leaq 6(%rbx,%rdx,7), %rax` | 6+p+7*q |

# Practice Problem 3.7
Consider the following code, in which we have omitted the expression being computed:
```C
short scale3(short x, short y, short z) { 
	short t = __________;  
	return t;
}
```
Compiling the actual function with gcc yields the following assembly code:
```
; short scale3(short x, short y, short z)
; x in %rdi, y in %rsi, z in %rdx

scale3:
  leaq (%rsi,%rsi,9), %rbx
  leaq (%rbx,%rdx), %rbx
  leaq (%rbx,%rdi,%rsi), %rbx
  ret
```
Fill in the missing expression in the C code.

**Solution**:
`; short scale3(short x, short y, short z)`
`; x in %rdi, y in %rsi, z in %rdx`
  `leaq (%rsi,%rsi,9), %rbx`: $rbx = y + 9*y = 10*y$
  `leaq (%rbx,%rdx), %rbx`: $rbx = rbx +z=10*y+z$
  `leaq (%rbx,%rdi,%rsi), %rbx`: $rbx = 10*y+z+x*y$
Therefore, we get this:
```C
short scale3(short x, short y, short z) { 
	short t = 10*y+z+x*y;  
	return t;
}
```

# 3.5.2 Unary and Binary Operations
![[image-20240318094815538.png|400]]
* **Unary** operation - with the single operand serving as both source and destination.
	* This operand can be either a register or a memory location.
	* Example: `incq (%rsp)` causes the 8-byte element on the top of the stack to be incremented.


![[image-20240318095802686.png|400]]
* **Binary** operation - the second operand is used as both a source and a destination. 
	* Example: `subq %rax,%rdx` decrements register `%rdx` by the value in `%rax`
		* The first operand can be either an immediate value, a register, or a memory location. 
		* The second can be either a register or a memory location.

# Practice Problem 3.8
Assume the following values are stored at the indicated memory addresses and registers:
![[image-20240318093143070.png|400]]
Fill in the following table showing the effects of the following instructions, in terms of both the register or memory location that will be updated and the resulting value:

| Instruction               | Destination | Value |
| ------------------------- | ----------- | ----- |
| `addq %rcx,(%rax)`        |             |       |
| `subq %rdx,8(%rax)`       |             |       |
| `imulq $16,(%rax,%rdx,8)` |             |       |
| `incq 16(%rax)`           |             |       |
| `decq %rcx`               |             |       |
| `subq %rdx,%rax`          |             |       |
**Solution**:

| Instruction               | Destination | Value            |
| ------------------------- | ----------- | ---------------- |
| `addq %rcx,(%rax)`        | 0x100       | 0xFF+0x1 = 0x100 |
| `subq %rdx,8(%rax)`       | 0x108       | 0xA8             |
| `imulq $16,(%rax,%rdx,8)` | 0x118       | 0x110            |
| `incq 16(%rax)`           | 0x110       | 0x14             |
| `decq %rcx`               | `%rcx`      | 0                |
| `subq %rdx,%rax`          | `%rax`      | 0xFD             |

# 3.5.3 Shift Operations

![[image-20240318094704265.png|600]]
* **Shift** operations - the shift amount is given first and the value to shift is given second;
	* the shift amount can be **an immediate value or with the single-byte register `%cl`**;
		* a 1-byte shift amount would make it possible to encode shift amounts ranging up to $2^8 − 1 = 255$;
* On x86-64, if data values are w bits long, we set low-order m bits of register `%cl`and get $2^m=w$. The higher-order bits are ignored.
	* Example: when register `%cl` has hexadecimal value 0xFF,
		* `salb` would shift by 7;
		* `salw` would shift by 15;
		* `sall` would shift by 31;
		* `salq` would shift by 63.
* `sal` and `shl` have the same effect, filling from the right with zeros;
* `sar` ($>>_A$) performs an arithmetic shift, filling with copies of the sign bit;
* `shr` ($>>_L$)performs a logical shift, filling with zeros;

# Practice Problem 3.9
Suppose we want to generate assembly code for the following C function:
```C
long shift_left4_rightn(long x, long n)
{
	x <<= 4;
	x >>= n;
	return x;
}
```
The code that follows is a portion of the assembly code that performs the actual shifts and leaves the final value in register `%rax`. Two key instructions have been omitted. Parameters x and n are stored in registers `%rdi` and `%rsi`, respectively.
```
  ; long shift_left4_rightn(long x, long n)
  ; x in %rdi, n in %rsi
shift_left4_rightn:
  movq %rdi, %rax    ; Get x
  _______________    ; x <<= 4
  movl %esi, %ecx    ; Get n (4 bytes)
  _______________    ; x >>= n
```
Fill in the missing instructions, following the annotations on the right. The right shift should be performed arithmetically.

**Solution**:

x <<=4: `shlq 4,%rax`
x >>=n: `sarq %cl,%rax`

# 3.5.4 Discussion
* C code
```C
long arith(long x, long y, long z)
{
	long t1 = x ^ y;
	long t2 = z * 48;
	long t3 = t1 & 0x0F0F0F0F;
	long t4 = t2 - t3;
	return t4;
}
```
* Assembly code
```
	; long arith(long x, long y, long z)
	; x in %rdi, y in %rsi, z in %rdx
arith:
	xorq %rsi, %rdi       ; t1 = x ^ y
	leaq (%rdx,%rdx,2),%rax ; 3*z
	salq $4, %rax         ; t2 = 16 * (3*z) = 48*z
	andl $252645135, %edi ; t3 = t1 & 0x0F0F0F0F
	subq %rdi, %rax       ; Return t2 - t3
	ret
```
* Arguments x, y, and z are initially stored in registers `%rdi, %rsi, and %rdx`, respectively. The assembly-code instructions correspond closely with the lines of C source code. 
* `xorq %rsi, %rdi` computes the value of `x^y`. 
* `leaq (%rdx,%rdx,2),%rax` and `salq $4, %rax` compute the expression z*48 by a combination of `leaq` and `shift` instructions. 
* `andl $252645135, %edi` computes the and of t1 and 0x0F0F0F0F. 
* `subq %rdi, %rax` the destination of the subtraction is register `%rax`, this will be the value returned by the function.

# Practice Problem 3.10
Consider the following code, in which we have omitted the expression being computed:
```C
short arith3(short x, short y, short z)
{
	short p1 = ______;
	short p2 = ______;
	short p3 = ______;
	short p4 = ______;
	return p4;
}
```
The portion of the generated assembly code implementing these expressions is as follows:
```
	short arith3(short x, short y, short z)
	x in %rdi, y in %rsi, z in %rdx
arith3:
	orq %rsi, %rdx
	sarq $9, %rdx
	notq %rdx
	movq %rdx, %rax
	subq %rsi, %rax
	ret
```
Based on this assembly code, fill in the missing portions of the C code.

**Solution**:
Firstly, we look into the assembly code:
```z80
	short arith3(short x, short y, short z)
	x in %rdi, y in %rsi, z in %rdx
arith3:
	orq %rsi, %rdx    ; rdx = y | z 
	sarq $9, %rdx     ; rdx = rdx >> 9
	notq %rdx         ; rdx = ~rdx
	movq %rdx, %rax   ; rax = rdx
	subq %rsi, %rax   ; rax = rax - y
	ret
```
* `rdx = y | z`: p1 = y | z
* `rdx = rdx >> 9`: p2 = p1 >> 9
* `rdx = ~rdx`: p3 = ~p2
* `rax = rax - y`: p4 = p3 - y

# Practice Problem 3.11
It is common to find assembly-code lines of the form
`xorq %rcx,%rcx`
in code that was generated from C where no exclusive-or operations were present.
	A. Explain the effect of this particular exclusive-or instruction and what useful operation it implements.
	B. What would be the more straightforward way to express this operation in assembly code?
	C. Compare the number of bytes to encode any two of these three different implementations of the same operation.

**Solution**：
A. it clears `%rcx`;
B. `movq $0, %rcx`
C. 
`48 31 c9                xor    rcx,rcx` - need 3 bytes
`48 c7 c1 00 00 00 00    mov    rcx,0x0` - need 7 bytes
`31 c9                   xor    ecx,ecx` - need 2 bytes
* updates the lower 4 bytes will cause the high-order bytes to be set to zero.
`b9 00 00 00 00          mov    ecx,0x0` - need 5 bytes
* updates the lower 4 bytes will cause the high-order bytes to be set to zero.


# 3.5.5 Special Arithmetic Operations
![[image-20240318165147066.png|600]]
## imulq / mulq operations
* Multiplying two 64-bit signed or unsigned integers can yield a product that requires 128 bits to represent.
* Intel refers to a 16-byte quantity as an **oct** word.
* The pair of registers `%rdx` and `%rax` are viewed as forming a single 128-bit oct word.
* 2 forms of `imulq`:
	1. “twooperand” multiply instruction (`imulq`) - generating a 64-bit product from two 64-bit operands. It implements the operations $*^u_{64}$ and $*^t_{64}$;
	2. “one-operand” multiply instructions - compute the full 128-bit product of two 64-bit values:
		* `mulq`   - for unsigned
		* `imulq` - for signed
		* One argument must be in register `%rax`, and the other is given as the instruction source operand.
		* The product is then stored in registers `%rdx` (high-order 64 bits) and `%rax` (low-order 64 bits).

* C code demonstrates the generation of a 128-bit product of two unsigned 64-bit numbers x and y:
	```C
	#include <inttypes.h>
	
	typedef unsigned __int128 uint128_t;
	void store_uprod(uint128_t *dest, uint64_t x, uint64_t y) {
		*dest=x* (uint128_t) y;
	}
	```
	* 128-bit integers, declared using the name \_\_int128. This code uses a typedef declaration to define data type uint128_t;
	* We compile the code using the cmd below:
	```
		> x86_64-linux-gnu-gcc -Og -S P226.c
	```
		![[image-20240319125729269.png]]
	* We get the full asm code:
	```
		.file	"P226.c"
	.text
	.globl	store_uprod
	.type	store_uprod, @function
	store_uprod:
		.LFB0:
			.cfi_startproc
			endbr64
			movq	%rsi, %rax
			mulq	%rdx
			movq	%rax, (%rdi)
			movq	%rdx, 8(%rdi)
			ret
			.cfi_endproc
		.LFE0:
			.size	store_uprod, .-store_uprod
			.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
			.section	.note.GNU-stack,"",@progbits
			.section	.note.gnu.property,"a"
			.align 8
			.long	1f - 0f
			.long	4f - 1f
			.long	5
		0:
			.string	"GNU"
		1:
			.align 8
			.long	0xc0000002
			.long	3f - 2f
		2:
			.long	0x3
		3:
			.align 8
		4:
	```
	* Then we get the core code:
	```
	; void store_uprod(uint128_t *dest, uint64_t x, uint64_t y)
	; dest in %rdi, x in %rsi, y in %rdx
	store_uprod:
		endbr64
		movq	%rsi, %rax
		mulq	%rdx
		movq	%rax, (%rdi)
		movq	%rdx, 8(%rdi)
		ret
	```
	* We analyze the asm code:
		* `movq	%rsi, %rax`: %rax = x
		* `mulq	%rdx`: x\*y
		* `movq	%rax, (%rdi)`: \*dest = low64bits of x\*y - low-order 8 bytes
		* `movq	%rdx, 8(%rdi)`: \*(dest+8) = high64bits of x\*y - high-order 8 bytes

## idivq / divq operations
* idivq:
	* dividend (被除数): 
		* `%rdx` - high 64bits
		* `%rax` - low 64bits
	* divisor (除数): given as the instruction operand;
	* quotient (商) - `%rax`;
	* remainder (余数) - `%rdx`;
* If the dividend is a 64-bit value: 
	* the value should be stored in register `%rax`.
	* The bits of `%rdx` should then be set to either all zeros (unsigned arithmetic) or the sign bit of `%rax` (signed arithmetic); - can be done by cqto instruction.

## cqto operation
* cqto - no operands. it implicitly reads the sign bit from `%rax` and copies it across all of `%rdx`.

## Example
* C code of division in x86-64:
```C
void remdiv(long x, long y, long *qp, long *rp) {
	long q = x/y;
	long r = x%y;
	*qp = q;
	*rp = r;
}
```
* after `x86_64-linux-gnu-gcc -Og -S P227.c`, we get:
```
; void remdiv(long x, long y, long *qp, long *rp)
; x in %rdi, y in %rsi, qp in %rdx, rp in %rcx
	endbr64
	movq	%rdi, %rax
	movq	%rdx, %r8
	cqto
	idivq	%rsi
	movq	%rax, (%r8)
	movq	%rdx, (%rcx)
	ret
```
* `movq	%rdi, %rax`: `%rax` = x
* `movq	%rdx, %r8`: `%r8` = qp
* `cqto`: copy the sign bit from `%rax` to all of `%rdx`.
* `idivq	%rsi`: x/y
* `movq	%rax, (%r8)`: \*qp = x/y
* `movq	%rdx, (%rcx)`: \*rp = x%y

# Practice Problem 3.12
Consider the following function for computing the quotient and remainder of two unsigned 64-bit numbers:
```C
void uremdiv(unsigned long x, unsigned long y, unsigned long *qp, unsigned long *rp) 
{
	unsigned long q = x/y;
	unsigned long r = x%y;
	*qp = q;
	*rp = r;
}
```
Modify the assembly code shown for signed division to implement this function.

**Solution**:
Firstly, we get the asm code for current C code:
```
; void remdiv(long x, long y, long *qp, long *rp)
; x in %rdi, y in %rsi, qp in %rdx, rp in %rcx
	endbr64
	movq	%rdi, %rax
	movq	%rdx, %r8
	cqto                  ; replace by `movq $0,%rdx` or `movl $0,%edx`
	idivq	%rsi          ; replace by `divq %rsi`
	movq	%rax, (%r8)
	movq	%rdx, (%rcx)
	ret
```














