# 4.1.1 Programmer-Visible State
## Registers
![[Pasted image 20240727231255.png|300]]
* 15 program registers: %rax, %rcx, %rdx, %rbx, %rsp, %rbp, %rsi, %rdi, and %r8 through %r14.
	* omit the x86-64 register %r15 to simplify the instruction encoding.
* Each of the registers stores a 64-bit word.
* Register %rsp is used as a stack pointer by the push, pop, call, and return instructions.
* The program counter (PC) holds the address of the instruction currently being executed.

## Memory
* The memory is conceptually **a large array of bytes**, holding both program and data.
* Y86-64 programs reference memory locations using ==virtual addresses==.
	* A combination of hardware and operating system software translates these into the **actual, or physical, addresses** indicating where the values are actually stored in memory.

## Status code (Stat)
* A final part of the program state is a **status code Stat**, indicating the overall state of program execution.
* It will indicate either normal operation or that some sort of ==exception== has occurred, such as when an instruction attempts to read from an invalid memory address.
* There are three single-bit condition codes, ZF, SF, and OF, storing information about the effect of the most recent arithmetic or logical instruction.
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
* V = 0x 00 00 00 00 00 00 00 0a = 10
* Code: `irmovq $10, %rbx`

Full code:
```
0x200 a0 6f                         pushq %rsi
0x202 80 0c 02 00 00 00 00 00 00    call func:
0x20b 00                            halt
0x20c func:
0x20c 30 f3 0a 00 00 00 00 00 00 00 irmovq $10, %rbx
```



