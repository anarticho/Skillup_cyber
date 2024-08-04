# 02/07/24
1. On travaille sur la machine Kali (./home/anarticho/BUFFER_OVERFLOW):
	1. [Overflow Exploit](https://www.youtube.com/watch?v=6sUd3AA7Q50)
	2. [Notes](./NOTES/buffer_overflow_workshits.md)
	3. C program (main.C) qui contient le buffer vulnérable.
	4. Pas encore terminé...
2. Nécessaire de s'informer sur le x86-64:
    1. [Gedare Bloom - x86-64 assembly](https://www.youtube.com/watch?v=lUbPUWtmVUU&t=491s)
	2. [Notes](./NOTES/x86-64_first_raw.md) -> 
3. Pas trop trop clair, on va voir des trucs basiques:
	1. [Reading assembly](https://wolchok.org/posts/how-to-read-assembly-language/) 
	2.[Cracking x86-64](https://medium.com/@sruthk/cracking-assembly-calling-conventions-in-x86-cc035d66f3f3)
	3. on a fait un petit programme C pour voir comment que ça marche les appels de fonctions etc..

# 03/07/24
1. On abbandonne l'idée du blog, on passe aussi au français.
2. On retourne sur la vidéo de Crow (https://www.youtube.com/watch?v=6sUd3AA7Q50)


# 04/07/24
1. Nouvelle série sur YT 		[flipthebit](https://www.youtube.com/watch?v=hH0Pss7PGVE)
2. Install Kali WSL				[official](https://www.kali.org/docs/wsl/wsl-preparations/#wsl-helper-script)
	- Pas utiliser le SUDO pleaase.
	> sudo tmux new -s gdb_sess
	> gdb
3. Jolie DKPG from video		[pwmdbg-tmux](https://medium.com/@Kuber19/enhancing-your-re-experience-with-pwndbg-tmux-459d271105f2)
4. Visual studio on lite WSL	[VS remote WSL](https://code.visualstudio.com/docs/remote/wsl)
5. Configure git on WSL			[Credential manager](https://dev.to/stephanlamoureux/configuring-git-in-wsl-5e3m)

# 05/07/24
1. On a enfin stabilisé la machine linux!
2. Au final on a perdu trop de temps sur l'interface (avec Splitmind).
4. Tools qu'on utilisera:
	- GCC (mention spéciale pour le -g flag "gcc -g -o exe_file c_file.c)
	- PWNDBG comme pluggin GDB (https://github.com/pwndbg/pwndbg)

	Et pour l'interface un mix entre:
	- GDB-dashboard (https://github.com/cyrus-and/gdb-dashboard)
	- TMUX pour le Terminal Multiplexer (https://github.com/tmux/tmux).

On pourrait substituer l'usage de TMUX avec la feature de GDB-dashboard qui permet de rediriger les éléments dans des terminals distincts (https://gitee.com/dongbo_89/gdb-dashboard#display-each-module-in-a-separate-terminal).

Même au niveau de TMUX, ça reste assez complet, donc complexe. Du coup ce template me parrait plus que correct pour commencer:
- https://github.com/ficoos/tgdbdb/tree/master.

# 06/07/24
1. Configuration at https://github.com/anarticho/Kali_machine/commit/9ff85f718ea0d59f74d739fe6c4dac36cb4439fd.


# 07/07/24
1. https://valsamaras.medium.com/introduction-to-x64-linux-binary-exploitation-part-1-14ad4a27aeef
2. On revoit l'organisation du follow up.
3. Backup pour la machine Kali (*C:\...\Desktop\Backup Kali*).
4. Repository github pour la machine entière (très mauvaise idée) supprimée.
5. Aujourd'hui:
	1. Me suis rendu compte que **python3** avait son output en Unicode Text String output, alors que **python2** l'a en Byte String.
    (https://discuss.python.org/t/unusal-behavior-of-python3-print-hex-values/15418/8)
	2. Je vais donc créer un programme en C++ qui permette de générer un output en byte-text string, un peu comme le `python -c 'print(..)'"` avec gdb run $(): 
    - VS_work/binject:
        - C++ with main entry point + Translater class.
        - Make file.

# 10/07/24
1. **tgdb** from *~/tools* to newly created *~/bin*.
2. Configuration Visual Studio Code Debug:
	- En suivant les instructions (https://hackernoon.com/how-to-set-up-c-debugging-in-vscode-using-a-makefile)
	- CTRL + P
		- Tasks: Run tasks
		- Debug: Start Debug
3. Nouveau **binject** ---> https://github.com/anarticho/binject
4. Executable ajouté à *~/bin*
5. Peut être utilisé avec **gdb**:
> r $(binject -s 123)

6. Réorganisation du dossier *C:.../SKILLUP_CYBER/Follow*.
7. Revenons sur le programme C de **crow**:
	- hackme function appelé avec le buffer overflow.
8. ret2win ROP challenge cracked !
9. Test avec un autre programme:
	- VS_work/buffer_overflow directory.
	- https://www.youtube.com/watch?v=hH0Pss7PGVE code.
	- Malgré que sur la session GDB la fonction soit bien appelée, SEGFAULT est lancé à l'appel de printf...
10. Avec ce code https://dhavalkapil.com/blogs/Buffer-Overflow-Exploit/:
	- Cracké (*VS_work/buffer_overflow/main2.c* + SOLUTIONS.md)
	- Par contre, ça marche toujours qu'en debug avec GDB.
11. Quelques hypotèses:
	- PIE ou ASLR?
	- la function printf est corrompue par l'overflow?

# 11/07/24
1. Apparemment, autant main que main2 (dans *VS_work/buffer_overflow/*) ont le même problème. Voyons voir les sécurités avec:
> checksec ret2win 
2. Nous informe que cet exécutable n'a pas PIE enable, alors que main et main2 l'ont..
3. A propos du **mechanism PIE**, j'ai trouvé [ce site](https://ir0nstone.gitbook.io/notes/types/stack/pie) et [celui-ci aussi](https://ir0nstone.gitbook.io/notes/other/pwntools/introduction)
4. On dégage complètement python2 vu qu'on l'utilise plus du tout (binject rpz).
5. Sauvagarde Kali at *C:\Users\...\Desktop\Backup Kali\11_07.tar*

# ATTENTION
Les points suivants font référence à des documents, c'est pas pour des prunes. Je réponds principalement aux problèmes soulevés précédemment.

# 12/07/24
- NOTES_07_01_split.md

# 13/07/24
-  ROP's callme challenge

# 14/07/24
- NOTES_07_02_callme.md
- Modifications à binject

# 15/07/24
- write4 challenge
- NOTES_07_03_write.md
- Pour la solution utilisant binject avec **.sh script**, il faut habiliter l'execution de ce dernier en utilisant:
> chmod +x le_script
- write4 solution.

# 16/07/24
- Review de toutes les notes.
- Traduction à français.
- Clean NOTES.
- https://www.youtube.com/watch?v=hhu7vhmuISY

# 18/07/24
- Vieil écran bleu, la WSL s'est corrompue.. Heuresement il y a les backup :3
- Le backup permet juste de récupérer les fichiers de home/..., parce que le `wsl --import` fonctionne pas trop trop.
- Du coup:
1. Dans *Programmes et fonctionnalités* > *Activer ou désactiver des fonctionnalités de Windows* > Décocher *Sous-système Windows pour Linux* > Redémmarrer.
2. Réinstaller WSL (recocher).
3. Installer kali linux avec `wsl --install --distribution kali-linux`
4. Enlever le message de base avec le `touch`
5. Installer:
	- `sudo apt update` pour actualiser les DNS.
	- `sudo apt install build-essential` pour installer gcc, g++ et make.
	- `sudo apt install gdb` pour installer gdb (le debugger).
	- **.bashrc** et **.tgdbinit** depuis le backup *.tar*. 
	- https://github.com/ficoos/tgdbdb
	- https://github.com/anarticho

# 19/07/24
- On commence le challenge ROP numero 5: badchar.

# 20/07/24
- modif NOTES_04_badchar.md
- notes en français
- TODO from ROP's notes

# 21/07/24
- 'bd' flag on binject
- quasiment fini le **badchars** challenge, mais problème avec une addresse qui contient un badchar, voir les derniers paragraphes de **## La solution**

# 22/07/24
Modification binject:
- [x] `-x64` -> `-a` pour addresse

# 26/07/24
Modification binject:
- [cb] to check for bad characters.
- [bd] to produce files to escape bad characters.

ROP badchar finished -> **NOTES_04_badchar.md**