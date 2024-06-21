
# 3.58
For a function with prototype
```C
long decode2(long x, long y, long z);
```

gcc generates the following assembly code:
```
decode2:
	subq %rdx, %rsi
	imulq %rsi, %rdi
	movq %rsi, %rax
	salq $63, %rax
	sarq $63, %rax
	xorq %rdi, %rax
	ret
```
Parameters x, y, and z are passed in registers %rdi, %rsi, and %rdx. The code stores the return value in register %rax.
Write C code for decode2 that will have an effect equivalent to the assembly code shown.

**Solution**:
Analyze assembly code:
```
# long decode2(long x, long y, long z)
# rdi - x, rsi - y, rdx - z
decode2:
	subq %rdx, %rsi    # rsi=rsi-rdx: rsi = y - z
	imulq %rsi, %rdi   # rdi=rdi*rsi: rdi = x * (y - z)
	movq %rsi, %rax    # rax=rsi: rax = y - z
	salq $63, %rax     # rax=rax<<63: rax = (y-z)<<63
	sarq $63, %rax     # rax=rax>>63: rax = ((y-z)<<63)>>63
	xorq %rdi, %rax    # rax=rax^rdi: rax = ((y-z)<<63)>>63 ^ (x * (y - z))
	ret
```
Thus the C code:
```C
long decode2(long x, long y, long z)
{
	return ((y-z)<<63)>>63 ^ (x*(y-z));
}
```

# 3.59
The following code computes the 128-bit product of two 64-bit signed values x and y and stores the result in memory:
```c
typedef __int128 int128_t;

void store_prod(int128_t *dest, int64_t x, int64_t y) {
	*dest = x * (int128_t)y;
}
```
Gcc generates the following assembly code implementing the computation:
```
store_prod:
	movq    %rdx, %rax
	cqto
	movq    %rsi, %rcx
	sarq    $63, %rcx
	imulq   %rax, %rcx
	imulq   %rsi, %rdx
	addq    %rdx, %rcx
	mulq    %rsi
	addq    %rcx, %rdx
	movq    %rax, (%rdi)
	movq    %rdx, 8(%rdi)
	ret
```
This code uses three multiplications for the multiprecision arithmetic required to implement 128-bit arithmetic on a 64-bit machine. Describe the algorithm used to compute the product, and annotate the assembly code to show how it realizes your algorithm. Hint: When extending arguments of x and y to 128 bits, they can be rewritten as $x = 2^{64} \times x_h + x_l$ and $y = 2^{64} \times y_h + y_l$, where $x_h$, $x_l$, $y_h$, and $y_l$ are 64-bit values. Similarly, the 128-bit product can be written as
$p = 2^{64} \times p_h + p_l$, where $p_h$ and $p_l$ are 64-bit values. Show how the code computes the values of $p_h$ and $p_l$ in terms of $x_h$, $x_l$, $y_h$, and $y_l$.

**Solution**:
Analyze the assembly code:
```
# void store_prod(int128_t *dest, int64_t x, int64_t y)
# rdi - dest, rsi - x, rdx - y
store_prod:
	movq    %rdx, %rax     # rax=rdx: rax = y
	cqto                   # convert 64bits in rax to 128bits: 
	                       # rdx = y >> 63
	                       # rax = y
	movq    %rsi, %rcx     # rcx=rsi: rcx = x
	sarq    $63, %rcx      # rcx=rcx>>63: rcx = x>>63 - get the sign bit of x
	imulq   %rax, %rcx     # rcx=rcx*rax: rcx = y * (x>>63)
	imulq   %rsi, %rdx     # rdx=rdx*rsi: rdx = (y >> 63) * x
	addq    %rdx, %rcx     # rcx=rcx+rdx: rcx = (y*(x>>63)) * ((y>>63)*x)
	mulq    %rsi
	addq    %rcx, %rdx
	movq    %rax, (%rdi)
	movq    %rdx, 8(%rdi)
	ret
```