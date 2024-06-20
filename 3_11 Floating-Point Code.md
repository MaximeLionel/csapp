* The **floating-point architecture** for a processor consists of:
	* How floating-point values are stored and accessed - some registers.
	* Instructions that operate on floating-point data.
	* The conventions used for passing floating-point values as arguments to functions and for returning them as results.
	* The conventions for how registers are preserved during function calls.

| Float Architecture Revision                    | Register Name | Register Size |
| ---------------------------------------------- | ------------- | ------------- |
| MMX - Multi Media Extensions                   | MM            | 64 bits       |
| SSE - Streaming SIMD Extensions                | XMM           | 128 bits      |
| ==AVX - Advanced Vector Extensions (on book)== | YMM           | 256 bits      |
| AVX-512 -（Advanced Vector Extensions 512）      | ZMM           | 512 bits      |
* Gcc will generate AVX2 code when given the command-line parameter `-mavx2`.
* The scalar AVX instructions (标量AVX指令) intend for operating on entire data vectors arise.
	* Scalar data - a **single value or element**, as opposed to a collection of values or elements (such as arrays, vectors, or matrices). Scalar data types represent individual quantities and are the simplest form of data in programming and computer science.

![[image-20240616115619888.png|500]]
* AVX floating-point architecture allows data to be stored in 16 YMM registers, named `%ymm0–%ymm15`. 
	* Each YMM register is 256 bits (32 bytes) long.
* Dealing with scalar data:
	* When operating on scalar data, these registers only hold floating-point data, and only the low-order 32 bits (for float) or 64 bits (for double) are used.
	* The assembly code refers to the registers by their SSE XMM register names %xmm0–%xmm15, where each XMM register is the low-order 128 bits (16 bytes) of the corresponding YMM register.

# 3.11.1 Floating-Point Movement and Conversion Operations
## Floating-point Movement Instructions
![[image-20240616135320260.png|500]]
* These operations above transfer values between memory and XMM registers, as well as between pairs of XMM registers.
### vmovss and vmovsd
* Memory and XMM registers data movement - The first 4 instructions (`vmovss`,`vmovsd`) that reference memory above are **scalar instructions**, meaning that they operate on individual, rather than packed, data values.
	* The data are held either in memory (indicated in the table as $M_{32}$ and $M_{64}$) or in XMM registers (shown in the table as X).
	* These instructions will work correctly regardless of the alignment of data, although the code optimization guidelines recommend that 32-bit memory data satisfy a 4-byte alignment and that 64-bit data satisfy an 8-byte alignment.
	* Memory references are specified in the same way as for the integer `mov` instructions, with all of the different possible combinations of displacement, base register, index register, and scaling factor.
### vmovaps and vmovapd
* `vmovaps` for single precision and `vmovapd` for double-precision values.
* When used for XMM registers and XMM registers data movements:
	* For these cases, whether the program copies the entire register or just the low-order value affects neither the program functionality nor the execution speed, and so using these instructions rather than ones specific to scalar data makes no real difference.
* When used for XMM registers and memory data movements:
	* The letter ‘a’ in these instruction names stands for “aligned”.
	* When used to read and write memory, they will cause an exception if the address does not satisfy a 16-byte alignment.

### Example
```c
float float_mov(float v1, float *src, float *dst) {
	float v2 = *src;
	*dst = v1;
	return v2;
}
```
* Reversed assembly code:
	```
	# float float_mov(float v1, float *src, float *dst)
	# v1 in %xmm0, src in %rdi, dst in %rsi
	
	float_mov:
		vmovaps %xmm0, %xmm1       # Copy v1
		vmovss (%rdi), %xmm0       # Read v2 from src
		vmovss %xmm1, (%rsi)       # Write v1 to dst
		ret                        # Return v2 in %xmm0
	```
	* `vmovaps` - copy data from xmm0 to xmm1;
	* `vmovss` - copy data from memory (M(rdi)) to an xmm0 register and from an xmm1 register to memory (M(rsi)).

## Converting between Floating/Double point and Integer
* Convert from a floating-point value read from either an XMM register or memory and write the result to a general-purpose register (e.g., %rax, %ebx, etc.):
	![[image-20240618093046427.png|600]]
	* When converting floating-point values to integers, they perform ==truncation==, rounding values toward zero.
* Convert from integer to floating point:
	![[image-20240618093310969.png|600]]
	* Three-operand format, with two sources and a destination.
	* These instructions above convert from the data type of the first source to the data type of the destination. The second source value has no effect on the low-order bytes of the result.
	* For our purposes, we can ignore the second operand, since its value only affects the upper bytes of the result.
	* The destination must be an XMM register. 
	* In common usage, both the second source and the destination operands are identical.
	* Example:
		```
		vcvtsi2sdq %rax, %xmm1, %xmm1
		```
		* Reads a long integer from register `%rax`. 
		* Converts it to data type double. 
		* Stores the result in the lower bytes of XMM register `%xmm1`.

## Converting from Single Precision to Double Precision
* Logically, we use `vcvtss2sd` - convert a single-precision value to a double-precision value:
	* suppose the low-order 4 bytes of %xmm0 hold a single-precision value
		```
		vcvtss2sd %xmm0, %xmm0, %xmm0
		```
		* convert a single-precision value in `%xmm0` to a double-precision value and store the result in the lower 8 bytes of register `%xmm0`.
* In reality, GCC generate code below:
	```
	# Conversion from single to double precision
	vunpcklps %xmm0, %xmm0, %xmm0             # Replicate first vector element
	vcvtps2pd %xmm0, %xmm0                    # Convert two vector elements to double
	```
	* `vunpcklps` instruction is normally used to interleave the values in two XMM registers and store them in a third.
		* Example - if register `%src0` contains words `[s3, s2, s1, s0]` and the other register `src1` contains words `[d3, d2, d1, d0]`, then after `vunpcklps %src0, %src1, %dest`, the value of the destination register `dest` will be `[s1, d1, s0, d0]`.
		* So, if the original register `%xmm0` held values `[x3, x2, x1, x0]`, then after `vunpcklps %xmm0, %xmm0, %xmm0`, the instruction will update the register to hold values `[x1, x1, x0, x0]`.
			* Please notice that x0,x1,x2 and x3 are all single precision value.
	* `vcvtps2pd` instruction expands the two low-order singleprecision values in the source XMM register to be the two double-precision values in the destination XMM register.
		* If the original register `%xmm0` held values `[x3, x2, x1, x0]`, then after `vcvtps2pd %xmm0, %xmm0`, `%xmm0` will be `[dx0,dx0]`, while `dx0` is the result of converting `x` to double precision.
	* Therefore, suppose `%xmm0` held values `[x3, x2, x1, x0]`, the `%xmm0` value will update as below:
		```
		# Conversion from single to double precision
		vunpcklps %xmm0, %xmm0, %xmm0             # xmm0 = [x1,x1,x0,x0]
		vcvtps2pd %xmm0, %xmm0                    # xmm0 = [dx0,dx0]
		```
	* The single precision value `x0` has been converted to double precision value `dx0` successfully.

## Converting from Double Precision to Single Precision
* Logically, we use `vcvtsd2ss` - convert a double-precision value to a single-precision value:
```
vcvtsd2ss %xmm0, %xmm0, %xmm0
```
* In reality, GCC generate code below:
	```
	# Conversion from double to single precision
	vmovddup %xmm0, %xmm0       # Replicate first vector element
	vcvtpd2psx %xmm0, %xmm0     # Convert two vector elements to single
	```
	* Suppose register `%xmm0` holding two double-precision values `[dx1, dx0]`. 
	* `vmovddup %xmm0, %xmm0` - set `%xmm0` to `[dx0, dx0]`. 
	* `vcvtpd2psx %xmm0, %xmm0` - convert to single precision, pack them into the low-order half of the register, and set the upper half to 0, yielding a result `[0.0, 0.0, sx0, sx0]`.
* Example:
	* C code:
	```c
	double fcvt(int i, float *fp, double *dp, long *lp)
	{
		float f = *fp; double d = *dp; long l = *lp;
		*lp = (long) d;
		*fp = (float) i;
		*dp = (double) l;
		return (double) f;
	}
	```
	* Assembly code:
```
# double fcvt(int i, float *fp, double *dp, long *lp)
# i in %edi, fp in %rsi, dp in %rdx, lp in %rcx
fcvt:
	vmovss (%rsi), %xmm0       # xmm0=M(rsi):       Get f = *fp
	movq (%rcx), %rax          # rax=M(rcx):        Get l = *lp
	vcvttsd2siq (%rdx), %r8    # r8=(long)M(rdx):   Get d = *dp and convert to long
	movq %r8, (%rcx)           # M(rcx)=r8:         Store at lp
	vcvtsi2ss %edi, %xmm1, %xmm1   # xmm1=(float)edi:    Convert i to float
	vmovss %xmm1, (%rsi)       # M(rsi)=xmm1:       Store at fp
	vcvtsi2sdq %rax, %xmm1, %xmm1  # xmm1=(double)rax:   Convert l to double
	vmovsd %xmm1, (%rdx)       # M(rdx)=xmm1:       Store at dp

    # The following two instructions convert f to double
	vunpcklps %xmm0, %xmm0, %xmm0
	vcvtps2pd %xmm0, %xmm0
	ret                        # Return f in xmm0
```

# Practice Problem 3.50
For the following C code, the expressions val1 – val4 all map to the program values i, f, d, and l:
```c
double fcvt2(int *ip, float *fp, double *dp, long l)
{
	int i = *ip; float f = *fp; double d = *dp;
	*ip = (int) val1;
	*fp = (float) val2;
	*dp = (double) val3;
	return (double) val4;
}
```
Determine the mapping, based on the following x86-64 code for the function:
```
# double fcvt2(int *ip, float *fp, double *dp, long l)
# ip in %rdi, fp in %rsi, dp in %rdx, l in %rcx
# Result returned in %xmm0

fcvt2:
	movl (%rdi), %eax
	vmovss (%rsi), %xmm0
	vcvttsd2si (%rdx), %r8d
	movl %r8d, (%rdi)
	vcvtsi2ss %eax, %xmm1, %xmm1
	vmovss %xmm1, (%rsi)
	vcvtsi2sdq %rcx, %xmm1, %xmm1
	vmovsd %xmm1, (%rdx)
	vunpcklps %xmm0, %xmm0, %xmm0
	vcvtps2pd %xmm0, %xmm0
	ret
```

**Solution**:
Firstly, go though all asm instructions:
```
# double fcvt2(int *ip, float *fp, double *dp, long l)
# ip in %rdi, fp in %rsi, dp in %rdx, l in %rcx
# Result returned in %xmm0

fcvt2:
	movl (%rdi), %eax                # eax=M(rdi):  eax = *ip
	vmovss (%rsi), %xmm0             # xmm0=M(rsi): xmm0= *fp
	vcvttsd2si (%rdx), %r8d          # r8d=M(rdx):  r8d = (int)(*dp)
	movl %r8d, (%rdi)                # M(rdi)=r8d:  *ip = r8d
	vcvtsi2ss %eax, %xmm1, %xmm1     # xmm1=(float)eax=(float)(*ip)
	vmovss %xmm1, (%rsi)             # M(rsi)=xmm1: (*fp) = xmm1 = (float)(*ip)
	vcvtsi2sdq %rcx, %xmm1, %xmm1    # xmm1=(double)rcx: xmm1=(double)l
	vmovsd %xmm1, (%rdx)             # M(rdx)=xmm1: (*dp)=xmm1=(double)l
	vunpcklps %xmm0, %xmm0, %xmm0    # convert float in xmm0 to double
	vcvtps2pd %xmm0, %xmm0
	ret
```

Secondly, 
* val1
	From `*ip = (int) val1` , we need to find out `*ip`.
	From line 8 and 9, `*ip` is from `*dp` by converting to `int`.
	So `val1 = d`.
* val2
	From `*fp = (float) val2`, we need to find out `*fp`.
	From line 11, `*fp` is from `*ip` by converting to `float`.
	So `val2 = i`.
* val3
	From `*dp = (double) val3`, we need to find out `*dp`.
	From line 13, `*dp` is from `l` by converting to `double`.
	So `val3 = d`.
* val4
	The funtion returns `double` type, so the return value is stored in `xmm0`. From line 7, `xmm0` is from `f`.
	So `val4 = f`.
	
# Practice Problem 3.51
The following C function converts an argument of type `src_t` to a return value of type `dst_t`, where these two types are defined using typedef:
```c
dest_t cvt(src_t x)
{
	dest_t y = (dest_t) x;
	return y;
}
```
For execution on x86-64, assume that argument x is either in %xmm0 or in the appropriately named portion of register `%rdi` (i.e., `%rdi` or `%edi`). One or two instructions are to be used to perform the type conversion and to copy the value to the appropriately named portion of register `%rax` (integer result) or `%xmm0` (floating-point result). Show the instruction(s), including the source and destination registers.

| $T_x$  | $T_y$  | Instructions             |
| ------ | ------ | ------------------------ |
| long   | double | `vcvtsi2sdq %rdi, %xmm0` |
| double | int    |                          |
| double | float  |                          |
| long   | float  |                          |
| float  | long   |                          |
**Solution**:

| $T_x$  | $T_y$  | Instructions                                                                                                  |
| ------ | ------ | ------------------------------------------------------------------------------------------------------------- |
| long   | double | `vcvtsi2sdq %rdi, %xmm0`                                                                                      |
| double | int    | `vcvttsd2si %xmm0, %eax`                                                                                      |
| double | float  | `vcvtsd2ss %xmm0, %xmm0, %xmm0`<br>In gcc, it's like:<br>`vmovddup %xmm0, %xmm0      vcvtpd2psx %xmm0, %xmm0` |
| long   | float  | `vcvtsi2ssq %rdi, %xmm0, %xmm0`                                                                               |
| float  | long   | `vcvttss2siq %xmm0, %rax`                                                                                     |

# 3.11.2 Floating-Point Code in Procedures
* With x86-64, the XMM registers are used for passing floating-point arguments to functions and for returning floating-point values from them.
* The following conventions are observed:
	* Up to eight floating-point arguments can be passed in XMM registers `%xmm0–%xmm7`.
	* Additional floating-point arguments can be passed on the **stack**.
	* A function that returns a floating-point value does so in register `%xmm0`.
	* All XMM registers are caller saved.
	* When a function contains a combination of pointer, integer, and floating point arguments, the pointers and integers are passed in general-purpose registers, while the floating-point values are passed in XMM registers.
* Examples:
	```c
	double f1(int x, double y, long z);
	```
	* x in `%edi`, y in `%xmm0`, and z in `%rsi`.
	```c
	double f2(double y, int x, long z);
	```
	* y in `%xmm0`, x in `%edi`, and z in `%rsi`. Same as above.
	```c
	double f1(float x, double *y, long *z);
	```
	* x in `%xmm0`, y in `%rdi`, and z in `%rsi`.

# Practice Problem 3.52
For each of the following function declarations, determine the register assignments for the arguments:
A. double g1(double a, long b, float c, int d);

B. double g2(int a, double \*b, float \*c, long d);

C. double g3(double \*a, double b, int c, float d);

D. double g4(float a, int \*b, float c, double d);

**Solution**:
A.
```c
double g1(double a, long b, float c, int d);
a - %xmm0
b - %rdi
c - %xmm1
d - %esi
```
B.
```c
double g2(int a, double *b, float *c, long d);
a - %edi
b - %rsi
c - %rcx
d - %rdx
```
C.
```c
double g3(double *a, double b, int c, float d);
a - %rdi
b - %xmm0
c - %esi
d - %xmm1
```
D.
```c
double g4(float a, int *b, float c, double d);
a - %xmm0
b - %rdi
c - %xmm1
d - %xmm2
```

# 3.11.3 Floating-Point Arithmetic Operations
* A set of scalar AVX2 floating-point instructions that perform arithmetic operations:
	![[image-20240619154607449.png|400]]
	* $S_1$ can be either an XMM register or a memory location.
	* $S_2$ and $D$ must be XMM registers.
	* Each operation has an instruction for single precision and an instruction for double precision. 
	* The result is stored in the destination register.
* Example:
	```c
	double funct(double a, float x, double b, int i)
	{
		return a*x - b/i;
	}
	```
	* Assembly code on book:
		![[image-20240619155030256.png|400]]
		![[image-20240619155051303.png|400]]
		* a, x, and b are passed in XMM registers `%xmm0–%xmm2`, while i is passed in register `%edi`.
		* lines 2–3: standard two-instruction sequence is used to convert argument x to double.
		* line 5: convert argument i to double.
		* Return in register `%xmm0`.
	* In real life, it's like:
		* AXV - Compile in avx code with `-mavx` option: `gcc -mavx -Og -fno-stack-protector -S funct.c -o funct_avx.s`
		```
		vcvtss2sd       %xmm1, %xmm1, %xmm1      # convert xmm1 from float to double
		vmulsd  %xmm0, %xmm1, %xmm1              # xmm1=xmm0*xmm1: xmm1=a*x
		vxorps  %xmm0, %xmm0, %xmm0              # clear xmm0
		vcvtsi2sdl      %edi, %xmm0, %xmm0       # convert edi from int to double, store it in xmm0
		vdivsd  %xmm0, %xmm2, %xmm2              # xmm2=xmm2/xmm0: xmm2=b/(double)i
		vsubsd  %xmm2, %xmm1, %xmm0              # xmm0=xmm1-xmm2: xmm0=a*x-b/i
		ret
		```
		* SSE2 - Compile in sse2 code with `-mavx` option: `gcc -msse2 -Og -fno-stack-protector -S funct.c -o funct_sse2.s`
		```
		cvtss2sd        %xmm1, %xmm1       # xmm1=(double)x
		mulsd   %xmm0, %xmm1               # xmm1=a*x
		pxor    %xmm0, %xmm0               # clear xmm0
		cvtsi2sdl       %edi, %xmm0        # xmm0=(double)i
		divsd   %xmm0, %xmm2               # xmm2=b/(double)i
		subsd   %xmm2, %xmm1               # xmm1=a*x-b/i
		movapd  %xmm1, %xmm0               # xmm0=a*x-b/i
		ret
		```

# Practice Problem 3.53
For the following C function, the types of the four arguments are defined by typedef:
```c
double funct1(arg1_t p, arg2_t q, arg3_t r, arg4_t s)
{
	return p/(q+r) - s;
}
```

When compiled, gcc generates the following code:
```
# double funct1(arg1_t p, arg2_t q, arg3_t r, arg4_t s)

funct1:
	vcvtsi2ssq      %rsi, %xmm2, %xmm2
	vaddss          %xmm0, %xmm2, %xmm0
	vcvtsi2ss       %edi, %xmm2, %xmm2
	vdivss          %xmm0, %xmm2, %xmm0
	vunpcklps       %xmm0, %xmm0, %xmm0
	vcvtps2pd       %xmm0, %xmm0
	vsubsd          %xmm1, %xmm0, %xmm0
	ret
```
Determine the possible combinations of types of the four arguments (there may be more than one).

**Solution**:
```
# double funct1(arg1_t p, arg2_t q, arg3_t r, arg4_t s)

funct1:
	vcvtsi2ssq      %rsi, %xmm2, %xmm2
	vaddss          %xmm0, %xmm2, %xmm0
	vcvtsi2ss       %edi, %xmm2, %xmm2
	vdivss          %xmm0, %xmm2, %xmm0
	vunpcklps       %xmm0, %xmm0, %xmm0
	vcvtps2pd       %xmm0, %xmm0
	vsubsd          %xmm1, %xmm0, %xmm0
	ret
```









