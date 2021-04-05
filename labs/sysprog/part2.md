# Partie 2: Compilation, debug et gestionnaire de signaux


## Exercice 1 : GDB et fichier core

Une fois le simulateur GPS compilé, le lancer grâce au script *gps/run.sh* :

````
$ sh run.sh
PTTY: /dev/pts/X
````

**Question 1** : Que se passe-t-il au bout de quelques secondes? Qu'en
                 déduisez vous?

Une segmentation fault se produit, l'édition de liens pour les librairies partagées à utiliser ne semble pas fonctionner.
En allant directement dans /bin pour exécuter le binaire ```./gps```, on obtient l'erreur suivante : "./gps: error while loading shared libraries: libptmx.so: cannot open shared object file: No such file or directory".

**Question 2** : Quel signal a reçu le processus pour se terminer ainsi? Comment
                vérifiez vous le numéro du signal reçu?

Le processus a reçu le signal 11) SIGSEGV associé à une segmentation fault pour se termoner ainsi. On peut le vérifier dans un fichier core généré et être rejoué par le débugger GDB pour comprendre l'erreur.

Lors d'une terminaison anormale, un fichier *core* peut être généré. Par défaut,
la génération d'un fichier core est généralement désactivée :

````
$ ulimit -c
0
````

Ici la commande renvoie grâce au paramètre *-c* la taille du fichier core à
générer. La taille étant 0, aucun fichier n'est créé. Pour y remédier :

````
$ ulimit -c unlimited
$ ulimit -c
unlimited
````

Relancez le simulateur GPS. Suite au crash, un fichier core doit être généré
dans le répertoire courant.

Nous allons ici utiliser GDB pour analyser le dump mémoire afin de trouver
l'origine de l'erreur. GDB est un outils très complet fournissant de
nombreuses commandes.

Pour lancer GDB et analyser un fichier core :

````
$ gdb ./bin/gps core
````

Ensuite, dans le prompt GDB, utilisez la commande *bt* (pour *backtrace*) afin
de savoir comment votre programme en est arrivé là (image de la pile).

**Question 3** : Grâce à GDB et au fichier *core* généré, analysez la source du
                 problème du binaire *gps*. Quelle partie du code est fausse?
                 Pourquoi?

Le problème vient du fichier source nmea.c ligne 23 avec l'appel de la fonction *puts* ``` puts(NULL);```. En exécutant ```man 3 puts``` on comprend que la fonction attend une chaîne de caractères et non un pointeur *NULL*.

GDB peut être aussi lancé de manière interactive :

````
$ gdb ./bin/gps
````

Une fois dans le prompt, il faut lancer la commande *r* (comme *run*) et *bt*
pour récupérer la backtrace.

Il est aussi possible de placer des breakpoint sur une ligne de code, dans une
fonction, etc. Puis on peut avancer pas à pas avec *n* ou *s* (en fonction de
ce que l'on veut faire). Par exemple:

````
$ gdb ./bin/gps
$ break nmea.c:19
$ r
...
$ s
...
$ n
...
````

**Question 4** : Que se passe-t-il quand vous lancez GDB en mode interactif sur
                 le binaire *gps*?

Une erreur survient en lançant la commande *r* pour rejouer le processus. On a l'erreur suivante : /bin/gps: error while loading shared libraries: libptmx.so: cannot open shared object file: No such file or directory.


Suite au problème repéré, allez dans le répertoire *gps/bin* et lancez la
commande suivante :

````
ldd ./gps
````

**Question 5** : À quoi sert la commande *ldd*? Quelle information
                supplémentaire cela vous apporte-t-il?

La commande ldd donne l'ensemble des librairies partagées nécessaires pour exécuter le programme *gps*. Pour chaque dépendance, ldd affiche le chemin de la librairie partagée et l'adresse hexadécimale à laquelle l'objet est chargé..
```
  linux-vdso.so.1 (0x00007ffc0f15e000)
	libptmx.so => not found
	libnmea.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa7030fc000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fa7036f0000)
```


**Question 6** : Comment résoudre ce problème en tant qu'utilisateur? N'hésitez
                 pas à regarder le fichier *gps/run.sh*.

On exécute la commande suivante dans le terminal :
```export LD_LIBRARY_PATH=$(pwd)/lib```

Puis on relance la commande ``` ldd .bin/gps``` et on obtient :
```
ldd: ./home/estellearrc/Documents/Cours/ENSTA_Bretagne_2020_2022/S4/GNU_Linux_embarque/myproject/embsys/labs/sysprog/gps/bin/gps: No such file or directory
        linux-vdso.so.1 (0x00007ffd36df6000)
        libptmx.so => /home/estellearrc/Documents/Cours/ENSTA_Bretagne_2020_2022/S4/GNU_Linux_embarque/myproject/embsys/labs/sysprog/gps/lib/libptmx.so (0x00007f89755fa000)
        libnmea.so => /home/estellearrc/Documents/Cours/ENSTA_Bretagne_2020_2022/S4/GNU_Linux_embarque/myproject/embsys/labs/sysprog/gps/lib/libnmea.so (0x00007f89753f7000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f8975006000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f8974c68000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f89759ff000)
```
L'édition de liens pour les librairies partagées est valide.

Relancez *ldd* puis GDB pour vérifier que votre solution a porté ses fruits.

**Question 7** : Quelle est la différence entre les commandes *s* et *n* dans
                 le prompt gdb suite à un breakpoint?

*s* exécute chaque ligne du code source, y compris celles des sous-fonctions qu'il appelle. On saute donc de fonction en fonction, de fichier en fichier.
"Step program until it reaches a different source line."

*r* exécute chaque ligne du code source, sans entrer dans les sous-fonctions appelées.
"Unlike "step", if the current source line calls a subroutine,
this command does not enter the subroutine, but instead steps over
the call, in effect treating it as a single source line."

Il existe aussi une version de GDB pour déboguer à distance. Il y
a alors un GDBServer tournant sur la cible où le programme à déboguer est
exécuté. Ensuite, un client GDB tourne sur la machine servant à déboguer
et communique avec le serveur grâce au réseau.

**Question 8** : Dans quel contexte ce type d'outils peut être intéressant?

Ce type d'outils est très intéressant lors d'une erreur de type *segmentation fault* puisqu'aucune information sur la provenance de l'erreur n'est donnée. GDB nous permet de remonter jusqu'à la source de l'erreur sans perdre trop de temps.

### À retenir

  * l'utilité de *ulimit* et comment déclencher la génération d'un fichier core
  * à quoi sert *GDB* et comment l'utiliser
  * l'utilité de *ldd*
  * pourquoi, quand et comment utiliser la variable d'environnement
    *LD_LIBRARY_PATH*

## Exercice 2 : LD_PRELOAD et sigaction

Maintenant que le problème est identifié, nous allons le résoudre. Cependant,
nous partons du principe que le code source du simulateur **NE DOIT PAS ÊTRE
MODIFIÉ**. Pour corriger le problème, nous allons utiliser la variable
d'environnement *LD_PRELOAD*. Cette variable permet de *hooker* (comprendre
*usurper*) certaines fonctions d'une application.

Utilisation :

````
LD_PRELOAD=libhook.so ./bin/gps
````

En faisant ainsi, le binaire cherchera en priorité les fonctions dont il
a besoin dans *libhook.so*! Pour que cela fonctionne, il faut que les fonctions
définies dans libhook aient exactement le même prototype.

Pour les questions suivantes, allez dans le répertoire de travail
*ld_preload*. Vous devrez travailler sur trois fichiers:

  * hook.c
  * Makefile
  * run.sh

**Question 1** : Implémentez dans le fichier hook.c la fonction à l'origine du
                 problème repéré au sein du simulateur GPS mais cette fois-çi
                 sans erreur.

**Question 2** : Éditez le Makefile pour compiler *hook.c* sous la forme d'une
                 librairie partagée nommée *libhook.so* (s'inspirer de
                 *gps/src/lib/ptmx/Makefile*). Testez la compilation.

**Question 3** : Éditez le fichier *run.sh* pour utiliser LD_PRELOAD au moment
                 de lancer le simulateur et ainsi hooker le binaire avec la
                 librairie libhook.so. Exécutez run.sh : le simulateur ne doit
                 plus partir en segfault.

Nous avons ici hooké une fonction définie dans une librairie "utilisateur". On
peut réaliser la même opération sur les librairies systèmes. Par exemple, le
simulateur GPS utilise la fonction *printf* dès son lancement.

**Question 4** : Utilisez le *man* pour déterminer le prototype de la fonction
                 *printf* (expliquez comment vous utilisez *man* dans ce cas et
                 pourquoi). Comment est appelé ce type de fonction?

On utilise la commande ```man 3 printf``` pour avoir la documentation C de la foonction **printf** et non celle du bash. Cette fonction fait partie des converteurs de sortie formattée, c'est-à-dire qu'elle écrit une sortie selon un format dans un flux de données.

**Question 5** : Analysez *gps/src/bin/gps/gps.c* er repérez où se trouve le
                 gestionnaires de signaux. Décrivez les fonctions utilisez
                 ainsi que les signaux gérés.

Le gestionnaire de signaux est la fonction **signals_handler** qui prend en argument le numéro du signal attrapé. La fonction **sigaction** change l'action à réaliser par un processus à la réception d'un signal spécifique. Elle associe donc à un signal donné (ici SIGINT soit le numéro 2) l'action à réaliser à la place de celle qui était réalisée par défaut par le processus.

L'argument **signum** spécifie le signal concerné (tous sont valides à l'exception de *SIGKILL* et *SIGSTOP*).

L'argument **act** renseigne la nouvelle action à mettre en place pour le signal **signum** s'il est non NULL. Si l'argument **oldact** est non NULL, l'action précédemment réalisée est enregistrée dans cette variable.

Pour créer une action, il faut renseigner au moins 3 attributs :
* **sa_handler** l'action à associer au signal **signum** attrapé par le processus. Elle peut être *SIG_DFL* pour l'action par défaut, *SIG_IGN* pour ignorer le signal, ou un pointeur vers la fonction custom gérant le signal (comme c'est le cas ici).
* **sa_mask** précise le masque de signaux qui doivent être bloqués lors de l'exécution du gestionnaire du signal attrapé **signum**.
* **sa_flags** précise l'ensemble des variables drapeaux qui modifient le comportement du signal.

**Question 6** : Hookez le simulateur pour que ce dernier ne puisse plus
                 être interrompu par le signal SIGINT (Ctrl-C) en
                 réimplémentant la fonction *printf* dans libhook.so. Pour
                 cela, utilisez la fonction *sigaction* pour mettre en place
                 un gestionnaire de signaux au sein même de la fonction
                 *printf*  réimplémentée.

**Question 7** : Comment faire pour interrompre le processus étant donné
                 que ce dernier ne répond plus au Ctrl-C? Citez deux méthodes.

On peut utiliser le signal *SIGQUIT* avec ```Ctrl + Alt Gr + \```. On peut aussi utiliser le signal *SIGSTP* avec ```Ctrl+Z``` qui va mettre en pause le processus en premier plan.

````
GPS OK
^Z   
[1]+  Stopped                 ./run.sh
````

On tape la commande ```ps``` dans un terminal pour avoir la liste des processus en cours d'exécution et donc récupérer le PID du script bash "run.sh".

````
PID TTY          TIME CMD
 9487 pts/1    00:00:00 bash
 9651 pts/1    00:00:00 run.sh
 9657 pts/1    00:00:00 gps
 9718 pts/1    00:00:00 ps
````

 Puis on tue le processus avec la commande ```kill -9 PID``` où PID = 9651 ici. On vérifie que le processus est bien terminé avec ```ps```:

````
 PID TTY          TIME CMD
 9487 pts/1    00:00:00 bash
 9724 pts/1    00:00:00 ps
[1]+  Killed                  ./run.sh
````

**Question 8** : En regardant le fichier *gps/Makefile*, que pouvez-vous dire
                 de la règle *ok*? À quoi sert-elle et comment
                 fonctionne-t-elle?

La règle **ok** sert à compiler le simulateur avec la variable **GPS_OK = 1** donc à *false*. Le simulateur n'exécutera donc jamais à l'instruction ```puts(NULL);``` et il n'y aura jamais de *core dumped*. Pour cela, on se place dans le dossier */gps* et on exécute ```make ok``` puis ```./run.sh```. Dans le cas où on exécute ```make```, la règle appelée par défaut est **all** et **GPS_OK = 0** donc on obtient un core dumped à l'exécution ```./run.sh```.

### À retenir

  * comment utiliser le *man*
  * la mise en place d'un gestionnaire de signaux grâce à *sigaction*
  * l'utilité et le fonctionnement de *LD_PRELOAD*


## Exercice 3 : Terminal série (minicom)

Dans le cas d'une communication série avec du matériel, il est toujours
intéressant d'observer les trames. En effet, on peut par exemple comprendre un
protocole de communication grâce à de la rétro ingénierie. Pour cela, il existe
des programmes tel que *gtkterm*, *PuTTY*, *picocom* et bien d'autres. Lors de
ce TP, nous allons utiliser *minicom*.

**Question 1** : Grâce à l'aide en ligne de commande de *minicom* (option
                 *-h*), se connecter au port du simulateur GPS et observez les
                 trames passées.

**Question 2** : Naviguez dans l'aide de minicom (Ctrl-A Z) pour récupérer la
                 configuration du port de communication. Expliquez brièvement
                 la signification de ces paramètres.

**Question 3** : Utiliser minicom pour capturer un extrait de la
                 communication dans un fichier.
