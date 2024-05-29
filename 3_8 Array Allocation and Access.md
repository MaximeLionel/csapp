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
## Example:
* Consider the declaration below:
	```c
	char A[12];
	char *B[8];
	int C[6];
	double *D[5];
	```
* The declarations will generate arrays with the following parameters:
	![[3_8 Array Allocation and Access.assets/image-20240529152558409.png|400]]









