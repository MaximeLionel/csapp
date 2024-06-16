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
* When operating on scalar data, these registers only hold floating-point data, and only the low-order 32 bits (for float) or 64 bits (for double) are used.
* The assembly code refers to the registers by their SSE XMM register names %xmm0–%xmm15, where each XMM register is the low-order 128 bits (16 bytes) of the corresponding YMM register.
