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
## Floating-point movement instructions
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

