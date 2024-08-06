# fluff challenge

## Introduction

L'énoncé est d'autant plus court qu'obscure... Il nous parle d'une instruction `mov` et de la fonction `questionableGadgets`, qui contient quelques instructions notables, propres à l'architecture *CISC* du x86. Celles-ci peuvent s'assimiler à un ensemble d'instructions *réduites* réunies en une seule et même instruction *complexe*. Ci-dessous les instructions contenues dans les gadgets:

---
<br>

>   0x0000000000400628 <+0>:     xlat   %ds:(%rbx)      <br>
>   0x0000000000400629 <+1>:     ret                    <br>

Cette première instruction `xlat` (**transLATe**) effectue un `mov` tel que:
- `%rbx` le registre **source**;
- `%al` (LSB de `%rax`) utilisé comme:
    - le registre de **control**; contient l'offset (en bits) à récupérer.
    - le registre de **destination**;

La valeur contenue dans le registre **source** doit être une addresse mémoire.

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

## Premier jet

Ma première idée fut la suivante:
1. `bextr` pour placer *l'addresse en mémoire d'un byte* de `flag.txt` dans `%rbx`.
2. `xlat` pour récupérer *ce byte depuis la mémoire* à placer dans `%rax`.
3. `stos` pour stocker ce byte en mémoire avec `%rdx`.

Et ce pour chacun des bytes de `flag.txt`; mais bien que le concept soit bon, je me suis vite retrouvé confronté à quelques problématiques que je vais tenter d'éclaircir ici.

---
<br>

*L'addresse en mémoire d'un byte* correspond à l'addresse en mémoire des caractères du paramètre à charger `flag.txt`. Pour ce faire, **ropper** propose la commande suivante (*ici pour le caractère 'f'*):
> ropper -f fluff --string f

Les addresses mémoires choisies pour les caractères seront énumérées dans la prochaine table. 

---
<br>

Le gadget `bextr` contient les instructions `pop`, ce qui convient parfaitement pour charger, depuis la stack, les valeurs de **control** et de **source**, dans leur registres respectifs, `%rcx` et `%rdx`. 

>   0x000000000040062a <+2>:     pop    %rdx            <br>
>   0x000000000040062b <+3>:     pop    %rcx            <br>
>   0x000000000040062c <+4>:     add    $0x3ef2,%rcx    <br>

La valeur de **control** du registre `%rdx` serait fixée à **0x4000**, afin de récupérer l'ensemble des bytes; néanmoins l'instruction `add` vient quelque peu perturber le *flow* de l'exploit, et oblige à préalablement calculer la valeur **source** (contenue dans `%rcx`) en lui soustrayant **0x3ef2**.

---
<br>

Le gadget `xlat` fonctionne avec un offset (en bytes), initiallement contenu par `%al` (LSB de `%rax`); il n'existe cependant aucun gadget `pop %rax` qui auraît permis de controler sa valeur. Aussi, il nous impose de disposer le **byte** de `%rbx` à ce même offset, initiallement à la valeur **0x0B**, puis itérativement avec chacune des valeurs de `flag.txt`...

---
<br>

Au final, la formule pour la valeur à charger dans le registre `%rdx`, celle pour *l'addresse en mémoire du byte* est la suivante, avec `i` l'indice de l'élément de `flag.txt`:
> %rbx[i] = mem_loc[i] - %al - 0x3ef2.

Avec le second terme `%al` qui aura initiallement la valeur **0x0B**, puis itérativement la valeur ASCII du caractère précédemment chargé. Ainsi, nous pouvons déduire les addresses suivantes:

Caractère   | Addresse       | Valeur à charger  |
----------- | -------------- | ----------------- |
f           | 0x004003f4     | 3fc4f7            |
l           | 0x00400405     | 3fc41f            |
a           | 0x004007a0     | 3fc52d            |
g           | 0x00400424     | 3fc88a            |
.           | 0x00400439     | 3fc4a7            |
t           | 0x004003d5     | 3fc4aa            |
x           | 0x004007bc     | 3fc7f5            |
t           | 0x004003d5     | 3fc427            |

## La solution

Il nous aura tout de même fallu faire des petits calculs, mais au final tout est bien qui fini bien;

Cette fois-ci la chaîne ROP inclue:
- L'overflow avec 40 fois le caractère `A`,
- Le chargement en mémoire de chacun des caractères de `flag.txt`:
    - L'instruction `bextr` pour charger l'addresse *brute*,
    - L'instruction `xlat` pour charger le caractère,
    - L'instruction `stos` pour charger ce caractère en mémoire.
- L'appel à la function `print_file`.


Comme d'habitude, l'addresse de l'instruction `ret` pour la fonction `pwnme` est:
> 0x00007ffff7c00942

