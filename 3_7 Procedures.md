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
	* With the stack fram, Q can save the values of registers, allocate space for local variables, and set up arguments for the procedures it calls.
* The stack frames for most procedures are of fixed size, allocated at the beginning of the procedure.