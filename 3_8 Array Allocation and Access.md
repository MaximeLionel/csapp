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
	char *B[8];
	int C[6];
	double *D[5];
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
	* For an expression AExpr denoting an address, *AExpr gives the value at that address.
	* The expressions Expr and \*&Expr are therefore equivalent.
	* The array `A[i]` is identical to the expression `*(A+i)`.
* Suppose the starting address of integer array E and integer index i are stored in registers `%rdx` and `%rcx`:
	![[image-20240531235948114.png|600]]

# Practice Problem 3.37
Suppose $x_P$, the address of short integer array P, and long integer index i are stored in registers %rdx and %rcx, respectively. For each of the following expressions, give its type, a formula for its value, and an assembly-code implementation. The result should be stored in register %rax if it is a pointer and register element %ax if it has data type short.

| Expression | Type | Value | Assembly Code |
| :--------: | ---- | ----- | ------------- |
|   `P[1]`   |      |       |               |
|  `P+3+i`   |      |       |               |
| `P[i*6-5]` |      |       |               |
|   `P[2]`   |      |       |               |
| `&P[i+2]`  |      |       |               |
**Solution**:

| Expression | Type   | Value           | Assembly Code                  |
| :--------: | ------ | --------------- | ------------------------------ |
|   `P[1]`   | short  | M[$x_P$+2]      | `movw 2(%rdx), %rax`           |
|  `P+3+i`   | short* | $x_P$+3*2+2i    | `leaq 6(%rdx,%rcx,2), %rax`    |
| `P[i*6-5]` | short  | M[$x_P$+12i-10] | `movw -10(%rdx,%rcx,12), %rax` |
|   `P[2]`   | short  | M[$x_P$+4]      | `movw 4(%rdx), %rax`           |
| `&P[i+2]`  | short* | $x_P$+2*2+2i    | `leaq 4(%rdx,%rcx,2), %rax`    |

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
		![[image-20240601100306237.png|200]]
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
	movq Q(,%rdi,8), %rax      # rax=Q+8*rdi: rax=Q+8(i+5j)=Q+8i+40j
	addq P(,%rdx,8), %rax      # rax=rax+P+8*rdx: rax=Q+8i+40j+P+8(7i+j) =P+Q+64i+48j
	ret
```
* Secondly, we analyze the C code:
	`long P[M][N];`
	`long Q[N][M];`
	`P[i][j] + Q[j][i] = P + i*N + j + Q + j*M + i`
	`= P + Q + (N+1)*i + (M+1)*j`





