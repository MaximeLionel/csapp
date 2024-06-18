* The **floating-point architecture** for a processor consists of:
	* How floating-point values are stored and accessed - some registers.
	* Instructions that operate on floating-point data.
	* The conventions used for passing floating-point values as arguments to functions and for returning them as results.
	* The conventions for how registers are preserved during function calls.

| Float Architecture Revision                | Register Name | Register Size |
| ------------------------------------------ | ------------- | ------------- |
| MMX - Multi Media Extensions               | MM            | 64 bits       |
| SSE - Streaming SIMD Extensions            | XMM           | 128 bits      |
| AVX - Advanced Vector Extensions (on book) | YMM           | 256 bits      |
| AVX-512 -（Advanced Vector Extensions 512）  | ZMM           | 512 bits      |
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

## Converting between Floating point and Integer
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

## Converting between 2 different floating-point formats
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











