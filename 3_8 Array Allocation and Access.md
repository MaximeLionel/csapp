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




