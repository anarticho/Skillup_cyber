
# split chalenge

En revenant aux challenges de ROP emporium, nous en somme au **split**: https://ropemporium.com/challenge/split.html

En regardant le guide pour débutant sur leur site web, on repère plusieurs *tools*:
- https://github.com/sashs/Ropper
 	- Pour trouver des gadgets ROP dans l'exécutable tel que:
	> ropper --file split --search "ret" **//** pour trouver des gadgets avec l'instruction ret
- https://github.com/Gallopsled/pwntools
	- Je suis pas sûr d'utiliser celui-ci, peut-être au moment de devoir gérer l'ASLR (**A**ddress **S**pace **L**ayout **R**andomization)
- https://github.com/radare/radare2
	- avec son utilitaire **rabin2**, pour voir les sections, les strings et d'autre trucs dans l'executable:
	> rabin2 -z split **//** pour afficher tous les strings avec leur addresse en mémoire.
	> rabin2 -S split **//** pour afficher toutes les sections, leur permission, leur addresse etc.

Tous les outils précédemment cités permettent une analyse **statique** de l'exécutable, tandis que GDB permet une analyse dynamique de l'exécutable.

## Le mur

Celui-ci était vraiment compliqué à franchir, mais nous l'avons fait !

Au début, nous avions tenté de simplement retourner à la fonction `usefulFunction` contenant l'appel à la fonction `system` en utilisant le paramètre `/bin/ls`:
> binject -ns 40 A -x 4207400000000000

Bien que cela puisse paraître plutôt basique, et que tous les tutoriels ainsi que les vidéos obtiennent le résultat console de la commande `ls`, moi j'obtiens un vieux **SEGFAULT** dans le corps de la fonction `system`, précisemment à l'instruction suivante:
> movaps %xmm0, 0x50(%rsp)

En cherchant un peu à ce sujet, il apparaît que le problème vient d'un mauvais alignement de la stack (**stack misalignment**), plusieurs sources intéressantes à ce sujet:
- [Stack Alignment](https://book.hacktricks.xyz/binary-exploitation/rop-return-oriented-programing#stack-alignment)
- [la réponse de Nate Eldredge](https://stackoverflow.com/a/74755766)

Cela peut être quelque peu *tricky* à comprendre; l'instruction `movaps` depuis l'appel `system` a besoin d'avoir la stack aligné sur 16 bits (en hexadécimal, le dernier *digit* à 0). 

Au final, aligner la stack est une opération simple, puisqu'il existe l'instruction `ret`, qui a pour effet de `pop` la *return address* de la stack et la charger dans le registre `%rip`; implicitemnt `pop`er un élement de la stack réduit sa taille de 8 bytes (taille d'une addresse pour l'architecture **x64**).

## La clef

En utilisant **ropper**, nous pouvons facilement trouver l'addresse d'une instruction `ret` dans l'executable,
> ropper --file split --search "ret" <br>
> 0x000000000040053e: ret;

L'idée est ensuite d'utiliser ce ROP gadget directement après avoir injecté les bytes pour l'overflow, ainsi la stack serait aligné pour le reste de la chaîne ROP
> binject -ns 40 A -x 3E05400000000000 -x 4207400000000000

Ainsi, nous obtenons enfin le résultat de la commande `ls` grâce à l'overflow. Comme **proof of concept**, voici les addresses des registres `%rsp` après l'injection de l'overflow, pour le programme sans alignement et celui qui a l'instruction `ret`, donc aligné:

Après l'instruction | ROP +RET	| Just RET
------------------- | --------- | -------
CALL <read>			| 0xB0		| 0xB0
LEAVE				| 0xD8		| 0xD8
RET					| 0xE0		| -
RET (extra ROP) 	| 0xE8		| 0xE0 
PUSH %RBP			| 0xE0		| 0xD8
AT CALL <system>	| 0xE0		| 0xD8

## Le challenge

L'étape suivante est la création d'une chaîne ROP; avant cela, quelques explications s'imposent quant à la convetion d'appel pour l'architecture **x86-64** (aka **x64**):
- https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/linux-x64-calling-convention-stack-frame

Dans d'autres architectures, les arguments passés aux fonctions sont poussés dans la stack. Avec le **x64**, les instructions sont chargées dans des registres concrets, et ne seront poussés dans la stack que sous de très strictes conditions. C'est d'autant plus visuel lorsqu'on désassemble le code d'une fonction qui contient une instruction `call`:
> mov    $0x40084a,%edi
> call   0x400560 <system@plt>

Ici, l'addresse *0x40084a* contient le paramètre pour l'appel de la fonction `system`, **et doit par convention** être chargé dans le registre `%rdi`.

Pour ce challenge, nous voulons appeler la même fonction `system` avec l'argument de notre choix. Cela signifie que nous devons nous assurer d'avoir charger ce nouvel argument dans le registre `%rdi` avant d'appeler la fonction; cela se fait facilement grâche à ce ROP gadget:
> pop %rdi; ret;

La chaîne s'avère particulièrement simple:
> binject -ns 40 A -x C307400000000000 -x 6010600000000000 -x 4B07400000000000

Avec:
- C307400000000000 the address for 'pop %rdi; ret' gadget,
- 6010600000000000 the address for '/bin/cat flag.txt' string,
- 4B07400000000000 the address for 'call <system>' instruction.
