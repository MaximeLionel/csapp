* Procedures are a key abstraction in software.
	* Provide a way to package code that implements some functionality with a designated set of arguments and an optional return value.
* Procedures come in many guises in different programming languages — functions, methods, subroutines, handlers, and so on — but they all share a general set of features.
* suppose procedure P calls procedure Q, and Q then executes and returns back to P. These actions involve one or more of the following mechanisms:
	* **Passing control** - The program counter must be set to the starting address of the code for Q upon entry and then set to the instruction in P following the call to Q upon return.
	* **Passing data** - P must be able to provide one or more parameters to Q, and Q must be able to return a value back to P.
	* **Allocating and deallocating memory** - Q may need to allocate space for local variables when it begins and then free that storage before it returns.

# 3.7.1 The Run-Time Stack
* A key feature of the procedure-calling mechanism of C, and of most other languages, is that it can make use of the **last-in, first-out** memory management discipline provided by a **stack** data structure.
* Suppose procedure P calling Q:
	* while Q is executing, P is temporarily suspended.
	* While Q is running, only it will need the ability to **allocate new storage** for its local variables or to set up a call to another procedure.
	* When Q returns, any local storage it has allocated can be freed.
* A program can manage the storage required by its procedures using a stack:
	* As P calls Q, control and data information are added to the end of the stack. 
	* This information gets deallocated when P returns.
* Stack management:
	* The x86-64 stack grows toward lower addresses and the stack pointer `%rsp` points to the top element of the stack.
	* Data can be stored on and retrieved from the stack using the `pushq` and `popq` instructions.
	* Space for data with no specified initial value can be allocated on the stack by simply decrementing the stack pointer by an appropriate amount. 
	* Space can be deallocated by incrementing the stack pointer.
## Stack Frame
![[3_7 Procedures.assets/image-20240522094322604.png|500]]
* When an x86-64 procedure requires storage beyond what it can hold in registers, it allocates space on the stack, which is the procedure's **stack frame**.
* The frame for the currently executing procedure is always at the top of the stack.
* When procedure P calls procedure Q, it will push the **return address** onto the stack, indicating where within P the program should resume execution once Q returns.
	* The return address is part of P’s stack frame, since it holds state relevant to P.
* Q allocates the space required for its stack frame by extending the current stack boundary.
	* With the stack fram, Q cAan save the values of registers, allocate space for local variables, and set up arguments for the procedures it calls.
* The stack frames for most procedures are of fixed size, allocated at the beginning of the procedure.
* Procedure P can pass up to six integral values on the stack, but if Q requires more arguments, these can be stored by P within its stack frame prior to the call.
* In the interest of space and time efficiency, x86-64 procedures allocate only the portions of stack frames they require.
* Some functions do not even require a stack frame. This occurs when all of the local variables can be held in registers and the function does not call any other functions.

# 3.7.2 Control Transfer
* Passing control from function P to function Q involves simply setting the **program counter (PC)** to the starting address of the code for Q.
* When later comes time for Q to return, the processor must have some record of the code location where it should resume the execution of P. This information is recorded in x86-64 machines by invoking procedure Q with the instruction `call Q`.
* `Call Q`:
	* Pushes an address A onto the stack; 
		* Address A is referred to as the return address and is computed as the address of the instruction immediately following the call instruction.
	* Sets the PC to the beginning of Q.
* `ret`:
	* Pops an address A off the stack;
	* Sets the PC to A.
![[3_7 Procedures.assets/image-20240522101614495.png|300]]
* A call can be either direct or indirect:
	* Direct call - the target of a direct call is given as a label;
	* Indirect call - the target of an indirect call is given by `*` followed by an operand specifier
