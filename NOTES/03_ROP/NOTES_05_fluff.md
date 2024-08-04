# fluff challenge

## Introduction

L'énoncé est d'autant plus court qu'obscure... Il nous parle d'une instruction `mov` et de la fonction `questionableGadgets`, qui contient quelques instructions notables, propres à l'architecture *CISC* du x86. Celles-ci peuvent s'assimiler à un ensemble d'instructions *réduites* réunies en une seule et même instruction *complexe*. Ci-dessous les instructions contenues dans les gadgets:

---
<br>

>   0x0000000000400628 <+0>:     xlat   %ds:(%rbx)      <br>
>   0x0000000000400629 <+1>:     ret                    <br>

Cette première instruction `xlat` (**transLATe**) effectue un `mov` tel que:
- `%rbx` le registre **source**; contient le **byte** à *mov*.
- `%al` (LSB de `%rax`) utilisé comme:
    - le registre de **control**; contient l'offset (en bits) à récupérer.
    - le registre de **destination**;
    
---
<br>

>   0x0000000000400633 <+11>:    bextr  %rdx,%rcx,%rbx  <br>
>   0x0000000000400638 <+16>:    ret                    <br>

La seconde instruction `bextr` (**Bit field EXTRact**) permet de récuper un set de bits tel que:
- `%rcx` le registre **source**.
- `%rdx` le registre de **control**;
    - bits [0:7] pour l'offset du set à récupérer.
    - bits [8:15] pour le nombre de bits à récupérer.
- `%rbx` le registre **destination**.

---
<br>

>   0x0000000000400639 <+17>:    stos   %al,%es:(%rdi)  <br>
>   0x000000000040063a <+18>:    ret                    

Cette dernière instruction `stos` (**STOre String**) effectue un `mov` tel que:
- `%al` (LSB de `%rax`) le registre **source**.
- `rdi` le registre **destination**; contenant l'addresse en mémoire où stocker le **byte**.

---
<br>

Il existe aussi un gadget `pop` afin de charger le paramètre pour l'appel de la fonction `print_file`; il servira tout aussi bien à charger l'addresse mémoire pour l'instruction `stos`.
> 0x00000000004006a3: pop rdi; ret;

### References:
Quelques liens intéressants pour chacunes de ces trois instructions:
- https://www.acepace.net/2019-07-27-xlatb/
- https://stackoverflow.com/a/70208912/26461955
- https://www.felixcloutier.com/x86/stos:stosb:stosw:stosd:stosq

## Premier jet

Ma première idée fut la suivante:
1. `bextr` pour placer un **byte** de `flag.txt` dans `%rbx`.
2. `xlat` pour récupérer ce byte et la placer dans `%rax`.
3. `stos` pour stocker ce byte en mémoire avec `%rdx`.

Et ce pour chacun des bytes de `flag.txt`; mais bien que le concept soit bon, je me suis vite retrouvé confronté à quelques problématiques que je vais tenter d'éclaircir ici.

---
<br>

Le gadget `bextr` contient les instructions `pop`, ce qui convient parfaitement pour charger, depuis la stack, les valeurs de **control** et de **source**, dans leur registres respectifs, `%rcx` et `%rdx`. 

>   0x000000000040062a <+2>:     pop    %rdx            <br>
>   0x000000000040062b <+3>:     pop    %rcx            <br>
>   0x000000000040062c <+4>:     add    $0x3ef2,%rcx    <br>

Néanmoins l'instruction `add` vient quelque peu perturber le *flow* de l'exploit, et oblige à préalablement calculer la valeur **source** (contenue dans `%rcx`). Dans l'idée, la valeur de **control** du registre `%rdx` 

---
<br>

Le gadget `xlat` implique un offset, initiallement contenu dans par `%al` (LSB de `%rax`), et il n'existe aucun gadget `pop %rax` qui auraît permis de controler sa valeur. Aussi, il nous impose de disposer le **byte** de `%rbx` à ce même offset, initiallement à la valeur **0x0B**, puis itérativement avec chacune des valeurs de `flag.txt`.

---
<br>

Il est important de retenir que deux types de registres seront sollicités ici:
- les registres directes, dont le contenu est chargé depuis la stack à travers des instructions `pop`; c'est le cas de:
    - `%rdx` et `%rcx`, le gadget `bextr` contient d'ailleurs les instructions `pop` ainsi que le `ret`.
    - `%rdi` pour le paramètre d'appel de la fonction `print_file`, et pour l'instruction `stos`.
- les registres indirectes, dont le contenu est chargé par des instructions:
    - `rbx` dont la valeur est modifiée par le gadget `bextr`, et utilisé par le gadget `stos`.

Un dernier point avant de passer à la construction de la *ROP chain*; si on revient au gadget pour l'instruction `bextr`, elle inclue tout autant les instructions `pop` pour les registres `%rcx` et `%rdx`, qu'une instruction `add`. Celle-ci vient ajouter un offset à la valeur contenue dans le registre `%rcx`, qui, pour rappel, détermine l'offset et la taille du bit field à extraire depuis `%rdx`. <br>

Il conviendra donc de calculer dès à présent les valeurs à charger:
- soit `%rdx` contenant le paramètre `flag.txt`.
- l'offset (bits 7-0 de `%rcx`) doit incrémenter à chaque fois.
- la taille (bits 15-8 de `%rcx`) doit être fixée à 1.
Soit l'ensemble des valeurs *0x010[1 to 7]*, qui devront impérativement être retrouvées après avoir appliqué l'instruction `add` avec la valuer **0x3ef2**. Après avoir élaboré un simple programme, nous retrouvons la valeur **0xc20f** pour l'offset **0x0101**.

Ainsi, il est facile de déduire la chaîne ROP à mettre en oeuvre:
1. Gadget `bextr` pour charger itérativement chacun des bytes de `flag.txt` dans `%rbx`.
2. Gadget `xlat` pour charger la valeur de `%rbx` dans `%al`
3. Gadget `pop` pour charger le registre `%rdi` depuis la stack.
4. Gadget `stos` pour charger le byte présent dans `%al` à l'addresse contenue dans le registre `%rdi`.

Il ne nous reste plus qu'à identifier la section sur laquelle charger le `flag.txt`:
> 24  0x00001038    0x0 0x00601038    0x8 -rw- NOBITS      .bss

## ROP Chain

Comme d'habitude, l'addresse de l'instruction `ret` pour la fonction `pwnme` est:
> 0x00007ffff7c00942

