* Arrays in C are one means of aggregating scalar data into larger data types. 
* C uses a particularly simple implementation of arrays, and hence the translation into machine code is fairly straightforward.

# 3.8.1 Basic Principles
* For data type T and integer constant N, consider a declaration of the form:
	```c
	T A[N];
	```
	* 2 effects of the declaration:
		* Allocates a contiguous region of $L \times N$ bytes in memory, where L is the size (in bytes) of data type T.
		* Introduces an identifier $A$ that can be used as a pointer to the beginning of the array.
	* The value of this pointer will be $x_A$.
	* The array elements can be accessed using an integer index ranging between 0 and N−1.
	* Array element i will be stored at address $x_A + L\times i$.
## Example - array declaration
* Consider the declaration below:
	```c
	char A[12];
	char* B[8];
	int C[6];
	double* D[5];
	```
* The declarations will generate arrays with the following parameters:
	![[3_8 Array Allocation and Access.assets/image-20240529152558409.png|400]]

## Example - memory referencing of array
* suppose `E` is an array of values of type `int` and we wish to evaluate `E[i]`, where the address of `E` is stored in register `%rdx` and `i` is stored in register `%rcx`.
```
movl (%rdx,%rcx,4),%eax
```
* The instruction will perform the address computation $x_E + 4i$, read that memory location, and copy the result to register `%eax`.

# Practice Problem 3.36
Consider the following declarations:
```c
int P[5];
short Q[2];
int **R[9];
double *S[10];
short *T[2];
```
Fill in the following table describing the element size, the total size, and the address of element i for each of these arrays.

| Array | Element Size | Total Size | Start Address | Element i |
| :---: | :----------: | :--------: | :-----------: | :-------: |
|   P   |              |            |     $x_P$     |           |
|   Q   |              |            |     $x_Q$     |           |
|   R   |              |            |     $x_R$     |           |
|   S   |              |            |     $x_S$     |           |
|   T   |              |            |     $x_T$     |           |

**Solution**:

| Array | Element Size | Total Size | Start Address | Element i |
| :---: | :----------: | :--------: | :-----------: | :-------: |
|   P   |      4       |     20     |     $x_P$     | $x_P+4i$  |
|   Q   |      2       |     4      |     $x_Q$     | $x_Q+2i$  |
|   R   |      8       |     72     |     $x_R$     | $x_R+8i$  |
|   S   |      8       |     80     |     $x_S$     | $x_S+8i$  |
|   T   |      8       |     16     |     $x_T$     | $x_T+8i$  |

# 3.8.2 Pointer Arithmetic
* If $p$ is a pointer to data of type $T$ , and the value of $p$ is $x_p$, then the expression $p+i$ has value $x_p + L\times i$, where $L$ is the size of data type $T$.
* The unary operators `&` and `*` allow the generation and dereferencing of pointers.
* Example:
	* For an expression Expr denoting some object, &Expr is a pointer giving the address of the object.
	* For an expression AExpr denoting an address, \*AExpr gives the value at that address.
	* The expressions Expr and \*&Expr are therefore equivalent.
	* The array `A[i]` is identical to the expression `*(A+i)`.
* Suppose the starting address of integer array E and integer index i are stored in registers `%rdx` and `%rcx`:
	![[image-20240531235948114.png|500]]
To correct:

| &E[i]-E | 4*i | leaq (0, %rcx, 4), %rax |
| ------- | --- | ----------------------- |


# Practice Problem 3.37
Suppose $x_P$, the address of short integer array P, and long integer index i are stored in registers `%rdx` and `%rcx`, respectively. For each of the following expressions, give its type, a formula for its value, and an assembly-code implementation. The result should be stored in register `%rax` if it is a pointer and register element `%ax` if it has data type short.

| Expression | Type | Value | Assembly Code |
| :--------: | ---- | ----- | ------------- |
|   `P[1]`   |      |       |               |
|  `P+3+i`   |      |       |               |
| `P[i*6-5]` |      |       |               |
|   `P[2]`   |      |       |               |
| `&P[i+2]`  |      |       |               |
**Solution**:

| Expression | Type   | Value            | Assembly Code              |
| :--------: | ------ | ---------------- | -------------------------- |
|    P[1]    | short  | $M[x_p+2]$       | movw 2(%rdx), %ax          |
|   P+3+i    | short* | $x_p+3*2+2*i$    | leaq 6(%rdx,%rcx,2),%rax   |
|  P[i*6-5]  | short  | $M[x_p+12*i-10]$ | movw -10(%rdx,%rcx,12),%ax |
|    P[2]    | short  | $M[x_p+4]$       | movw 4(%rdx), %ax          |
|  &P[i+2]   | short* | $x_p+2*i+4$      | leaq 4(%rdx,%rcx,2),%rax   |

# 3.8.3 Nested Arrays
* A multidimensional array declared as:
	```c
	T D[R][C];
	```
* Array element `D[i][j]` is at memory address:
	$\&D[i][j] = x_D + L(C . i + j)$
	* $L$ is the size of data type T in bytes.

## Example:
```c
int A[5][3];
```
equals to:
```c
typedef int row3_t[3];
row3_t A[5];
```
* Interpretation:
	* `typedef int row3_t[3]` defines `row3_t` as a new name for an array of 3 integers (`int[3]`).
	* `row3_t` can be used as a type that represents an array of 3 integers.
	* `row3_t A[5]` declares `A` as an array of 5 elements, where each element is of type `row3_t`.
	* Array A contains 5 elements (`row3_t`), each requiring 12 bytes (`row3_t[3]`) to store the three integers. 
	* The total array size is then $4 \times 5 \times 3 = 60$ bytes.
* Array A can also be viewed as a two-dimensional array with 5 rows and 3 columns, referenced as `A[0][0]` through `A[4][2]`.
	* The array elements are ordered in memory in row-major order, meaning all elements of row 0, which can be written `A[0]`, followed by all elements of row 1 (`A[1]`), and so on.
		![[image-20240601100306237.png|150]]
	* This ordering is a consequence of our nested declaration. Viewing A as an array of 5 elements, each of which is an array of three int’s, we first have `A[0]`, followed by `A[1]`, and so on.
* Suppose $x_A$, i, and j are in registers `%rdi`, `%rsi`, and `%rdx`:
	```
	# A in %rdi, i in %rsi, and j in %rdx
	leaq (%rsi,%rsi,2), %rax.     # Compute 3i
	leaq (%rdi,%rax,4), %rax      # Compute xA + 12i
	movl (%rax,%rdx,4), %eax      # Read from M[xA + 12i + 4]
	```
* The code computes the element’s address as $x_A + 12i + 4j = x_A + 4(3i + j)$.

# Practice Problem 3.38
Consider the following source code, where M and N are constants declared with `#define`:
```c
long P[M][N];
long Q[N][M];

long sum_element(long i, long j) {
	return P[i][j] + Q[j][i];
}
```
In compiling this program, gcc generates the following assembly code:
```z80
# long sum_element(long i, long j)
# i in %rdi, j in %rsi

sum_element:
	leaq 0(,%rdi,8), %rdx
	subq %rdi, %rdx
	addq %rsi, %rdx
	leaq (%rsi,%rsi,4), %rax
	addq %rax, %rdi
	movq Q(,%rdi,8), %rax
	addq P(,%rdx,8), %rax
	ret
```
Use your reverse engineering skills to determine the values of M and N based on this assembly code.

**Solution**:
* First, we interpret the assembly code:
```z80
# long sum_element(long i, long j)
# i in %rdi, j in %rsi

sum_element:
	leaq 0(,%rdi,8), %rdx      # rdx=0+8*rdi: rdx=8i
	subq %rdi, %rdx            # rdx=rdx-rdi: rdx=8i-i=7i
	addq %rsi, %rdx            # rdx=rdx+rsi: rdx=7i+j
	leaq (%rsi,%rsi,4), %rax   # rax=rsi+4*rsi=5*rsi: rax=5j
	addq %rax, %rdi            # rdi=rdi+rax: rdi=i+5j
	movq Q(,%rdi,8), %rax      # rax=*(Q+8*rdi): rax=M(Q+8(i+5j))=M(Q+8i+40j)
	addq P(,%rdx,8), %rax      # rax=rax+M(P+8*rdx): rax=M(Q+8i+40j)+M(P+56i+8j)
	ret
```
* Secondly, we analyze the C code:
	`long P[M][N];`
	`long Q[N][M];`
	`P[i][j] + Q[j][i] = *(P + 8*i*N + 8*j) + *(Q + 8*j*M + 8*i)`
* Finally, we get `N = 7, M = 5`

# 3.8.4 Fixed-Size Arrays
* The C compiler is able to make many optimizations for code operating on multidimensional arrays of fixed size, while we use `-O1` for testing.
## Differences between `-Og` and `-O1`
In GCC, `-Og` and `-O1` are both optimization levels, but they serve different purposes and enable different sets of optimizations. Here are the key differences between `-Og` and `-O1`:
### `-O1` Optimization Level
- **Purpose:** `-O1` is intended to provide a moderate level of optimization, improving the performance and size of the generated code without significantly increasing compilation time.
- **Optimizations Enabled:** `-O1` enables a range of optimizations that improve execution speed and reduce code size. Some of these optimizations include:
    - Dead code elimination
    - Common subexpression elimination
    - Basic inlining of functions
    - Simple loop optimizations
    - Constant propagation
    - Basic block reordering
- **Usage Scenario:** `-O1` is suitable for situations where you want better performance than unoptimized code (`-O0`) but do not want to wait for the longer compile times associated with higher optimization levels like `-O2` or `-O3`. It strikes a balance between optimization and compilation time.

### `-Og` Optimization Level
- **Purpose:** `-Og` is designed specifically for ==debugging==. It provides a good level of optimization while ensuring that debugging information remains accurate and the debugging experience is not compromised.
- **Optimizations Enabled:** `-Og` enables optimizations that do not interfere with debugging. The goal is to make the code run faster while still being easy to debug. The optimizations typically enabled include:
    - Minimal inlining of functions
    - Local variable optimization
    - Basic optimizations that do not reorder code significantly
- **Usage Scenario:** `-Og` is ideal during the development and debugging phase. It allows you to debug your program with accurate variable tracking and line number information, making it easier to diagnose issues without dealing with heavily optimized and possibly harder-to-debug code.

### Key Differences
- **Debugging Experience:**
    - `-Og` focuses on preserving the debugging experience by enabling only those optimizations that do not interfere with the ability to debug the code effectively.
    - `-O1` applies more aggressive optimizations that can sometimes make debugging harder.
- **Performance and Code Size:**
    - `-O1` generally provides better performance and smaller code size compared to `-Og` because it enables a broader range of optimizations.
    - `-Og` provides less optimization compared to `-O1`, focusing more on maintaining debuggability.

### When to Use Which
- Use `-Og` when you are actively **developing and debugging your code**. It allows you to catch bugs more easily without sacrificing too much performance.
- Use `-O1` when you need a **reasonably optimized build** that performs better than no optimization (`-O0`) but do not need the full debugging capabilities of `-Og`.

### Example
If you are compiling a C program and want to optimize for debugging:
`gcc -Og -g -o myprogram myprogram.c`
If you are compiling for a slightly optimized release build where debugging is not a priority:
`gcc -O1 -o myprogram myprogram.c`
In summary, `-Og` is tailored for development and debugging, ensuring the code remains easy to debug, while `-O1` provides a balanced optimization for performance and size, suitable for more general use cases where debugging is less of a concern.


## Example
### The C Code
* We try the code below:
	```c
	#define N 16
	typedef int fix_matrix[N][N];
	
	/* Compute i,k of fixed matrix product */
	int fix_prod_ele (fix_matrix A, fix_matrix B, long i, long k) {
		long j;
		int result = 0;
		for (j = 0; j < N; j++)
			result += A[i][j] * B[j][k];
		return result;
	}
	```
	* Whenever a program uses some constant as an array dimension or buffer size, it is best to associate a name with it via a #define declaration, and then use this name consistently, rather than the numeric value.

### The `-Og` Assembly Code
* No optimization - compile with `-Og`: `gcc -Og -S fix_prod.c` and get the assembly code below:
	```z80
	# int fix_prod_ele (fix_matrix A, fix_matrix B, long i, long k)
	# rdi - fix_matrix A
	# rsi - fix_matrix B
	# rdx - long i
	# rcx - long k
	
	.text
	.globl  fix_prod_ele
	.type   fix_prod_ele, @function
	fix_prod_ele:
	        movq    %rdi, %r9       # r9 =rdi: r9 =A
	        movq    %rsi, %r10      # r10=rsi: r10=B
	        movl    $0, %r8d        # r8d=0
	        movl    $0, %eax        # eax=0
	        jmp     .L2
	.L3:
	        movq    %rdx, %rdi      # rdi=rdx: rdi=i
	        salq    $6, %rdi        # rdi=rdi<<6: rdi=64i
	        addq    %r9, %rdi       # rdi=rdi+r9: rdi=A+64i
	        movq    %rax, %rsi      # rsi=rax: rsi=0
	        salq    $6, %rsi        # rsi=rsi<<6: rsi=64*rsi
	        addq    %r10, %rsi      # rsi=rsi+r10: rsi=rsi+B
	        movl    (%rsi,%rcx,4), %esi  # esi=M(rsi+4*rcx): B[j][k]
	        imull   (%rdi,%rax,4), %esi  # esi=M(rdi+4*rax)*esi: A[i][j] * B[j][k]
	        addl    %esi, %r8d      # r8d=esi+r8d: result
	        addq    $1, %rax        # rax+=1: j++
	.L2:
	        cmpq    $15, %rax       # 
	        jle     .L3             # if(rax<=15) jump to .L3: j<N
	        movl    %r8d, %eax      # return result
	        ret
	```

### The `-O1` Assembly Code
* `-O1` optimization - compile with `-O1`: `gcc -O1 -S fix_prod.c` and get the assembly code below:
	```z80
	# int fix_prod_ele (fix_matrix A, fix_matrix B, long i, long k)
	# rdi - fix_matrix A
	# rsi - fix_matrix B
	# rdx - long i
	# rcx - long k
	
	.text
	.globl  fix_prod_ele
	.type   fix_prod_ele, @function
	fix_prod_ele:
	        salq    $6, %rdx             # rdx=rdx*64:    rdx=64i
	        addq    %rdx, %rdi           # rdi=rdi+rdx:   rdi=A+64i=&A[i][0] -> A_ptr
	        leaq    (%rsi,%rcx,4), %rax  # rax=rsi+4*rcx: rax=B+4k=&B[0][k] -> B_ptr
	        leaq    1024(%rax), %rsi     # rsi=rax+1024:  rsi=B+4k+1024=&B[N][k] -> B_end
	        movl    $0, %ecx             # ecx=0
	.L2:
	        movl    (%rdi), %edx         # edx=M(A+64i): edx=A[i][0]
	        imull   (%rax), %edx         # edx=M(A+64i)*M(B+4k): edx=A[i][0]*B[0][k]
	        addl    %edx, %ecx           # ecx=ecx+edx: ecx=result+A[i][0]*B[0][k]
	        addq    $4, %rdi             # rdi+=4: A_ptr++
	        addq    $64, %rax            # rax+=64: B_ptr+=N
	        cmpq    %rsi, %rax           # Compare B_ptr and B_end
	        jne     .L2
	        movl    %ecx, %eax
	        ret
	```
	* Suppose: `&A[i][0]` is `A_ptr`, `&B[0][k]` is `B_ptr`, `&B[N][k]` is `B_end`.
		* Generating a pointer, which we have named `Aptr`, that points to successive elements in row i of A.
		* Generating a pointer, which we have named `Bptr`, that points to successive elements in column k of B. 
		* Generating a pointer, which we have named `Bend`, that equals the value `Bptr` will have when it is time to terminate the loop.
	* It removes the integer index `j` and converts all array references to pointer dereferences.
		* The initial value for `Aptr` is the address of the first element of row i of A, given by the C expression `&A[i][0]`. 
		* The initial value for `Bptr` is the address of the first element of column k of B, given by the C expression `&B[0][k]`. 
		* The value for Bend is the index of what would be the `(n + 1)st` element in column j of B, given by the C expression `&B[N][k]`.
* Optimized C code:
	![[image-20240602211651509.png|400]]

# Practice Problem 3.39
Use Equation 3.1 to explain how the computations of the initial values for `Aptr`, `Bptr`, and `Bend` in the C code of Figure 3.37(b) (lines 3–5) correctly describe their computations in the assembly code generated for `fix_prod_ele` (lines 3–5).

**Solution**:
Equation 3.1: $\&D[i][j] = x_D + L(C \times i + j)$
C = 16 because of `#define N 16`.
L = 4  because of integer type.

$Aptr = \&A[i][0] = x_A+4(16 \times i+0)=x_A+64i$
$Bptr=\&B[0][k]=x_B+4(16 \times 0+k)=x_B+4k$
$Bend=\&B[N][k]=x_B+4(16 \times 16 + k)=x_B+4k+1024$

# Practice Problem 3.40
The following C code sets the diagonal elements of one of our fixed-size arrays to `val`:
```c
/* Set all diagonal elements to val */

void fix_set_diag(fix_matrix A, int val) {
	long i;
	for (i = 0; i < N; i++)
		A[i][i] = val;
}
```
When compiled with optimization level `-O1`, gcc generates the following assembly code:
```z80
# void fix_set_diag(fix_matrix A, int val)
# A in %rdi, val in %rsi

fix_set_diag:
	movl $0, %eax
.L13:
	movl %esi, (%rdi,%rax)
	addq $68, %rax
	cmpq $1088, %rax
	jne .L13
	rep; ret
```
Create a C code program `fix_set_diag_opt` that uses optimizations similar to those in the assembly code, in the same style as the code in Optimized C Code. Use expressions involving the parameter N rather than integer constants, so that your code will work correctly if N is redefined.

**Solution**:
Firstly, we interpret the assembly code below:
```z80
# void fix_set_diag(fix_matrix A, int val)
# A in %rdi, val in %rsi

fix_set_diag:
	movl $0, %eax             # eax=0
.L13:
	movl %esi, (%rdi,%rax)    # M(rax+rdi)=esi: *(rax+A)=val
	addq $68, %rax            # rax+=68
	cmpq $1088, %rax          # compare rax and 1088
	jne .L13                  # if rax <= 1088, jump to .L13
	rep; ret
```
Secondly, we introduce `char* Abase` and construct the C code below:
```C
void fix_set_diag_opt(fix_matrix A, int val)
{
	char* Abase = &A[0][0];
	int i = 0;              // movl $0, %eax
	do{
		*(Abase + i) = val;
		i+=68;
	}while(i<=1088)         // cmpq $1088, %rax
	                        // jne .L13
}
```
Thirdly, 
Suppose N is `int A[N][N]`, then we get the equation below:
	4(N+1)=68 -> N=16

# 3.8.5 Variable-Size Arrays
* In the C version of variable-size arrays, we can declare an array：
	```c
	int A[expr1][expr2]
	```
* Example - access element i, j of an n × n array:
	```c
	int var_ele(long n, int A[n][n], long i, long j) {
		return A[i][j];
	}
	```
	* The parameter n must precede the parameter `A[n][n]`, so that the function can compute the array dimensions as the parameter is encountered.
	* Assembly code:
		![[image-20240603110542601.png|400]]
	* The code computes the address of element i, j as $x_A + 4(n \times i) + 4j = x_A + 4(n \times i + j)$.

* When variable-size arrays are referenced within a loop, the compiler can often optimize the index computations by exploiting the regularity of the access patterns.

## Example
### C Code
```C
/* Compute i,k of variable matrix product */

int var_prod_ele(long n, int A[n][n], int B[n][n], long i, long k) {
	long j;
	int result = 0;
	for (j = 0; j < n; j++)
		result += A[i][j] * B[j][k];
	return result;
}
```

### Assembly Code - `-Og`
```z80
# int var_prod_ele(long n, int A[n][n], int B[n][n], long i, long k)
# rdi - n
# rsi - A
# rdx - B
# rcx - i
# r8  - k

.text
.globl  var_prod_ele
var_prod_ele:
        movq    %rsi, %r10           # r10=rsi: r10=A
        movq    %rdx, %r11           # r11=rdx: r11=B
        movl    $0, %esi             # esi=0
        movl    $0, %eax             # eax=0
        jmp     .L2
.L3:
        movq    %rcx, %rdx           # rdx=rcx: rdx=i
        imulq   %rdi, %rdx           # rdx=rdi*rdx: rdx=n*i
        leaq    (%r10,%rdx,4), %r9   # r9=r10+4*rdx: r9=A+4*n*i
        movq    %rax, %rdx           # rdx=rax: rdx=0
        imulq   %rdi, %rdx           # rdx=rdx*rdi: rdx=rdx*n
        leaq    (%r11,%rdx,4), %rdx  # rdx=r11+4*rdx: B+4*n*rdx
        movl    (%rdx,%r8,4), %edx   # edx=M(rdx+4*r8): edx=*(B+4*n*rdx+4*k)
        imull   (%r9,%rax,4), %edx   # edx=edx*M(r9+4*rax): edx=*(B+4*n*rdx+4*k) * *(A+4*n*i+4*rax)
        addl    %edx, %esi           # esi=esi+edx: result += A[i][j] * B[j][k]
        addq    $1, %rax             # rax+=1: j++
.L2:
        cmpq    %rdi, %rax
        jl      .L3
        movl    %esi, %eax
        ret
```
* Transform to C code:
```C
int var_prod_ele(long n, int A[n][n], int B[n][n], long i, long k)
{
	char* Abase = &A[0][0];
	char* Bbase = &B[0][0];
	int j = 0;
	int result = 0;

	while(j < n)
	{
		result += A[i][j] * B[j][k];
	}
	return result;
}
```


### Assembly Code - `-O1`
```z80
# int var_prod_ele(long n, int A[n][n], int B[n][n], long i, long k)
# rdi - n
# rsi - A
# rdx - B
# rcx - i
# r8  - k

.text
.globl  var_prod_ele
var_prod_ele:
        testq   %rdi, %rdi            # test n&n
        jle     .L4                   # if n<=0, jump to .L4
        imulq   %rdi, %rcx            # rcx=rcx*rdi: rcx=i*n
        leaq    (%rsi,%rcx,4), %r10   # r10=rsi+4*rcx: r10=A+4i
        leaq    0(,%rdi,4), %r9       # r9=4*rdi: r9=4n
        leaq    (%rdx,%r8,4), %rcx    # rcx=rdx+4*r8: rcx=B+4k
        movl    $0, %esi              # esi=0: result=0
        movl    $0, %eax              # eax=0: j=0
.L3:
        movl    (%r10,%rax,4), %edx   # edx=M(r10+4*rax): edx=*(A+4i+4*j)
        imull   (%rcx), %edx          # edx=edx*M(rcx): *(A+4i+4*j) * *(B+4k)
        addl    %edx, %esi            # esi+=edx: result = result +  *(A+4i+4*j) * *(B+4k)
        addq    $1, %rax              # rax+=1: j++
        addq    %r9, %rcx             # rcx+=r9: rcx=(B+4k)+4n
        cmpq    %rax, %rdi            # compare rax and rdi
        jne     .L3
.L1:
        movl    %esi, %eax
        ret
.L4:
        movl    $0, %esi
        jmp     .L1
```
* Transform to C code:
```C
int var_prod_ele(long n, int A[n][n], int B[n][n], long i, long k)
{
	int* Aptr = &A[i][0];
	int* Bptr = &B[0][k];
	int result = 0;
	int j = 0;
	do{
		result += (*(Aptr+j))*(*Bptr);
		Bptr+=n;
	}while(j!=n)
	return result;
}
```

* We have seen that, with optimizations enabled, gcc is able to recognize patterns that arise when a program steps through the elements of a multidimensional array.












