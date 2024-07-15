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
1. Configuration at https://github.com/anarticho/Kali_machine/commit/9ff85f718ea0d59f74d739fe6c4dac36cb4439fd.
2. Coming back to flipboard video.


# 07/07/24
1. https://valsamaras.medium.com/introduction-to-x64-linux-binary-exploitation-part-1-14ad4a27aeef
2. Review follow ups.
3. Having a backup for WSL2 (C:\...\Desktop\Backup Kali)
4. Remove github repository from kali
5. Day flow:
	1. Figuring out that python3 provide Unicode Text String output, when python2 was Byte String.
    (https://discuss.python.org/t/unusal-behavior-of-python3-print-hex-values/15418/8)
	2. Let's create a C++ program to generate byte-text string to be used as "python -c 'print(..)'" into gdb run $() 
    > VS_work/binary_stdout <br>
        > C++ with main entry point + Translater class. <br>
        > Make file. <br>

# 10/07/24
1. Moved tgdb from ~/tools to newly created ~/bin.
2. Configure Visual Studio Code Debug
	- Following thoses instructions (https://hackernoon.com/how-to-set-up-c-debugging-in-vscode-using-a-makefile)
	- CTRL + P
		- Tasks: Run tasks
		- Debug: Start Debug
3. New **binject** executable to replace *python2 -c* Code into https://github.com/anarticho/binject
4. Executable added to bin/
5. Successfully used with gdb using:
> r $(binject -s 123)

6. Reorganized this folder.
7. Review workflow
8. Successfully jumping to hackme function
9. ret2win ROP challenge cracked !
10. Try with another program:
	- VS_work/buffer_overflow directory.
	- https://www.youtube.com/watch?v=hH0Pss7PGVE code.

11. This method do not work to print from returned function, while GDB debugging session
is effectivly returning to this function?.

12. From https://dhavalkapil.com/blogs/Buffer-Overflow-Exploit/
	- Cracked (VS_work/buffer_overflow/main2.c + SOLUTIONS.md)
	- However, only works by debugging with GDB.
13. Some hypotesis about that:
	- PIE and ASLR?
	- printf getting corrupted?

# 11/07/24
1. It appears that both main and main2 have the same problem. Investigating that.
> checksec --file=ret2win 

2. Informs that the ROP's ret2win executable do not have the PIE enabled, while for main and main2 ones, it is activated.

3. About **PIE mechanism**, found this [website](https://ir0nstone.gitbook.io/notes/types/stack/pie)
and this [one too](https://ir0nstone.gitbook.io/notes/other/pwntools/introduction)

4. Removed python2 as we will use binject to perform the exact same functionnality as:

5. Sauvagarde Kali at "C:\Users\elena\Desktop\Backup Kali\11_07.tar"

# 12/07/24
- NOTES_07_01_split.md

# 13/07/24
- Start ROP's callme challenge

# 14/07/24
- NOTES_07_02_callme.md
- Modifications to binject

# 15/07/24
- Start write4 challenge
- NOTES_07_03_write.md

