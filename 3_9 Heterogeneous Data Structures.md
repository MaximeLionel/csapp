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
	* This structure contains four fields: two 4-byte values of type int, a two-element array of type int, and an 8-byte integer pointer, giving a total of 24 bytes:
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
NOTE: the assembly code above is wrong! Please refer to the correct one below:
```
.text
.globl  st_init
st_init:
        movzwl  8(%rdi), %eax
        movw    %ax, 10(%rdi)
        leaq    10(%rdi), %rax
        movq    %rax, (%rdi)
        movq    %rdi, 16(%rdi)
        ret
```
On the basis of this information, fill in the missing expressions in the code for `st_init`.

**Solution**:
A. offsets (in bytes) of the following fields:
```
struct test {
	short *p;
	struct {
		short x;
		short y;
	} s;

	struct test *next;
};


	p: 0
	s.x: 0x8
	s.y: 0xA
	next: 0xC
```

B.
Total bytes of the structure required: 8+2+2+8=0x14

C. 
```z80
# void st_init(struct test *st)
# st in %rdi

.text
.globl  st_init
st_init:
        movzwl  8(%rdi), %eax       # eax=M(rdi+8): eax=st->x
        movw    %ax, 10(%rdi)       # M(rdi+10)=ax: st->y=st->x
        leaq    10(%rdi), %rax      # rax=rdi+10: rax=&(st->y)
        movq    %rax, (%rdi)        # M(rdi)=rax: st->p=&(st->y)
        movq    %rdi, 16(%rdi)      # M(rdi+16)=rdi: st
        ret
```

