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