
# split chalenge

Coming back to ROP emporium challengs, on split one.
https://ropemporium.com/challenge/split.html

Looking at the begginer's guide, some new tools:
- https://github.com/sashs/Ropper
 	- To find ROP gadgets within executable
	- ropper --file split --search "ret"
- https://github.com/Gallopsled/pwntools
	- Not now, will be really usefull when leading with ASLR.
- https://github.com/radare/radare2
	- mainly rabin2 to find strings within executable
	- "rabin2 -z split"

## The wall

This one was really hard, but we did it !
We first tried to just return to the 'usefulFunction' containing the 'system' call with '/bin/ls' as parameter using buffer overflow with:
> binject -ns 40 A -x 4207400000000000

Whithout having nothing more than a strange segmentation fault within the 'system' call, precisely at the 'movaps %xmm0, 0x50(%rsp)'. Searching a bit about this, it appears that the problem was a a misalignement of the stack...

https://book.hacktricks.xyz/binary-exploitation/rop-return-oriented-programing#stack-alignment
https://stackoverflow.com/a/74755766

It can be a bit tricky to understand, but this 'movaps' instruction from system call need the stack to be 16-bit aligned (on hexadecimal, last digit to 0). Align stack is pretty easy, by just asserting a ret instruction, it increase the stack (and its RSP) by 8 bytes (as it pop the return address from the stack, on x64 its 8 bytes).

## The key

Using ropper:
> ropper --file split --search "ret"

We can easily find the address for a return instruction within the program.
Overflowing the vulnerable buffer on stack, we can overwrite the return address with the return one. It is a ROT gadget.

> binject -ns 40 A -x 3E05400000000000 -x 4207400000000000

For program with ROP gadget, and the one without, here is the values of %RSP after the 'read' call:

Next to 			| ROP +RET	| Just RET
------------------- | --------- | -------
CALL <read>			| 0xB0		| 0xB0
LEAVE				| 0xD8		| 0xD8
RET					| 0xE0		| -
RET (extra ROP) 	| 0xE8		| 0xE0 
PUSH %RBP			| 0xE0		| 0xD8
AT CALL <system>	| 0xE0		| 0xD8

## The challenge

Next step is to completely build a ROP chain.
Before, reading some explanation about x64 calling convention:
https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/linux-x64-calling-convention-stack-frame

Arguments at function call are not pushed to the stack under very strict conditions. Instead, they are stored within conventionals registers. It is pretty visual if we look at the assembly code for the 'usefulFunction':
> mov    $0x40084a,%edi
> call   0x400560 <system@plt>

In this case, the address 0x40084a is containing the parameter to be provided to the 'system' call, that we want to be on the %RDI buffer to be recovered by the fonction by convention.

For this challenge, we have to provide an other argument to the system call. It means that we have to ensure having our new argument loaded to %RDI before performing the call. This can be acheived by having a ROP gadget as:
> pop %rdi; ret;

Thus, we have to ensure that our new argument is just next to this gadget address within the stack, to be poped within %RDI register.

We then have:
> binject -ns 40 A -x C307400000000000 -x 6010600000000000 -x 4B07400000000000

As with:
- C307400000000000 the address for 'pop %rdi; ret' gadget,
- 6010600000000000 the address for '/bin/cat flag.txt' string,
- 4B07400000000000 the address for 'call <system>' instruction.

## Callme 
https://ropemporium.com/challenge/callme.html

It will be about PLT (Procedure Linkage Table). 

Interresting introduction from: ROP website:
https://ropemporium.com/guide.html#Appendix%20A

More detailled content here:
https://ir0nstone.gitbook.io/notes/types/stack/aslr/plt_and_got

*To resume*: <br>
Extenal functions are not appended to the **.text**/**.code** section of the compiled program, but rather dedicated calls are deferred to the **PLT** procedure. Basically, it is a bunch of functions from binary **.plt**, **.got** and **.got.plt** sections that allow to dynamically resolve its symbol and build a "look-up table" mechanism to optimize function calls.

### PLT

https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries/

Using gdb, we can retrieve the differents sections from a binary using the command:
> info files

Then, we can display the instructions located at **.plt** address using:
> x/20i ADDRESS_.plt

Obviously, 20 is arbitrary, and it really depend on your program (and how many external variables and functions it have to resolve). Shortly, the .plt have:
- PLT[0] Some assembly instructions for the resolver routine (as dynamic loader itself in UNIX systems).
- PLT[N], as function routine to load address from **.got.plt** section. If not resolved, it will contain instruction address within PLT[N] to forward to resolver routine (at PLT[0]).

It is really important to understant that the code for thoses external **@plt** functions is located within this exact same **.plt** section, refeering to the global offset table, aka GOT.

## GOT

Some clarifications about the **.got.plt**; first, do not care about **.got** and **.got.plt**, we can say that they are something like the same thing. Most important, it contains addresses. It means that when PLT[N] jump to GOT[N], it will basically jump to the address contained by related **.got** section!

Here is its structure of 8 bytes elements, +0x08, +0x10 for the offsets:
- *GOT[0]*: records the first address of the **.dynamic section**, which records the basic information required by the dynamic linker, such as the symbol table location, relocation table location, etc.
- *GOT[1]*: records the identification information of the instantiated dynamic linker.
- *GOT[2]*: records the entry address of the dynamic linker delayed binding code <_dl_runtime_resolve_xsavec>.
- *GOT[3]*: records the address to \<**printf@plt**>, as the first entry of GOT table.

## Resolution

https://img-blog.csdnimg.cn/20200208160601515.gif
https://blog.csdn.net/Ruger008/article/details/107062608

At executing a program, the DL (dynamic linker, ld.so or ld-linux.so) is responsible for loading and initializing GOT entries, including GOT[1] and GOT[2], during the execution of a dynamically linked program. It manages the process of symbol resolution and updates GOT entries with the correct addresses of functions and variables from shared libraries, ensuring smooth and efficient runtime operation.

All the other addresses from **.got.plt** section (i.e. GOT[>=3]) points to related PLT instruction, as to be resolved by the DL later.

Then, imagine having unique external printf function to resolve through **PLT**:

1. **.code** instruction reach a **call** <...@plt> instruction.
2. it jumps to dedicated PLT[1] within **.plt**.
3. it jumps to the address pointed to GOT[3], that return to PLT[1] (next to instruction from 2.).
4. it push 0 to the stack and jumpts to PLT[0] as for the function entry offset.
5. it push GOT[1] on stack.
6. it jumps to adress from GOT[2] (as resolve function), parameters on stack are (function entry offset, dynamic_linker).

At this last point, the kernel dedicated binary will compute the symbol and then move it to GOT[3]. Then, each time \<**printf@plt**> will be called, instead of coming back to PLT[1] at *3.*, it will directly jump to the resolved symbol.
