# 4.1.1 Programmer-Visible State
## Registers
![[Pasted image 20240727231255.png|300]]
* 15 program registers: %rax, %rcx, %rdx, %rbx, %rsp, %rbp, %rsi, %rdi, and %r8 through %r14.
	* omit the x86-64 register %r15 to simplify the instruction encoding.
* Each of the registers stores a 64-bit word.
* Register %rsp is used as a stack pointer by the `push`, `pop`, `call`, and `return` instructions.
* The program counter (PC) holds the address of the instruction currently being executed.

## Memory
* The memory is conceptually **a large array of bytes**, holding both program and data.
* Y86-64 programs reference memory locations using ==virtual addresses==.
	* A combination of hardware and operating system software translates virtual addresses into the **actual, or physical, addresses** indicating where the values are actually stored in memory.

## Status code (Stat)
* A final part of the program state is a **status code (Stat)**, indicating the overall state of program execution.
* It will indicate either normal operation or that some sort of ==exception== has occurred, such as when an instruction attempts to read from an invalid memory address.
* There are 3 single-bit condition codes, ZF, SF, and OF, storing information about the effect of the most recent arithmetic or logical instruction.
	![[Pasted image 20240728220428.png|100]]

# 4.1.2 Y86-64 Instructions
![[Pasted image 20240728220633.png|500]]
* In this figure, we show the assembly-code representation of the instructions on the left and the byte encodings on the right.
* The set of Y86-64 instructions is largely a subset of the x86-64 instruction set.
* Y86-64 Instruction encodings range between 1 and 10 bytes.
* An instruction consists of a 1-byte instruction specifier, possibly a 1-byte register specifier, and possibly an 8-byte constant word.
* Field `fn` specifies a particular integer operation (`OPq`), data movement condition (`cmovXX`), or branch condition (`jXX`). 
* All numeric values are shown in hexadecimal.


## `MOVQ` - data transfer instruction
![[Pasted image 20240730223317.png|500]]
* The Y86-64 `movq` instruction is split into four different instructions: `irmovq`, `rrmovq`, `mrmovq`, and `rmmovq`, explicitly indicating the form of the source and destination.
* The source is either immediate (i), register (r), or memory (m). It is designated by the first character in the instruction name.
* The destination is either register (r) or memory (m). It is designated by the second character in the instruction name.
* The memory references for the two memory movement instructions have a simple base and displacement format.
* As with x86-64, we do not allow direct transfers from one memory location to another. In addition, we do not allow a transfer of immediate data to memory.

## `OPq` - integer operation instructions
![[Pasted image 20240730225333.png|300]]

![[Pasted image 20240730225244.png|100]]
* 4 integer operation instructions: `addq`, `subq`, `andq`, and `xorq`.
* Y86-64 `OPq` operate only on register data, whereas x86-64 `OPq` also allows operations on memory data.
* Y86-64 `OPq` set the three condition codes ZF, SF, and OF (zero, sign, and overflow).

## `JXX` - jump instructions
* 7 jump instructions: `jmp`, `jle`, `jl`, `je`, `jne`, `jge`, and `jg`.
* Branches are taken according to the type of branch and the settings of the condition codes.

## `CMOV` - conditional move instructions
* 6 conditional move instructions: `cmovle`, `cmovl`, `cmove`, `cmovne`, `cmovge`, and `cmovg`.

## `CALL` and `RET`
* The `call` instruction pushes the return address on the stack and jumps to the destination address. 
* The `ret` instruction returns from such a call.

## `pushq` and `popq` instructions
* The `pushq` and `popq` instructions implement `push` and `pop`.

## `halt`
* The `halt` instruction stops instruction execution.
* Y86-64 executes the `halt` instruction causes the processor to stop, with the status code set to `HLT`.

# 4.1.3 Instruction Encoding - Y86-64
* One important property of any instruction set is that the byte encodings must have a unique interpretation.
* This property ensures that a processor can execute an object-code program without any ambiguity about the meaning of the code.
## Byte-level encoding of the instructions
![[Pasted image 20240801141959.png|400]]
* Each instruction requires between 1 and 10 bytes, depending on which fields are required.
* Every instruction has an initial byte identifying the instruction type. This byte is split into two 4-bit parts: 
	* the high-order, or code, part.
		* **The code values** range from 0 to 0xB.
	* the low-order, or function, part.
		* **The function values** are significant only for the cases where a group of related instructions share a common code.

## Function values for Y86-64 instruction set.
![[Pasted image 20240801143245.png|500]]
* Observe that `rrmovq` has the same instruction `code value` as the conditional moves, which can be viewed as an “unconditional move”.
* Also `jmp` instruction can also be viewed as an unconditional jump.

## Register identifiers
![[Pasted image 20240801144810.png|400]]
* Each of the 15 program registers has an associated **identifier (ID)** ranging from 0 to 0xE. 
* ID 0xF in a register field of an instruction indicates the absence of a register operand.
* The program registers are stored within the CPU in a register file, a small random access memory where the register IDs serve as addresses.
* There can be an additionalregister specifier byte, specifying either one or two registers. These register fields are called rA and rB.
	![[Pasted image 20240801145505.png|350]]
	* As the assembly-code versions of the instructions show, they can specify the registers used for data sources and destinations, as well as the base register used in an address computation, depending on the instruction type.
	* Instructions that have no register operands, such as `branches` and `call`, do not have a register specifier byte.
	* Those that require just one register operand (`irmovq`, `pushq`, and `popq`) have the other register specifier set to value 0xF.

## Constant word
![[Pasted image 20240801145505.png|350]]
* Some instructions require an additional 8-byte constant word.
* This word can serve as the immediate data for `irmovq`, the displacement for `rmmovq` and `mrmovq` address specifiers, and the destination of `branches` and `calls`.
* Fpr Y86-64, branch and call destinations are given as absolute addresses, rather than using the PC-relative addressing seen in x86-64.
* As with x86-64, all integers have a `little-endian encoding`. When the instruction is written in disassembled form, these bytes appear in reverse order.

## Example - generate the byte encoding of an instruction
Let us generate the byte encoding of the instruction `rmmovq %rsp,0x123456789abcd(%rdx)` in hexadecimal.
* `rmmovq` has initial byte 40.
* The source register `%rsp`: 
	* Encoded in the `rA` field.
	* Register ID is 4.
* The base register `%rdx`: 
	* Encoded in the `rB` field.
	* Register ID is 2.
* The displacement (0x123456789abcd) is encoded in the 8-byte constant word.
	* Pad `0x123456789abcd` with leading zeros to fill out 8 bytes, giving a byte sequence of `00 01 23 45 67 89 ab cd`.
	* Because of little-endian encoding, write this in byte-reversed order as `cd ab 89 67 45 23 01 00`.
* Fill in the bytes as the format below:
	![[Pasted image 20240801153216.png|600]]
	```
	40 42 cd ab 89 67 45 23 01 00   rmmovq %rsp, 0x123456789abcd(%rdx)
	```


# Summary of Y86-64 Instructions
![[Pasted image 20240801215256.png|550]]
![[Y86-64.htm]]

![[Pasted image 20240801221104.png|300]]
# Practice Problem 4.1
Determine the byte encoding of the Y86-64 instruction sequence that follows. The line `.pos 0x100` indicates that the starting address of the object code should be 0x100.
```
.pos 0x100 # Start code at address 0x100
	irmovq $15,%rbx
	rrmovq %rbx,%rcx
loop:
	rmmovq %rcx,-3(%rbx)
	addq %rbx,%rcx
	jmp loop
```

**Solution**:
`irmovq $15,%rbx`:
* `irmovq` format:
	![[Pasted image 20240801154741.png|500]]
* rB - rbx (register ID: 3)
* Constant - 15:
	* 15 - 0x 00 00 00 00 00 00 00 0F
	* Reverse the bits - `0F 00 00 00 00 00 00 00`
* Byte encoding: `30 F3 0F 00 00 00 00 00 00 00

`rrmovq %rbx,%rcx`:
* `rrmovq` format:
	![[Pasted image 20240801161232.png|200]]
* rA - rbx (register ID: 3)
* rB - rcx (register ID: 1)
* Byte encoding: `20 31`

`rmmovq %rcx,-3(%rbx)`:
* `rmmovq` format:
	![[Pasted image 20240801161427.png|500]]
* rA - rcx (register ID: 1)
* rB - rbx (register ID: 3)
* D - 0x FF FF FF FF FF FF FF FD
	* Reverse the bits - `FD FF FF FF FF FF FF FF`
* Byte encoding: `40 13 FD FF FF FF FF FF FF FF`

`addq %rbx,%rcx`:
* `addq` format:
	![[Pasted image 20240801161835.png|200]]
	* function values of `addq` - 60
	![[Pasted image 20240801161921.png|70]]
* rA - rbx (register ID: 3)
* rB - rcx (register ID: 1)
* Byte encoding: `60 31`

`jmp loop`:
* `jmp` format:
	![[Pasted image 20240801162302.png|500]]
	* function values of `jmp` - 70
	![[Pasted image 20240801162429.png|120]]
* Dest:
	* We need to calculate the address of .loop: 0x 100 + 0x A + 0x 2 = 0x 10C
	* Expand to 8 bytes: 0x 00 00 00 00 00 00 01 0C
	* Reverse the bits: `0C 01 00 00 00 00 00 00`
* Byte encoding: `70 0C 01 00 00 00 00 00 00`

Overall, we get the full byte encodings below:
```
.pos 0x100 # Start code at address 0x100
100	 30 F3 0F 00 00 00 00 00 00 00  irmovq $15,%rbx
10A	 20 31                          rrmovq %rbx,%rcx
10C  40 13 FD FF FF FF FF FF FF FF  rmmovq %rcx,-3(%rbx)
116  60 31	                        addq %rbx,%rcx
118  70 0C 01 00 00 00 00 00 00	    jmp loop
```

# Practice Problem 4.2
For each byte sequence listed, determine the Y86-64 instruction sequence it encodes. If there is some invalid byte in the sequence, show the instruction sequence up to that point and indicate where the invalid value occurs. For each sequence, we show the starting address, then a colon, and then the byte sequence.

A. 0x100: 30f3fcffffffffffffff40630008000000000000

B. 0x200: a06f800c020000000000000030f30a00000000000000

C. 0x300: 5054070000000000000010f0b01f

D. 0x400: 611373000400000000000000

E. 0x500: 6362a0f0

**Solution**:
A. 0x100: 30f3fcffffffffffffff40630008000000000000
`30f3fcffffffffffffff`:
* 30:
	![[Pasted image 20240801222447.png|500]]
* rB = 3: %rbx
* V = 0x ff ff ff ff ff ff ff fc (reversed) = -4
* Code: `irmovq $-4, %rbx`
`40630008000000000000`:
* 40: ![[Pasted image 20240801223005.png|500]]
* rA = 6: %rsi
* rB = 3: %rbx
* D = 0x 00 00 00 00 00 00 08 00 (reversed) = 0x800
* Code: `rmmovq %rsi, 0x800(%rbx)`

Full code:
```
0x100 30 f3 fc ff ff ff ff ff ff ff    irmovq $-4, %rbx
0x10A 40 63 00 08 00 00 00 00 00 00    rmmovq %rsi, 0x800(%rbx)
```

B. 0x200: a06f800c020000000000000030f30a00000000000000
`a06f`:
* a0: ![[Pasted image 20240801223854.png|150]]
* rA = 6: %rsi
* Code: `pushq %rsi`
`800c02000000000000`:
* 80: ![[Pasted image 20240801224120.png|400]]
* D = 0x 00 00 00 00 00 00 02 0c (reversed) = 0x20c
* Code: `call 0x20c` - what is 0x20c? we will see next.
`00`: 
* Code: `halt` 
`30f30a00000000000000`:
* 30: ![[Pasted image 20240801224717.png|450]]
* rB = 3: %rbx
* V = 0x 00 00 00 00 00 00 00 0a (reversed) = 10
* Code: `irmovq $10, %rbx`

Full code:
```
0x200 a0 6f                         pushq %rsi
0x202 80 0c 02 00 00 00 00 00 00    call func:
0x20b 00                            halt
0x20c func:
0x20c 30 f3 0a 00 00 00 00 00 00 00 irmovq $10, %rbx
```

C. 0x300: 5054070000000000000010f0b01f
`50540700000000000000`:
* 50: ![[Pasted image 20240801225608.png|500]]
* rA = 5: %rbp
* rB = 4: %rsp
* D = 0x 00 00 00 00 00 00 00 07 = 7
* Code: `mrmovq 7(%rsp), %rbp`
`10`:
* Code: `nop`
`f0`: invalid code
`b01f`:
* b0: ![[Pasted image 20240801230200.png|150]]
* rA = 1: %rcx
* Code: `popq %rcx`

Full code:
```
0x300 50 54 07 00 00 00 00 00 00 00    mrmovq 7(%rsp), %rbp
0x30a 10                               nop
0x30b f0
0x30c b0 1f                            popq %rcx
```

D. 0x400: 611373000400000000000000
`6113`:
* 61: ![[Pasted image 20240801230923.png|200]]
* rA = 1: %rcx
* rB = 3: %rbx
* Code: `subq %rcx, %rbx`
`730004000000000000`:
* 73: ![[Pasted image 20240801231139.png|500]]
* Dest = 0x 00 00 00 00 00 00 04 00 = 0x400
* Code: `je 0x400`
`00`: 
* Code: `halt`

Full code:
```
0x400 loop:
0x400 61 13                      subq %rcx, %rbx
0x402 73 00 04 00 00 00 00 00 00 je loop
0x40c 00                         halt
```


E. 0x500: 6362a0f0
`6362`:
* 63: ![[Pasted image 20240801231704.png|180]]
* rA = 6: %rsi
* rB = 2: %rdx
* Code: `xorq %rsi, %rdx`

* a0: ![[Pasted image 20240801231916.png|150]]
* f0: invalid instruction

Full code:
```
0x500 63 62 xorq %rsi, %rdx
0x502 a0    pushq???
0x503 f0    invalid instruction
```


# 4.1.4 Y86-64 Exceptions
* Y86-64 status codes：
	![[Pasted image 20240802162013.png|300]]
	* `AOK` - the program is executing normally.
	* `HLT` - the processor has executed a ==halt== instruction.
	* `ADR` - the processor attempted to read from or write to an ==invalid== memory address, either while fetching an instruction or while reading or writing data.
	* `INS` - an invalid instruction code has been encountered.
* For Y86-64, we will simply have the processor stop executing instructions when it encounters any of the exceptions listed.
* In a more complete design, the processor would typically invoke an exception handler, a procedure designated to handle the specific type of exception encountered.

# 4.1.5 Y86-64 Programs
## Comparison of Y86-64 and x86-64 assembly programs
* C code:
	```c
	long sum(long *start, long count)
	{
		long sum = 0;
		while (count) {
			sum += *start;
			start++;
			count--;
		}
		return sum;
	}
	```
* x86-64 assembly code:
	```
	# long sum(long *start, long count)
	# start in %rdi, count in %rsi
	
	sum:
		movl $0, %eax       # sum = 0
		jmp .L2             # Goto test
	.L3:                    # loop:
		addq (%rdi), %rax   # Add *start to sum
		addq $8, %rdi       # start++
		subq $1, %rsi       # count--
	.L2:                    # test:
		testq %rsi, %rsi    # Test sum
		jne .L3             # If !=0, goto loop
		rep; ret            # Return
	```
* y86-64 assembly code:
	```
	# long sum(long *start, long count)
	# start in %rdi, count in %rsi
	
	sum:
		irmovq $8,%r8           # Constant 8
		irmovq $1,%r9           # Constant 1
		xorq %rax,%rax          # sum = 0
		andq %rsi,%rsi          # Set CC - condition codes
		jmp test                # Goto test
	loop:
		mrmovq (%rdi),%r10      # Get *start
		addq %r10,%rax          # Add to sum
		addq %r8,%rdi           # start++
		subq %r9,%rsi           # count--. Set CC
	test:
		jne loop                # Stop when 0
		ret                     # Return
	```
	* The Y86-64 code loads constants into registers, since it cannot use immediate data in arithmetic instructions.
	```
		irmovq $8,%r8           # Constant 8
		irmovq $1,%r9           # Constant 1
	```
	* The Y86-64 code requires two instructions to read a value from memory and add it to a register, whereas the x86-64 code can do this with a single `addq` instruction.
	```y86-64
		mrmovq (%rdi),%r10      # Get *start
		addq %r10,%rax          # Add to sum
	```

	```x86-64
		addq (%rdi), %rax       # Add *start to sum
	```
	* Our hand-coded Y86-64 implementation takes advantage of the property that the `subq` instruction also sets the condition codes, and so the `testq` instruction of the gcc-generated code is not required. For this to work, though, the Y86-64 code must set the condition codes prior to entering the loop with an `andq` instruction.
		`andq %rsi,%rsi          # Set CC - condition codes`
		`subq %r9,%rsi           # count--. Set CC`


## Sample program written in Y86-64 assembly code
```y86-64
# Execution begins at address 0
	.pos 0
	irmovq stack, %rsp    # Set up stack pointer
	call main             # Execute main program
	halt                  # Terminate program

# Array of 4 elements
	.align 8
array:
	.quad 0x000d000d000d
	.quad 0x00c000c000c0
	.quad 0x0b000b000b00
	.quad 0xa000a000a000

main:
	irmovq array,%rdi
	irmovq $4,%rsi
	call sum              # sum(array, 4)
	ret

# long sum(long *start, long count)
# start in %rdi, count in %rsi
sum:
	irmovq $8,%r8    # Constant 8
	irmovq $1,%r9    # Constant 1
	xorq %rax,%rax   # sum = 0
    andq %rsi,%rsi   # Set CC
    jmp test         # Goto test

loop:
    mrmovq (%rdi),%r10  # Get *start
    addq %r10,%rax      # Add to sum
    addq %r8,%rdi       # start++
    subq %r9,%rsi       # count--. Set CC

test:
    jne loop            # Stop when 0
    ret                 # Return

# Stack starts here and grows to lower addresses
	.pos 0x200
stack:
```
* An example of a complete program file written in Y86-64 assembly code.
	* The program contains both data and instructions. 
	* Directives indicate where to place code or data and how to align it.
	* The program specifies issues such as stack placement, data initialization, program initialization, and program termination.
* Detailed explanation of the program:
	* Words beginning with ‘.’ are assembler directives telling the assembler to adjust the address at which it is generating code or to insert some words of data.
	* The directive `.pos 0` (line 2) indicates that the assembler should begin generating code starting at address 0. This is the starting address for all Y86-64 programs.
	* The next instruction (line 3: `irmovq stack, %rsp    # Set up stack pointer`) initializes the stack pointer. We can see that the label stack is declared at the end of the program (line 42: `stack:`), to indicate address 0x200 using a .pos directive (line 41: `.pos 0x200`).
		* Our stack will therefore start at this address and grow toward lower addresses.
	* Lines 8 to 13 of the program declare an array of four words, having the values.
		```Y86-64
			.align 8
		array:
			.quad 0x000d000d000d
			.quad 0x00c000c000c0
			.quad 0x0b000b000b00
			.quad 0xa000a000a000
		```
		* The label `array` denotes the start of this array, and is aligned on an 8-byte boundary.
	* Lines 16 to 19 show a “main” procedure that calls the function sum on the four-word array and then halts.
		```
		main:
			irmovq array,%rdi
			irmovq $4,%rsi
			call sum              # sum(array, 4)
			ret
		```

## Output of yas assembler
```
                            | # Execution begins at address 0
0x000:                      | .pos 0
0x000: 30f40002000000000000 | irmovq stack, %rsp # Set up stack pointer
0x00a: 803800000000000000   | call main # Execute main program
0x013: 00                   | halt # Terminate program
                            |
                            | # Array of 4 elements
0x018:                      | .align 8
0x018:                      | array:
0x018: 0d000d000d000000     | .quad 0x000d000d000d
0x020: c000c000c0000000     | .quad 0x00c000c000c0
0x028: 000b000b000b0000     | .quad 0x0b000b000b00
0x030: 00a000a000a00000     | .quad 0xa000a000a000
                            |
0x038:                      | main:
0x038: 30f71800000000000000 | irmovq array,%rdi
0x042: 30f60400000000000000 | irmovq $4,%rsi
0x04c: 805600000000000000   | call sum # sum(array, 4)
0x055: 90                   | ret
                            |
                            | # long sum(long *start, long count)
                            | # start in %rdi, count in %rsi
0x056:                      | sum:
0x056: 30f80800000000000000 | irmovq $8,%r8 # Constant 8
0x060: 30f90100000000000000 | irmovq $1,%r9 # Constant 1
0x06a: 6300                 | xorq %rax,%rax # sum = 0
0x06c: 6266                 | andq %rsi,%rsi # Set CC
0x06e: 708700000000000000   | jmp test # Goto test
0x077:                      | loop:
0x077: 50a70000000000000000 | mrmovq (%rdi),%r10 # Get *start
0x081: 60a0                 | addq %r10,%rax # Add to sum
0x083: 6087                 | addq %r8,%rdi # start++
0x085: 6196                 | subq %r9,%rsi # count--. Set CC
0x087:                      | test:
0x087: 747700000000000000   | jne loop # Stop when 0
0x090: 90                   | ret # Return
                            |
                            | # Stack starts here and grows to lower addresses
0x200:                      | .pos 0x200
0x200:                      | stack:
```
* The result of assembling the code shown above by an assembler we call `yas`.
* Each line includes a hexadecimal address and between 1 and 10 bytes of object code.
* We have implemented an instruction set simulator we call `yis`, the purpose of which is to model the execution of a Y86-64 machine-code program without attempting to model the behavior of any specific processor implementation.

## Output of yis simulator
```
Stopped in 34 steps at PC = 0x13. Status ’HLT’, CC Z=1 S=0 O=0
Changes to registers:
%rax: 0x0000000000000000 0x0000abcdabcdabcd
%rsp: 0x0000000000000000 0x0000000000000200
%rdi: 0x0000000000000000 0x0000000000000038
%r8: 0x0000000000000000 0x0000000000000008
%r9: 0x0000000000000000 0x0000000000000001
%r10: 0x0000000000000000 0x0000a000a000a000
Changes to memory:
0x01f0: 0x0000000000000000 0x0000000000000055
0x01f8: 0x0000000000000000 0x0000000000000013
```
* The first line of the simulation output summarizes the execution and the resulting values of the PC and program status.
* In printing register and memory values, it only prints out words that change during simulation, either in registers or in memory. The original values (here they are all zero) are shown on the left, and the final values are shown on the right.

# Practice Problem 4.3
One common pattern in machine-level programs is to add a constant value to a
register. With the Y86-64 instructions presented thus far, this requires first using an `irmovq` instruction to set a register to the constant, and then an `addq` instruction to add this value to the destination register. Suppose we want to add a new instruction `iaddq` with the following format:
![[Pasted image 20240803235003.png|400]]
This instruction adds the constant value `V` to register `rB`.
Rewrite the Y86-64 sum function to make use of the `iaddq` instruction. In the original version, we dedicated registers `%r8` and `%r9` to hold constant values. Now, we can avoid using those registers altogether.

**Solution**:
```
	# long sum(long *start, long count)
	# start in %rdi, count in %rsi
	
	sum:
		# irmovq $8,%r8           # Constant 8
		# irmovq $1,%r9           # Constant 1
		xorq %rax,%rax          # sum = 0
		andq %rsi,%rsi          # Set CC - condition codes
		jmp test                # Goto test
	loop:
		mrmovq (%rdi),%r10      # Get *start
		addq %r10,%rax          # Add to sum
		iaddq $8,%rdi           # start++
		iaddq $-1,%rsi          # count--. Set CC
	test:
		jne loop                # Stop when 0
		ret                     # Return
```

# Practice Problem 4.4
Write Y86-64 code to implement a recursive product function `rproduct`, based
on the following C code:
```C
long rproduct(long *start, long count)
{
	if (count <= 1)
		return 1;
	return *start * rproduct(start+1, count-1);
}
```

Use the same argument passing and register saving conventions as x86-64 code
does. You might find it helpful to compile the C code on an x86-64 machine and then translate the instructions to Y86-64.

**Solution**:
* Compile the C code first:`gcc -Og -S -fno-stack-protector rproduct.c`
* x86-64 assembly code (`rproduct.s`) and add annotations:
	```x86-64
	# long rproduct(long *start, long count)
	# %rdi - start, %rsi - count
	rproduct:
		cmpq	$1, %rsi      # compare rsi,1: compare count and 1
		jle	.L3               # if count <= 1, goto .L3
		                      # if count > 1
		pushq	%rbx          # save %rbx
		movq	(%rdi), %rbx  # rbx=M(rdi): rbx = *start
		subq	$1, %rsi      # rsi=rsi-1: count--
		addq	$8, %rdi      # rdi=rdi+8: start++
		call	rproduct      # rproduct(start+1, count-1)
		imulq	%rbx, %rax    # rax=rax*rbx: rax = *start * rproduct(start+1, count-1)
		popq	%rbx          # load %rbx
		ret
	.L3:
		movl	$1, %eax      # eax = 1
		ret
	```
* Translate to y86-64 code:
	```y86-64
	# long rproduct(long *start, long count)
	# %rdi - start, %rsi - count
	rproduct:
		irmovq  $1, %r8
		irmovq  $8, %r9
		subq	%r8, %rsi     # compare rsi,1: compare count and 1
		jle	.L3               # if count <= 1, goto .L3
		                      # if count > 1
		pushq	%rbx          # save %rbx
		mvmovq	(%rdi), %rbx  # rbx=M(rdi): rbx = *start
		subq	$r8, %rsi     # rsi=rsi-1: count--
		addq	%r9, %rdi     # rdi=rdi+8: start++
		call	rproduct      # rproduct(start+1, count-1)
		imulq	%rbx, %rax    # rax=rax*rbx: rax = *start * rproduct(start+1, count-1)
		popq	%rbx          # load %rbx
		ret
	.L3:
		irmovq	$1, %rax      # rax = 1
		ret
	```

# Practice Problem 4.5
Modify the Y86-64 code for the `sum` function to implement a function `absSum` that computes the sum of absolute values of an array. Use a conditional jump instruction within your inner loop.

**Solution**:
* Original `sum` function:
	```
	# long sum(long *start, long count)
	# start in %rdi, count in %rsi
	
	sum:
		irmovq $8,%r8           # Constant 8
		irmovq $1,%r9           # Constant 1
		xorq %rax,%rax          # sum = 0
		andq %rsi,%rsi          # Set CC - condition codes
		jmp test                # Goto test
	loop:
		mrmovq (%rdi),%r10      # Get *start
		addq %r10,%rax          # Add to sum
		addq %r8,%rdi           # start++
		subq %r9,%rsi           # count--. Set CC
	test:
		jne loop                # Stop when 0
		ret                     # Return
	```
* To write `absSum` function, we need to reverse the sign bit when the number is negative.
	```
	# long absSum(long *start, long count)
	# start in %rdi, count in %rsi
	
	absSum:
		irmovq $8,%r8           # Constant 8
		irmovq $1,%r9           # Constant 1
		xorq %rax,%rax          # sum = 0
		andq %rsi,%rsi          # Set CC - condition codes
		jmp test                # Goto test
	loop:
		mrmovq (%rdi),%r10      # Get *start
		xorq %r11, %r11         # r11 = 0
		subq %r10, %r11         # r11=r11-r10: r11 = -(*start)
		jle  pos                # if(r10 >= 0), goto pos
		rrmovq %r11, %r10       # r10 = r11
	pos:
		addq %r10,%rax          # Add to sum
		addq %r8,%rdi           # start++
		subq %r9,%rsi           # count--. Set CC
	test:
		jne loop                # Stop when 0
		ret                     # Return
	```

# Practice Problem 4.6
Modify the Y86-64 code for the `sum` function to implement a function `absSum` that computes the sum of absolute values of an array. Use a conditional move instruction within your inner loop.

**Solution**:
* Original `sum` function:
	```
	# long sum(long *start, long count)
	# start in %rdi, count in %rsi
	
	sum:
		irmovq $8,%r8           # Constant 8
		irmovq $1,%r9           # Constant 1
		xorq %rax,%rax          # sum = 0
		andq %rsi,%rsi          # Set CC - condition codes
		jmp test                # Goto test
	loop:
		mrmovq (%rdi),%r10      # Get *start
		addq %r10,%rax          # Add to sum
		addq %r8,%rdi           # start++
		subq %r9,%rsi           # count--. Set CC
	test:
		jne loop                # Stop when 0
		ret                     # Return
	```
* To write `absSum` function, we need to reverse the sign bit when the number is negative.
	```
	# long absSum(long *start, long count)
	# start in %rdi, count in %rsi
	
	absSum:
		irmovq $8,%r8           # Constant 8
		irmovq $1,%r9           # Constant 1
		xorq %rax,%rax          # sum = 0
		andq %rsi,%rsi          # Set CC - condition codes
		jmp test                # Goto test
	loop:
		mrmovq (%rdi),%r10      # Get *start
		xorq %r11, %r11         # r11 = 0
		subq %r10, %r11         # r11=r11-r10: r11 = -(*start)
		cmovg %r11, %r10        # if r11 > 0, r10 = r11
		                        # if r10 <=0, r10 = r10
		addq %r10,%rax          # Add to sum
		addq %r8,%rdi           # start++
		subq %r9,%rsi           # count--. Set CC
	test:
		jne loop                # Stop when 0
		ret                     # Return
	```


# 4.1.6 Some Y86-64 Instruction Details
* 2 unusual instruction combinations require special attention.


## pushq %rsp
* The `pushq` instruction both decrements the stack pointer by 8 and writes a register value to memory.
## popq %rsp
* `popq %rsp` could either set `%rsp` to the value read from memory or to the incremented stack pointer.

# Practice Problem 4.7
Let us determine the behavior of the instruction `pushq %rsp` for an x86-64 processor. We could try reading the Intel documentation on this instruction, but a simpler approach is to conduct an experiment on an actual machine. The C compiler would not normally generate this instruction, so we must use hand-generated assembly code for this task. Here is a test function we have written:

```
.text
.globl pushtest
pushtest:
	movq %rsp, %rax   # Copy stack pointer
	pushq %rsp        # Push stack pointer
	popq %rdx         # Pop it back
	subq %rdx, %rax   # Return 0 or 4
	ret
```

In our experiments, we find that function `pushtest` always returns 0. What
does this imply about the behavior of the instruction pushq `%rsp` under x86-64?

**Solution**:
let's think over it logically:
* from rax = 4, we get that rdx = rax
* so in `pushq %rsp`, it actually save the rsp to the stack first, then decrease the rsp secondly.


# Practice Problem 4.8
The following assembly-code function lets us determine the behavior of the instruction `popq %rsp` for x86-64:

```
.text
.globl poptest
poptest:
	movq %rsp, %rdi   # Save stack pointer
	pushq $0xabcd     # Push test value
	popq %rsp         # Pop to stack pointer
	movq %rsp, %rax   # Set popped value as return value
	movq %rdi, %rsp   # Restore stack pointer
	ret
```

We find this function always returns 0xabcd. What does this imply about the
behavior of `popq %rsp`? What other Y86-64 instruction would have the exact same behavior?

**Solution**:
`popq %rsp` is equal to `mrmovq (%rsp),%rsp`.
 

