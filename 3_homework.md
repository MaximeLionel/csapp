
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
	imulq %rsi, %rdi   # rdi=rdi*rsi: rdi = x * y
	movq %rsi, %rax    # rax=rsi: rax = y - z
	salq $63, %rax     # rax=rax<<63: rax = (y-z)<<63
	sarq $63, %rax     # rax=rax>>63: rax = ((y-z)<<63)>>63
	xorq %rdi, %rax    # rax=rax^rdi: rax = ((y-z)<<63)>>63 ^ (x*y)
	ret
```
Thus the C code:
```C
long decode2(long x, long y, long z)
{
	return ((y-z)<<63)>>63 ^ (x*y);
}
```