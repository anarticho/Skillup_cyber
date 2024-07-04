# Pour les noobs - x86-64 Assembly

## Concepts basiques

Comme une première introduction à l'assembleur, je vais présenter une des architectures les plus répandues dans le CPU-game, la x86-64 (la version 64-bits de l'architecture RISC x86). Mon but ici ne sera pas de présenter les bases théorigques du fonctionnement d'un processeur. Par contre il me paraît intéressant d'aiguiller les novices vers certains notions importantes.

## Pour les très noobs
Il y a un début à tout; et parce que je suis vraiment quelque'un de particulièrement gentil, je vous met cette jolie table en guise de roadline.
Step    | Topic                         | Pour passer à l'étape suivante..
------- | ----------------------------- | -------------------------------------------------------
0       | Electronique                  | [C'est quoi un transistor?](./transistor.jpg)
1       | Système binaire               | [Et 3 c'est quoi en binaire?](./bin.jpg)
2       | Représentation hexadécimale   | [0xDEADBEEF c'est quoi?](./calc_hex.PNG)
3       | Opération binaire             | [Une porte logique c'est quoi?](./xor_gate.jpg)
4       | Pointer                       | [😮👇](./pointer.PNG)
5       | Mémoire et types natifs       | Nombre de bytes pour un integer sur un windows 64-bits? [BONUS 🙅](./easy.png)
6       | Algorithmique                 | C'est quoi une FSM?

Puis au final, si [ce meme]() semble à peu près compréhensible, je pense qu'il est temps de s'atttaquer à des notions déjà plus spécifiques. 

>Friendly reminder, google est mort, [__longue vie à ChatGPT__](gpt.png):


## Notions importantes

### __MPU, MCU et CPU__
Alors, la base c'est tout de même le CPU.

Le MPU, c'est en général ce que t'as sur ton ordinateur, un CPU quadri-mega-super-coeur 32535 threads 0.12132 nanometres etc, avec des périphériques pour qu'il fonctionne au-dessus, la RAM, le disk et tout le bordel, et tout ça pour que jean-jesaispascommentclaquermathune puisse streamer ces jeux-vidéos préférés, [quel geek](./linus.png)).

Puis le MCU, ça c'est plutôt spécialisé, et c'est ce qui se retrouvera le plus dans les systèmes embarqués. Ca contient déjà tout les périphériques utiles, et c'est généralement pas des gros gros CPUS ki²toudur, mais [ça fait le café](https://www.righto.com/2019/09/a-computer-built-from-nor-gates-inside.html).

<image src="mcu_mpu.png" width=500>

Pour faire plus facile dans l'histoire, nous allons nous concentrer sur le CPU pour cet article, et je l'appelerai;
>The machine 🤖.

### Machine et language
Le CPU a une langue, le __langage machine__. Et dépendemment de son architecture (i.e. son constructeur) il parle une langue ou une autre. Bon oui il y a bien des conventions, mais ça reste franchement différent d'une architecutre à l'autre.

### Programmation et compilation

Les compilateurs, ou compilers __idc__, traduisent un truc vaguement compréhensible par le commun des mortels (__langage de programmation__), en un truc parfaitement compréhensible par 
> The machine 🤖. 

Simple. Basique.

<image src="ytvideo.png" width=500>

### Les différents types de mémoire
Plutôt que de vous prendre la tête, je me la suis mangé moi-même pour vous faire un résumé:
- __Premièrement__ les types de mémoires physiques, palpables, matérielles:
    - __persistent__ pour la mémoire qui persiste même sans ⚡ (Flash, SD, Hard Disk).
    - __permanent__ pour la mémoire qui ne peut être altérée (ROM, fuses, OTP).
    - __volatile__ pour la mémoire qui se perd sans ⚡ (RAM, SRAM).
- __Secondement__, d'un point de vue conceptuel, au niveau du système:
    - __disk__ pour la mémoire lente d'accès; utilisé pour booter (Flash, ROM), contient le programme ou pour avoir un file system (SD, SSD, NAND Flash) persistent.
    - __memory__ pour la mémoire rapide d'accès, c'est la __RAM__, là où _vis le programme_ et ça doit être le truc le plus connu je pense, donc je passe.
- __Dernièrement__, d'un point de vue conceptuel toujours, mais au niveau du programme cette fois-ci:
    - __stack__, ou __pile__ fonctionne en [LIFO](https://fr.wikipedia.org/wiki/Last_in,_first_out), pour gérer les appels de fonction, les variables locales. De taille limitée (car fixée), mais rapide d'accès.
    - __heap__, ou __tas__ utilisé pour l'allocation dynamique... C'est pas assez explicite? C'est lent d'accès, et sa création/destruction est gérée par le programme lui-même. C'est à l'origine d'immondices comme le [garbage collector, là ou des languages qui se respectent laisse l'utilisateur se responsabiliser de son mauvais design](./garbage.png).
    - __data segment__, utilsé pour les variables statiques dans le programme.
    - __text code__, plutôt explicite comme nom pas vrai. C'est pour les instructions du code.

__Révélation qui va vous changer la vie, tout ce dernier petit monde, la pile, le tas, le code etc..c'est de la RAM__.

__Cela n'existe que pour le programme, et c'est des sections qu'un programme utilise de manière différente__.

Allez salut 👋.

## L'architecture x86-64

Alors, sans mentir, j'ai galérer à la rédaction de cette partie, et ça partait d'une bonne intention:
> Fournir un truc potable.

Comme je l'ai dis, du contenu y en a. Et c'est pas un petit blog en français qui va se démarquer 😅

Par contre du contenu potable, c'est compliqué. L'assembleur c'est complexe, et le vulgariser n'est pas une tâche aisée. En résumé:
> Les explications disponibles sont franchement pas dégueu, mais les figures/diagrammes..
![alt text](cdlamerde.png)

Promis je vais pas vous perdre, mais avant de rentrer dans le lard, quelques précisions sur les outils utilisés ici:
- Langage C,
- Compilateur GDB + add-on PEDA,
- Beaucoup de verveine.

C'est parti.

### Introduction made in GPT

>L'architecture x86-64, introduite par AMD en 2003 et adoptée par Intel, étend l'architecture x86 à 64 bits, permettant un adressage de mémoire plus vaste et des performances améliorées. Elle est rétrocompatible avec les instructions 32 bits et 16 bits. Les caractéristiques clés incluent l'ajout de 8 registres généraux supplémentaires, le doublement de la taille des registres (de 32 à 64 bits), et une capacité d'adressage mémoire théorique jusqu'à 16 exaoctets. Cette architecture est largement utilisée dans les ordinateurs personnels, les serveurs et les stations de travail modernes.

Merci ChatGPT pour cette magnifique introduction, maintenant que le travail de péon est fait, passons à des trucs plus intéressants.

### Un peu de registres

Si tu sais pas ce qu'est un registre, tu n'est sûrement pas prêt pour ce qui suit, et je t'invite à te référer aux deux premières parties (celles pour les noobs 😘).

<details> 
    <summary> Si tu doutes de ton niveau... </summary>
    Combien de bits a un registre du x64?
</details>
<br>

Des registres pour le x64, il en existe plusieurs types:
- Général:
    - Traditionnels (_A,B,C,D_),
    - Utilitaires (_SI,DI,BP,SP_),
    - Additionnels (_8,9,10,11,12,13,14,15_).
- Pointeur d'instruction (_IP_),
- Etat/CPU status (_FLAGS_),
- Segment (_CS,DS,SS,ES,FS,GS_),
- Contrôle (_CR0,CR1,CR2,CR3,CR8_),
- Test/Debug (_DR0,DR1,DR3,DR4,DR5,DR6,DR7_)
- Memoire (_GDTR,IDTR,LDTR,TR_)

Quelques commentaires intéressants:
- Il existe [deux syntaxes différentes](https://imada.sdu.dk/u/kslarsen/dm546/Material/IntelnATT.htm) s'aggisant de la x64; j'utiliserai celle d'__Intel__.
- Les registres traditionnels proposent une rétro-compatibilité avec les jeux d'instructions de CPU d'Intel plus vieux (les coquins) à savoir:
    - [AI32](https://es.wikipedia.org/wiki/IA-32), en 32 bits.
    - [8086](https://en.wikipedia.org/wiki/Intel_8086), en 16-bits.
    - [8080](https://en.wikipedia.org/wiki/Intel_8080) en 8 bits.

### __Instruction Cycle__


Ou _Fetch-exectute cycle_, je ferai pas mieux que [wikipedia](https://en.wikipedia.org/wiki/Instruction_cycle) sur ce coup-là. Par contre c'est assez dense comme notion... Prenez bien votre temps sur celui-ci. [Cette vidéo](https://www.youtube.com/watch?v=-IyB8hBkA9w) fournit une superbe analogie pour avoir une compréhension superficielle, mais suffisante des points importants de ce concept crucial dans la compréhension d'un processeur.

### Un peu d'instructions

Afin

Dans le cas présent, nous n'allons nous intéresser qu'aux registres utilitaires BP, SP et IP, respectivement:
- Base Pointer,
- Stack Pointer,
- Instruction Pointer.

### La pile d'appel



- __stack__, ou __pile__ fonctionne en [LIFO](https://fr.wikipedia.org/wiki/Last_in,_first_out), pour gérer les appels de fonction, les variables locales. De taille limitée (car fixée), mais rapide d'accès.

Bref, les appels de fonction, ses paramètres, les variables locales, tout se passe ici. 

### Le programme

Pour
Le point d'entrée d'un programme



### Relevant registers and usage

#### RIP, Instruction Pointer
Ce register contient l'addresse en mémoire de l'instruction actuellement exécutée.

#### RSP, Stack Pointer
Ce register contient l'adresse en mémoire du top de la stack.

#### RBP, Base Pointer
Ce register contient l'addresse du frame pointer (si quelqu'un a une meilleure traduction qu'il se prononce ou se taise à jamais). Et le frame pointer c'est 

### Function call

Le sujet le plus intéressant selon moi, parce que cela devient vraiment technique. Jusqu'à maintenant j'ai évoqué énormément de concepts différents, puis introduit les bases de l'architecture x86-64 d'Intel. Quelque chose de plus concret, pratique cette fois-ci, un appel de fonction.



Avant toute chose, il convient de rappeler quelques points importants:
- La __Stack__ est descendante, et fonctionne en LIFO (Last-In, First-Out).
- 

<image src="./stack.jpg" height=400>

Les instructions basiques pour pousser une valeur dans la stack et la retirer sont `pushq` et `popq`. 

Ces instructions incrémentent, ou décrémentent le registre _stack pointer_ par 8 (bytes): <br>
⚠️ OJOO, ce sont 8 bytes pour les instructions avec un `q` pour __quadword__ qui signifie 4 words de 16-bits (=64 bits). <br>
⚠️⚠️ D'autres instructions pourront incrémenter ou décrémenter la stack selon la taille de l'instruction (et c'est pas plus mal). <br>
⚠️⚠️⚠️ `pushq` décrémente l'addresse de `%rsp` de 8 bytes. `popq` l'incrémente.

L'appel de l'instruction (ça fait très nouvel opus de LOTR), se fait de la manière suivante:
1. Calcul de la __Return address__ comme l'instruction dans le programme après la function.
2. __Push__ cette address dans la __Stack__.
3. __Jump__ to function using its __symbol__.

Ensuite, au __return__:
1. __Pop__ le top de la __stack__.
2. __Jump__ to __Return address__.

Alors dis comme ça, c'est pas forcément très explicite. J'ai vainement essayé de faire un graphique animé, mais ça m'aurai supposé trop de temps pour un résultat pas forcément très ouf. Du coup, je me suis retourné sur [PlantUML](https://plantuml.com/fr/) parce qu'y étais déjà pas mal habitué avec mon ancien taff.

