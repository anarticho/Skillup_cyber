# Buffer Overflows

## References
https://www.youtube.com/watch?v=6sUd3AA7Q50

## What is the point
- WHAT? Give process that index an array of elements, more elements that its effective size.

- WHEN? This exploit can not be used anywhere, only if buffer overflow was protected.

-  WHY? To run arbitrary code within OS without having to explicitly load it.

## Example
Simple C program that ask for user to enter some input values.

- Compile warning (buffer size is less than size's parameter of read function)
- Segmentation fault.

## GDB peda
- Disassemby + debugging in real-time.
- All in assembly.
- Binary shall be "not-striped".

## Exploits
### Type
1. segault/crash.
2. eip-offset.
3. eip-overwrite.
4. shellcode/REt2*-bad attacks.
5. pwned.

Similar pattern.

### Stack-based exploitation guideline
1. Crash or overflow the program.
2. Find out how many bites it took to crash the program (it is the EIP offset).
3. Confirm the offset with an EIP 'overwrite'.
4. Either:
    1. Use/create shellcode to exploit the program, be carreful that it will not work if there is:
        1. No Execute (NX) Bit mechanism.
        2. Dead Execution Protection (DEP) mechanism.
    2. Utilize a ret2* based attack.
5. Make the program do whatever you want it to do.

## GCC Programing Registers
### EIP register
**Instruction Pointer** register. _Where the program have to go next to current instruction beign processed_.

### ESP register
Different from **ESP register** (Stack pointer).
Where the stack is within memory.
Push/pop mechanism.

### EBP register
Base pointer (where i am within the stack).

## Stack and ESP in assembly
### References
https://www.youtube.com/watch?v=6sUd3AA7Q50&t=15s
### Procedure
1. At the instruction for calling a function:
    - Decrements the ESP register value.
    - Push EIP value at ESP register value.
    - EIP have address of instruction to execute next to calling function.

2. Move to first instruction of the function (EIP change coresspondigly). 

3. At reaching RET instruction, pop last value from stack (from 1.) onto put it into EIP register to be executed.

## ROP Emporium

GOGOGO