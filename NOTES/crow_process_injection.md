# Malware Development: Process Injection
https://www.youtube.com/watch?v=A6EKDAKBXPs

---------------------------------------------

## Requirements

Whats exactly is process injection.
Inject something that you want to be run, into a target.

- The workspace's folder shall be exclude from Windows Defender.
- x64 target for x64 program.

	
## TODO, NX bit???
>TODO (or looking at buffer overflow injection from Crow very first video)

## Basic injection

- Allocate virtual memory for our shell code.
- Thread/Process ID are multiple of 4.
- From MSDN documentation, look at "Windows Data Types"

### Code

- Process ID is provided to the executable from argv[] from main entry point.
- Use OpenProcess (@MSDN) with specific access right (PROCESS_VM_WRITE, we could use PROCESS_ALL_ACCESS).
	
### Result

![](process_inject.gif)

## Find Process ID

Different ways to achieve that:
1. Using cmd:
> tasklist | findstr <process_name>

2. Using System Informer.

3. Using Power shell:
> <exe> (Get-process | ?{$_.ProcessName -eq "<process>"} | Select-Object -ExpandProperty Id)` 


## Shellcode generation

### References
- Crow video from 40:00
- [Metasploit Basics, Part 9: Using msfvenom to Create Custom Payloads](https://www.hackers-arise.com/post/2017/07/31/metasploit-basics-part-9-using-msfvenom-to-create-custom-payloads)
- [ZeroTotal: Msfvenom Calc](https://steve-s.gitbook.io/0xtriboulet/zerototal/zerototal-msfvenom-calc)

As Crow, we will use the metasploit/msfvenom tool to generate a raw array of bytes for the shell code.
However, we are not going to make a reverse_tcpip. Instead, we will prefere just to open the calculator application.

### Usefull commands

- msfvenom commands always hangs on a bit.

- paramaters for msvenom command:
	- -e is for the encoder. It is usefull for evading detection and ensuring payloads are delivered successfully to the target system
		- *x86/shikata_ga_nai* (japanese for "nothing can be done about it").
	- --platform is used to specify the target platform.
		- *windows*
	- -f to specify the output format of the payload (there is two types, executable and transform formats).
		- *exe*

- payload options can be retrieve using:
	> msfvenom -p <payload_name> --list-options
	
### Step by step

- generate the shell code using:
	1 > msfvenom --platform windows --arch x64 -p windows/x64/exec CMD="calc.exe" -f c
		> Genere un shell code basique.
	2 > msfvenom --platform windows --arch x64 -p windows/x64/exec CMD="calc.exe" -f c -e x86/shikata_ga_nai -i 200
		> Genere un shell code encodé 
	3 > [CMD] > shellcode.txt
		> Genere le shell code dans un fichier texte.
		
1) 
- Work in excluded folder.
- Detected as trojan elsewhere.

2) 
- Do not work anywhere.
- Detected as trojan from outside the excluded folder.
	
## DLL generation
Is a Dynamic Linked Library.

### References
- https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
- https://www.geoffchappell.com/studies/windows/win32/kernel32/api/index.htm
- Crow video from 45:00

### Code
Implement a _BOOL WINAPI DllMain_ to create a popup windows.
Then to test it:
> rundll32 DLL_injection.dll,DllMain

## Injection
### Code
view Injection from DLL_injection solution.
