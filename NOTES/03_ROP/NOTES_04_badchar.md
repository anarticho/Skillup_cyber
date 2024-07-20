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

Le paramètre *"flag.txt"* que nous devons passer à la fonction *print_file* doit impérativement être injecté à travers de ce mécanisme, tout comme l'ensemble des éléménts écrits dans la stack à travers de l'*overflow*. A ce titre, l'utilitaire **ropper** fournit un flag permettant d'exclure un ensemble de caractères dans la recherche de gadget:
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

[COMMIT]()

## Les gadgets

Si nous utilisons `ropper --file badchars --search "xxx"` pour identifier les ROP gadgets, il s'avère que le `mov qword ptr [r14], r15` n'est plus présent dans cet exécutable, à la place:
> 0x0000000000400634: mov qword ptr [r13], r12; ret;

Si on se réfère au gadget pour le XOR présent dans le programme; il permet d'effectuer l'opération OU-exclusif entre le byte de poids faible contenu dans le registre `%r14` et le byte pointé par l'addresse contenue par le registre `%r15`:
> 0x0000000000400628: xor byte ptr [r15], r14b; ret;

Il existe un gadget `pop` qui permet de récuperer tous ces registres depuis la stack:
> 0x000000000040069c: pop r12; pop r13; pop r14; pop r15; ret;

Ensuite, exactement le même mécanisme d'appel de fonction que celui pour le challenge *write4*, c'est à dire le ROP gadget `pop` sur `%rdi` avec l'addresse en mémoire du paramètre d'appel de `print_file`; `flag.txt`.
> 0x00000000004006a3: pop rdi; ret;

Afin de construire la chaîne ROP, il nous va falloir un utilitaire capable de repérer les **badchars** 

La chaîne ROP seraît donc la suivante:
1. Overflow du buffer vulnérable:
> binject -of ovf.bin -ns 40 A
2. Copie dans les registres `%r12`, `%r13`, `%r14` et `%r15` des valeurs dans la stack:
> binject -of pop.bin -x64 40069c -s flag.txt -x64 601028 -x64 FFFFFFFF -x64 601028
3. Copie de la valeur contenue dans `%r12` à l'addresse contenue dans `%r13`: 
> binject -of mem.bin -x64 400634
4. Application 
> binject -of fnc.bin -x64 4006a3 -x64 601028 -x64 400510 <br>

Parce que la fonction **pwnme@plt** est particulièrement chiante, voici la commande **gdb** pour insérer un breakpoint sur son instruction `ret`:
> b *(0x7ffff7c00a06)

Bien que cette chaîne ROP ne va pas fonctionner, nous disposons déjà d'un ensemble de fichiers binaires que nous pourrions tester pour équiper notre programmme **binject** afin qu'il puisse faire face à cette nouvelle fonctionnalité avec:
- un flag pour checker un fichier binaire.
- un flag pour générer une chaîne ROP qui échappe les **badchars** d'une chaîne de caractère.
