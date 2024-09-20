* C provides two mechanisms for creating data types by combining objects of different types:
	* structure
	* union
# 3.9.1 Structures
* The C struct declaration creates a data type that groups objects of possibly different types into a single object.
* The implementation of structures is similar to that of arrays in that all of the components of a structure are stored in a **contiguous region of memory** and a pointer to a structure is the address of its first byte.
* Example - memory layout:
	```c
	struct rec {
		int i;
		int j;
		int a[2];
		int *p;
	};
	```
	* This structure contains 4 fields: two 4-byte values of type int, a two-element array of type int, and an 8-byte integer pointer, giving a total of 24 bytes:
		![[image-20240603164441657.png|500]]
	* Array a is embedded within the structure.
* To access the fields of a structure, the compiler generates code that adds the appropriate offset to the address of the structure.
* Example - access struct element:
	* suppose variable r of type struct `rec*` is in register %rdi. Then the following code copies element r->i to element r->j:
		```z80
		# Registers: r in %rdi
		movl (%rdi), %eax              # Get r->i
		movl %eax, 4(%rdi)             # Store in r->j
		```
	* Since the offset of field i is 0, the address of this field is simply the value of r. 
	* To store into field j, the code adds offset 4 to the address of r.
* To generate a pointer to an object within a structure, we can simply add the field’s offset to the structure address.
	* we can generate the pointer `&(r->a[1])` by adding offset 8+4x1=12.
	* For pointer r in register `%rdi` and long integer variable i in register `%rsi`, we can generate the pointer value `&(r->a[i])` with the single instruction.
	```z80
	# Registers: r in %rdi, i %rsi
	leaq 8(%rdi,%rsi,4), %rax             # Set %rax to &r->a[i]
	```
	* The assembly code of `r->p = &r->a[r->i + r->j]`:
	```z80
	# Registers: r in %rdi
	movl 4(%rdi), %eax          # eax=M(rdi+4): eax=*(r+4) - Get r->j
	addl (%rdi), %eax           # eax=eax+M(rdi): eax=*(r+4)+(*r) - Add r->i which is (r->j)+(r->i)
	cltq                        # eax to rax: Extend to 8 bytes
	leaq 8(%rdi,%rax,4), %rax   # rax=rdi+4*rax+8: rax=r+4*rax+8 - Compute &r->a[r->i + r->j]
	movq %rax, 16(%rdi)         # M(rdi+16)=rax: r->p=rax - Store in r->p
	```

# Practice Problem 3.41
Consider the following structure declaration:
```C
struct test {
	short *p;
	struct {
		short x;
		short y;
	} s;

	struct test *next;
};
```
This declaration illustrates that one structure can be embedded within another, just as arrays can be embedded within structures and arrays can be embedded within arrays.

The following procedure (with some expressions omitted) operates on this structure:
```C
void st_init(struct test *st) {
	st->s.y = ______ ;
	st->p = ______ ;
	st->next = ______ ;
}
```
A. What are the offsets (in bytes) of the following fields?
```C
	p: ______
	s.x: ______
	s.y: ______
	next: ______
```
B. How many total bytes does the structure require?

C. The compiler generates the following assembly code for st_init:
```z80
# void st_init(struct test *st)
# st in %rdi

st_init:
	movl 8(%rdi), %eax
	movl %eax, 10(%rdi)
	leaq 10(%rdi), %rax
	movq %rax, (%rdi)
	movq %rdi, 12(%rdi)
	ret
```

On the basis of this information, fill in the missing expressions in the code for `st_init`.

**Solution**:
A.
```C
	p:    0
	s.x:  0x8
	s.y:  0xA
	next: 0xC
```

B. 
0x14

C.
```z80
# void st_init(struct test *st)
# st in %rdi

st_init:
	movl 8(%rdi), %eax   # eax=M(rdi+8): eax = st->x
	movl %eax, 10(%rdi)  # M(rdi+10)=eax: st->y = st->x
	leaq 10(%rdi), %rax  # rax=rdi+10: rax = &(st->y)
	movq %rax, (%rdi)    # M(rdi)=rax: st->p = &(st->y)
	movq %rdi, 12(%rdi)  # M(rdi+12)=rdi: st->next = st
	ret
```

```C
void st_init(struct test *st) {
	st->s.y = st->x;
	st->p = &(st->y);
	st->next = st;
}
```


# Analyze memory layout of structure element (take Practice Problem 3.41 as example) 
Firstly, compile the source C code (st_init.c):
```
struct test {
	short *p;
	struct {
		short x;
		short y;
	} s;

	struct test *next;
};

void st_init(struct test *st) {
	st->s.y = st->s.x;
	st->p = &(st->s.y);
	st->next = st;
}
```

`gcc -Og -g -c st_init.c`

Secondly, use objdump to view the debugging information and get the long information below:
`objdump --dwarf=info st_init.o`

```
root@ml:~/csapp/chap3/prac3_41# objdump --dwarf=info st_init.o

st_init.o:     file format elf64-x86-64

Contents of the .debug_info section:

  Compilation Unit @ offset 0x0:
   Length:        0xb1 (32-bit)
   Version:       5
   Unit Type:     DW_UT_compile (1)
   Abbrev Offset: 0x0
   Pointer Size:  8
 <0><c>: Abbrev Number: 3 (DW_TAG_compile_unit)
    <d>   DW_AT_producer    : (indirect string, offset: 0x0): GNU C17 11.4.0 -mtune=generic -march=x86-64 -g -Og -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
    <11>   DW_AT_language    : 29       (C11)
    <12>   DW_AT_name        : (indirect line string, offset: 0x0): st_init.c
    <16>   DW_AT_comp_dir    : (indirect line string, offset: 0xa): /root/csapp/chap3/prac3_41
    <1a>   DW_AT_low_pc      : 0x0
    <22>   DW_AT_high_pc     : 0x18
    <2a>   DW_AT_stmt_list   : 0x0
 <1><2e>: Abbrev Number: 4 (DW_TAG_structure_type)
    <2f>   DW_AT_byte_size   : 4
    <30>   DW_AT_decl_file   : 1
    <31>   DW_AT_decl_line   : 3
    <32>   DW_AT_decl_column : 2
    <33>   DW_AT_sibling     : <0x4c>
 <2><37>: Abbrev Number: 1 (DW_TAG_member)
    <38>   DW_AT_name        : x
    <3a>   DW_AT_decl_file   : 1
    <3a>   DW_AT_decl_line   : 4
    <3b>   DW_AT_decl_column : 9
    <3c>   DW_AT_type        : <0x4c>
    <40>   DW_AT_data_member_location: 0
 <2><41>: Abbrev Number: 1 (DW_TAG_member)
    <42>   DW_AT_name        : y
    <44>   DW_AT_decl_file   : 1
    <44>   DW_AT_decl_line   : 5
    <45>   DW_AT_decl_column : 9
    <46>   DW_AT_type        : <0x4c>
    <4a>   DW_AT_data_member_location: 2
 <2><4b>: Abbrev Number: 0
 <1><4c>: Abbrev Number: 5 (DW_TAG_base_type)
    <4d>   DW_AT_byte_size   : 2
    <4e>   DW_AT_encoding    : 5        (signed)
    <4f>   DW_AT_name        : (indirect string, offset: 0x9a): short int
 <1><53>: Abbrev Number: 6 (DW_TAG_structure_type)
    <54>   DW_AT_name        : (indirect string, offset: 0xa9): test
    <58>   DW_AT_byte_size   : 24
    <59>   DW_AT_decl_file   : 1
    <5a>   DW_AT_decl_line   : 1
    <5b>   DW_AT_decl_column : 8
    <5c>   DW_AT_sibling     : <0x82>
 <2><60>: Abbrev Number: 1 (DW_TAG_member)
    <61>   DW_AT_name        : p
    <63>   DW_AT_decl_file   : 1
    <63>   DW_AT_decl_line   : 2
    <64>   DW_AT_decl_column : 9
    <65>   DW_AT_type        : <0x82>
    <69>   DW_AT_data_member_location: 0
 <2><6a>: Abbrev Number: 1 (DW_TAG_member)
    <6b>   DW_AT_name        : s
    <6d>   DW_AT_decl_file   : 1
    <6d>   DW_AT_decl_line   : 6
    <6e>   DW_AT_decl_column : 4
    <6f>   DW_AT_type        : <0x2e>
    <73>   DW_AT_data_member_location: 8
 <2><74>: Abbrev Number: 7 (DW_TAG_member)
    <75>   DW_AT_name        : (indirect string, offset: 0xa4): next
    <79>   DW_AT_decl_file   : 1
    <7a>   DW_AT_decl_line   : 8
    <7b>   DW_AT_decl_column : 15
    <7c>   DW_AT_type        : <0x87>
    <80>   DW_AT_data_member_location: 16
 <2><81>: Abbrev Number: 0
 <1><82>: Abbrev Number: 2 (DW_TAG_pointer_type)
    <83>   DW_AT_byte_size   : 8
    <83>   DW_AT_type        : <0x4c>
 <1><87>: Abbrev Number: 2 (DW_TAG_pointer_type)
    <88>   DW_AT_byte_size   : 8
    <88>   DW_AT_type        : <0x53>
 <1><8c>: Abbrev Number: 8 (DW_TAG_subprogram)
    <8d>   DW_AT_external    : 1
    <8d>   DW_AT_name        : (indirect string, offset: 0x92): st_init
    <91>   DW_AT_decl_file   : 1
    <92>   DW_AT_decl_line   : 11
    <93>   DW_AT_decl_column : 6
    <94>   DW_AT_prototyped  : 1
    <94>   DW_AT_low_pc      : 0x0
    <9c>   DW_AT_high_pc     : 0x18
    <a4>   DW_AT_frame_base  : 1 byte block: 9c         (DW_OP_call_frame_cfa)
    <a6>   DW_AT_call_all_calls: 1
 <2><a6>: Abbrev Number: 9 (DW_TAG_formal_parameter)
    <a7>   DW_AT_name        : st
    <aa>   DW_AT_decl_file   : 1
    <ab>   DW_AT_decl_line   : 11
    <ac>   DW_AT_decl_column : 27
    <ad>   DW_AT_type        : <0x87>
    <b1>   DW_AT_location    : 1 byte block: 55         (DW_OP_reg5 (rdi))
 <2><b3>: Abbrev Number: 0
 <1><b4>: Abbrev Number: 0
```

Thirdly, analyze the structure element:
we know the struct in C:
```
struct test {
	short *p;
	struct {
		short x;
		short y;
	} s;

	struct test *next;
};
```

For each DIE(debugging information entry), it's like:
- `DW_AT_name`: element name
- `DW_AT_type`: point to another DIE, which describe the element type
- `DW_AT_data_member_location`: offset to the start of the structure

For example, let's analyze the size of `s`
* we get the size of `test` is 24 bytes:
	```
	<1><53>: Abbrev Number: 6 (DW_TAG_structure_type)
	    <54>   DW_AT_name        : (indirect string, offset: 0xa9): test
	    <58>   DW_AT_byte_size   : 24
	```
* the offset of `s` in structure `test` is 8:
	```
	 <2><6a>: Abbrev Number: 1 (DW_TAG_member)
	    <6b>   DW_AT_name        : s
	    <6d>   DW_AT_decl_file   : 1
	    <6d>   DW_AT_decl_line   : 6
	    <6e>   DW_AT_decl_column : 4
	    <6f>   DW_AT_type        : <0x2e>
	    <73>   DW_AT_data_member_location: 8   # offset
	```
* the offset of `next` in structure test is 16:
	```
	<2><74>: Abbrev Number: 7 (DW_TAG_member)
	    <75>   DW_AT_name        : (indirect string, offset: 0xa4): next
	    <79>   DW_AT_decl_file   : 1
	    <7a>   DW_AT_decl_line   : 8
	    <7b>   DW_AT_decl_column : 15
	    <7c>   DW_AT_type        : <0x87>
	    <80>   DW_AT_data_member_location: 16  # offset
	```
* Thus we know the size of `s` is 8 bytes. 
* Then we check the element in `s` struct:
	```
	 <2><37>: Abbrev Number: 1 (DW_TAG_member)
	    <38>   DW_AT_name        : x
	    <3a>   DW_AT_decl_file   : 1
	    <3a>   DW_AT_decl_line   : 4
	    <3b>   DW_AT_decl_column : 9
	    <3c>   DW_AT_type        : <0x4c>
	    <40>   DW_AT_data_member_location: 0    # offset
	 <2><41>: Abbrev Number: 1 (DW_TAG_member)
	    <42>   DW_AT_name        : y
	    <44>   DW_AT_decl_file   : 1
	    <44>   DW_AT_decl_line   : 5
	    <45>   DW_AT_decl_column : 9
	    <46>   DW_AT_type        : <0x4c>
	    <4a>   DW_AT_data_member_location: 2    # offset
	```
* We get the `x` and `y` only take 4 bytes, so the 4 bytes left is like for 8 bytes alignment requirement.

# Practice Problem 3.42
The following code shows the declaration of a structure of type ACE and the prototype for a function test:
```C
struct ACE {
	short v;
	struct ACE *p;
};
short test(struct ACE *ptr);
```
When the code for fun is compiled, gcc generates the following assembly code:
```z80
# short test(struct ACE* ptr)
# ptr in %rdi

test:
	movl $1, %eax
	jmp .L2
	.L3:
	imulq (%rdi), %rax
	movq 2(%rdi), %rdi
	.L2:
	testq %rdi, %rdi
	jne .L3
	rep; ret
```
A. Use your reverse engineering skills to write C code for test.

B. Describe the data structure that this structure implements and the operation performed by test.

**Solution**:
A.
```z80
# short test(struct ACE* ptr)
# ptr in %rdi

test:
	movl $1, %eax            # eax=1
	jmp .L2
	.L3:
	imulq (%rdi), %rax      # rax=rax*M(rdi): rax=rax*(*ptr)
	movq 2(%rdi), %rdi      # rdi=M(rdi+2): rdi=*(rdi+2)
	.L2:
	testq %rdi, %rdi
	jne .L3                 # if(rdi!=0), jump to .L3
	rep; ret
```
* Reversed C code:
```C
short test(struct ACE* ptr)
{
	int result = 1;
	while(ptr != 0)
	{
		result = result * (ptr->v);
		ptr = ptr->p;
	}
}
```

B.
The function is to compute the cumulative product of the element values in a singly linked list.


# 3.9.2 Unions
* Unions provide a way to circumvent the type system of C, allowing a single object to be referenced according to multiple types.
## Example - compare struct and union:

```C
	struct S3 {
		char c;
		int i[2];
		double v;
	};
	
	union U3 {
		char c;
		int i[2];
		double v;
	};
```
* When compiled on an x86-64 Linux machine, the offsets of the fields, as well as the total size of data types S3 and U3, are as shown in the following table:
		![[image-20240605095905508.png|200]]
* For pointer p of type union `U3*`, references `p->c`, `p->i[0]`, and `p->v` would all reference the beginning of the data structure.
* The overall size of a union equals the maximum size of any of its fields.
## Example - 2 mutually exclusive fields in a union to save space:
* One application is when we know in advance that the use of two different fields in a data structure will be mutually exclusive. Then, declaring these two fields as part of a union rather than a structure will reduce the total space allocated.
```C
struct node_s {
	struct node_s *left;
	struct node_s *right;
	double data[2];
};

union node_u {
	struct {
		union node_u *left;
		union node_u *right;
	} internal;
	double data[2];
};
```
* For `node_s`, every node requires 32 bytes, with half the bytes wasted for each type of node.
* For `node_u`, every node will require just 16 bytes.
* If n is a pointer to a node of type union `node_u *`, we would reference the data of a leaf node as `n->data[0]` and `n->data[1]`, and the children of an internal node as `n->internal.left` and `n->internal.right`.
* With this encoding, however, there is no way to determine whether a given node is a leaf or an internal node.
## Example - introduce an enumerated type
* A common method is to introduce an enumerated type defining the different possible choices for the union, and then create a structure containing a tag field and the union:
```C
typedef enum { N_LEAF, N_INTERNAL } nodetype_t;

struct node_t {
	nodetype_t type;
	union {
		struct {
			struct node_t *left;
			struct node_t *right;
		} internal;
	double data[2];
	} info;
};
```
* This structure requires a total of 24 bytes: 4 for type, and either 8 each for `info.internal.left` and `info.internal.right` or 16 for `info.data`.
* An additional 4 bytes of padding is required between the field for type and the union elements, bringing the total structure size to 4 + 4 + 16 = 24, which we will talk about on data alignment part.

## Example - access bit patterns
* Suppose we use a simple cast to convert a value d of type double to a value u of type unsigned long:
	```C
	unsigned long u = (unsigned long) d;
	```
	* Value u will be an integer representation of d.
* Consider the following code to generate a value of type unsigned long from a double:
	```C
	unsigned long double2bits(double d) {
		union {
			double d;
			unsigned long u;
		} temp;
		temp.d = d;
		return temp.u;
	};
	```
	* In this code, we store the argument in the union using one data type and access it using another.
		* u will have the same bit representation as d, including fields for the sign bit, the exponent, and the significand.

## Example - byte-ordering issues
* When using unions to combine data types of different sizes, byte-ordering issues can become important.
```C
double uu2double(unsigned word0, unsigned word1)
{
	union {
		double d;
		unsigned u[2];
	} temp;
	temp.u[0] = word0;
	temp.u[1] = word1;
	return temp.d;
}
```
* On a little-endian machine, such as an x86-64 processor, argument `word0` will become the low-order 4 bytes of d, while `word1` will become the high-order 4 bytes. 
* On a big-endian machine, the role of the two arguments will be reversed.

# Practice Problem 3.43
Suppose you are given the job of checking that a C compiler generates the proper code for structure and union access. You write the following structure declaration:
```C
typedef union {
	struct {
		long u;
		short v;
		char w;
	} t1;
	
	struct {
		int a[2];
		char *p;
	} t2;
} u_type;
```
You write a series of functions of the form:
```C
void get(u_type *up, type *dest) {
	*dest = expr;
}
```
with different access expressions `expr` and with destination data type type set according to type associated with `expr`. You then examine the code generated when compiling the functions to see if they match your expectations.

Suppose in these functions that `up` and `dest` are loaded into registers `%rdi` and `%rsi`, respectively. Fill in the following table with data type type and sequences of one to three instructions to compute the expression and store the result at dest.

|        expr        | type |                    Code                    |
| :----------------: | :--: | :----------------------------------------: |
|      up->t1.u      | long | `movq (%rdi), %rax`<br>`movq %rax, (%rsi)` |
|      up->t1.v      |      |                                            |
|     &up->t1.w      |      |                                            |
|      up->t2.a      |      |                                            |
| up->t2.a[up->t1.u] |      |                                            |
|     *up->t2.p      |      |                                            |
**Solution**:
```C
typedef union {
	struct {
		long u;
		short v;
		char w;
	} t1;
	
	struct {
		int a[2];
		char *p;
	} t2;
} u_type;
```

```C
void get(u_type *up, type *dest) {
	*dest = expr;
}
```

| expr               | type  | Code                                                                     | Actual compiled asm code                                                          |
| :----------------- | :---- | :----------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| up->t1.u           | long  | `movq (%rdi), %rax`<br>`movq %rax, (%rsi)`                               | `movq    (%rdi), %rax`<br>`movq    %rax, (%rsi)`                                  |
| up->t1.v           | short | `movw 8(%rdi), %ax`<br>`movw %ax, (%rsi)`                                | `movzwl  8(%rdi), %eax`<br>`movw    %ax, (%rsi)`                                  |
| &up->t1.w          | char* | `leaq 10(%rdi), %rax`<br>`movq %rax, (%rsi)`                             | `addq    $10, %rdi`<br>`movq    %rdi, (%rsi)`                                     |
| up->t2.a           | int*  | `movq %rdi, (%rsi)`                                                      | `movq    %rdi, (%rsi)`                                                            |
| up->t2.a[up->t1.u] | int   | `movq (%rdi), %rax`<br>`movl (%rdi,%rax,4), %eax`<br>`movl %eax, (%rsi)` | `movq    (%rdi), %rax`<br>`movl    (%rdi,%rax,4), %eax`<br>`movl    %eax, (%rsi)` |
| *up->t2.p          | char  | `movq 8(%rdi), %rax`<br>`movb (%rax), %al`<br>`movb %al, (%rsi)`         | `movq    8(%rdi), %rax`<br>`movzbl  (%rax), %eax`<br>`movb    %al, (%rsi)`        |

# 3.9.3 Data Alignment - 数据对齐
* Many computer systems place restrictions on the allowable addresses for the primitive data types, requiring that the address for some objects must be a multiple of some value K (typically 2, 4, or 8).
	![[image-20240605170724672.png|150]]
* Such alignment restrictionssimplify the design of the hardware forming the interface between the processor and the memory system.
	* For example, suppose a processor always fetches 8 bytes from memory with an address that must be a multiple of 8.
* Alignment is enforced by making sure that every data type is organized and allocated in such a way that every object within the type satisfies its alignment restrictions.
* The compiler places directives in the assembly code indicating the desired alignment for global data：
	```
	.align 8
	```
	* This ensures that the data following it (in this case the start of the jump table) will start with an address that is a multiple of 8.
## Example 1
```C
struct S1 {
	int i;
	char c;
	int j;
};
```
* Suppose the compiler used the minimal 9-byte allocation:
	![[image-20240605171856098.png|200]]
* To satisfy the 4-byte alignment requirement, it will be:
	![[image-20240605171942336.png|200]]
	* The compiler inserts a 3-byte gap between fields c and j.
	* The overall structure size is 12 bytes.

## Example 2
```C
struct S2 {
	int i;
	int j;
	char c;
};
```
* For `struct S2 d[4]`, the compiler allocates 12 bytes for structure S2, with the final 3 bytes being wasted space:
	![[image-20240606102920272.png|250]]
	* The elements of d will have addresses $x_d$, $x_d + 12$, $x_d + 24$, and $x_d + 36$.

# Practice Problem 3.44
For each of the following structure declarations, determine the offset of each field, the total size of the structure, and its alignment requirement for x86-64:

A. `struct P1 { short i; int c; int *j; short *d; }`;

B. `struct P2 { int i[2]; char c[8]; short s[4]; long *j; }`;

C. `struct P3 { long w[2]; int *c[2] }`;

D. `struct P4 { char w[16]; char *c[2] }`;

E. `struct P5 { struct P4 a[2]; struct P1 t }`;

**Solution**:
A.`struct P1 { short i; int c; int *j; short *d; }` // the answer on book is wrong.

| Field | Offset | Comment                 |
| ----- | ------ | ----------------------- |
| i     | 0      |                         |
| c     | 2      | padding byte6 and byte7 |
| j     | 8      |                         |
| d     | 16     |                         |
Alignment: 8 bytes
Total Size: 24 bytes

B. `struct P2 { int i[2]; char c[8]; short s[4]; long *j; }`

| Field | Offset | Comment |
| ----- | ------ | ------- |
| i     | 0      |         |
| c     | 8      |         |
| s     | 16     |         |
| j     | 24     |         |
Alignment: 8 bytes
Total Size: 32 bytes

C. `struct P3 { long w[2]; int *c[2] }`

| Field | Offset | Comment         |
| ----- | ------ | --------------- |
| w     | 0      |                 |
| c     | 16     | take 2\*8 bytes |
Alignment: 8 bytes
Total Size: 32 bytes

D. `struct P4 { char w[16]; char *c[2] }`

| Field | Offset | Comment         |
| ----- | ------ | --------------- |
| w     | 0      |                 |
| c     | 16     | take 2\*8 bytes |
Alignment: 8 bytes
Total Size: 32 bytes

E. `struct P5 { struct P4 a[2]; struct P1 t }`

| Field | Offset | Comment          |
| ----- | ------ | ---------------- |
| a     | 0      | take 32\*2 bytes |
| t     | 64     | take 24\*1 bytes |
Alignment: 8 bytes
Total Size: 88 bytes

# Practice Problem 3.45
Answer the following for the structure declaration：
```C
struct {
	int *a;
	float b;
	char c;
	short d;
	long e;
	double f;
	int g;
	char *h;
} rec;
```
A. What are the byte offsets of all the fields in the structure?

B. What is the total size of the structure?

C. Rearrange the fields of the structure to minimize wasted space, and then show the byte offsets and total size for the rearranged structure.

**Solution**:
A.

| Field | Offset | Comment                                                                                                                        |
| ----- | ------ | ------------------------------------------------------------------------------------------------------------------------------ |
| a     | 0      | pointer                                                                                                                        |
| b     | 8      | take 4 bytes for `float` type                                                                                                  |
| c     | 12     | take 1 byte for `char` type and 1 byte for padding                                                                             |
| d     | 14     | take 2 bytes for `short` type, because `short` is 2 bytes and `K` will 2, which means the address of d must be multiples of 2. |
| e     | 16     | take 8 bytes for `long` type                                                                                                   |
| f     | 24     | take 8 bytes for `double` type                                                                                                 |
| g     | 32     | take 4 bytes for `int` type and 4 bytes for padding                                                                            |
| h     | 40     | take 8 bytes for `char*` type                                                                                                  |
We may check through `objdump --dwarf=info struct.o`

B.
Totally, 48 bytes.

C.
Logically, because the alignment is 8 bytes, we use blocks of 8 bytes as few as possible. It's partly like tetris (俄罗斯方块) .

Considering the total size, it's already the best optimization which is 48 bytes.