# 23/06/24
- https://www.youtube.com/watch?v=aNEqC-U5tHM&list=PL_z_ep2nxC57sHAlCcvvaYRrpdMIQXri1 
	- https://www.crow.rip/crows-nest/mal/dev/getting-started
	- Blog/DOCS/mlw_0/lecture.md
	- https://www.elastic.co/fr/blog/ten-process-injection-techniques-technical-survey-common-and-trending-process
	
- Sur [CDN](https://blog.csdn.net/2301_76168381/article/details/136567947?ops_request_misc=&request_id=&biz_id=102&utm_term=malware%20%E5%AD%A6%E4%B9%A0%E7%B3%BB%E5%88%97&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-7-136567947.142^v100^control&spm=1018.2226.3001.4187)
un site chinois, de très très bon liens.

- Parmis eux [celui-ci](https://github.com/joe-shenouda/awesome-cyber-skills?tab=readme-ov-file)

# 27/06/24
- [Binary Injection](https://www.youtube.com/watch?v=A6EKDAKBXPs)
- updated NOTES/corw_process_injection.md
- Reception STM32F7 + NAND Flash

# 28/06/24
- [Overflow Exploit](https://www.youtube.com/watch?v=6sUd3AA7Q50)
- https://www.youtube.com/watch?v=RU5vUIl1vRs
- https://ropemporium.com/
> Going to GDP-peta and ROP emporium challenges.
- [](./NOTES/crow_buffer_overflow.md)

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
1. Nouvelle série sur YT
	https://www.youtube.com/watch?v=hH0Pss7PGVE
2. Install Kali Linux sur WSL
	https://www.kali.org/docs/wsl/wsl-preparations/
3. Jolie DKPG
	https://medium.com/@Kuber19/enhancing-your-re-experience-with-pwndbg-tmux-459d271105f2

## TODO
- Look at Blog/DOCS/usb_1/text.txt
- [Pwn - Bypass NX/ASLR via leak de la libc](https://www.youtube.com/watch?v=8skdPGCEuxE)
	- ASLR (Adress Space Layout Randomization)
- [Binary Exploit](https://github.com/RPISEC/MBE/tree/master?tab=readme-ov-file)
- [Malware Analysis](https://github.com/RPISEC/Malware)
- References from https://blog.csdn.net/2301_76168381/article/details/136567947
- APC queue injection