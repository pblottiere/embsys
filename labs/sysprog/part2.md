# Partie 2: Compilation, debug et gestionnaire de signaux


## Exercice 1 : GDB et fichier core

Une fois le simulateur GPS compilé, le lancer grâce au script *gps/run.sh* :

````
$ sh run.sh
PTTY: /dev/pts/X
````

**Question 1** : Que se passe-t-il au bout de quelques secondes? Qu'en
                 déduisez vous?

```bash
$ sh ./run.sh
PTTY: /dev/pts/2
Segmentation fault (core dumped)
```

Il y a un "Core Dumped". C'est sûrement dû à une fuite memoire.

**Question 2** : Quel signal a reçu le processus pour se terminer ainsi? Comment
                vérifiez vous le numéro du signal reçu?

Avec GDB et la commande *bt* on peut obtenir le signal qui a terminé le programme :

```bash
Program terminated with signal SIGSEGV, Segmentation fault.
```
Le programme s'est donc terminé sur un signal **SIGSEV**.

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

```bash
#0  __strlen_sse2 () at ../sysdeps/x86_64/multiarch/../strlen.S:120
#1  0x00007f5a1fc129d2 in _IO_puts (str=0x0) at ioputs.c:35
#2  0x00007f5a1ff83aab in knot_to_kmh_str (not=5.51000023, size=6, 
    format=0x7f5a1ff83f6f "%05.1f", kmh_str=0x7ffe51e0e7e2 "010.2")
    at nmea.c:23
#3  0x00007f5a1ff83ef6 in nmea_vtg (vtg=0x7ffe51e0e820) at nmea.c:178
#4  0x0000558712cacc5c in write_vtg (fd=3) at gps.c:40
#5  0x0000558712cacee1 in main () at gps.c:109

```

Dans la fonction *knot_to_kmh_str* qui se trouve dans le fichier nmea.c ligne 23, il y a un *puts(NULL);* qui n'est pas apprécié. 

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

```bash
/home/quentin/Code/embsys/labs/sysprog/gps/bin/gps 
/home/quentin/Code/embsys/labs/sysprog/gps/bin/gps: error while loading shared libraries: libptmx.so: cannot open shared object file: No such file or directory
[Inferior 1 (process 8247) exited with code 0177]
```

Suite au problème repéré, allez dans le répertoire *gps/bin* et lancez la
commande suivante :


```bash
$ ldd ./gps
  linux-vdso.so.1 (0x00007ffd156c3000)
  libptmx.so => not found
  libnmea.so => not found
  libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f3cfb8d8000)
  /lib64/ld-linux-x86-64.so.2 (0x00007f3cfbecc000)

```

**Question 5** : À quoi sert la commande *ldd*? Quelle information
                supplémentaire cela vous apporte-t-il?

                ldd permet d'afficher les dépendances de librairies partagées.
                Visiblement il manque les librairies partagées *libptmx.so* et *libnmea.so*

**Question 6** : Comment résoudre ce problème en tant qu'utilisateur? N'hésitez
                 pas à regarder le fichier *gps/run.sh*.

```bash
SCRIPT=`readlink -f $0`
ROOT_DIR=`dirname $SCRIPT`

export LD_LIBRARY_PATH=$ROOT_DIR/lib
```

Va permettre ici d'ajouter la variable d'environnement des librairies dynamiques, le dossier des libraries manquantes générées par notre projet (ici *libptmx.so* et *libnmea.so*).

Relancez *ldd* puis GDB pour vérifier que votre solution a porté ses fruits.

**Question 7** : Quelle est la différence entre les commandes *s* et *n* dans
                 le prompt gdb suite à un breakpoint?

s : commande step qui permet d'avancer pas à pas dans l'execution
n : commande next qui permet d'avancer à l'instruction suivante ... utile apparement avec les switch ... case

Il existe aussi une version de GDB pour déboguer à distance. Il y
a alors un GDBServer tournant sur la cible où le programme à déboguer est
exécuté. Ensuite, un client GDB tourne sur la machine servant à déboguer
et communique avec le serveur grâce au réseau.

**Question 8** : Dans quel contexte ce type d'outils peut être intéressant?

Typiquement dans le domaine du système embarqué, avec un robot en phase de tests. Pour tester des progammes, il nous faut le hardware du robot. Du coup il faut lancer le programme sur le robot et donc le débugger dessus.

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

Ca marche :sunglasses:

Nous avons ici hooké une fonction définie dans une librairie "utilisateur". On
peut réaliser la même opération sur les librairies systèmes. Par exemple, le
simulateur GPS utilise la fonction *printf* dès son lancement.

**Question 4** : Utilisez le *man* pour déterminer le prototype de la fonction
                 *printf* (expliquez comment vous utilisez *man* dans ce cas et
                 pourquoi). Comment est appelé ce type de fonction?

```bash
man 3 printf
```
Pour lire la troisième page du manuel. On y trouve alors l'entête de la fonction printf et on remarque qu'elle acceptent un nombre d'arguments pouvant varier (en fonction du nombre defomratage de texte à effectuer). C'est donc une fonction variadique.

```c
#include <stdio.h>
int printf(const char *format, ...);
```

**Question 5** : Analysez *gps/src/bin/gps/gps.c* er repérez où se trouve le
                 gestionnaires de signaux. Décrivez les fonctions utilisées
                 ainsi que les signaux gérés.

Le fichier *gps.c* utilise les fonctions d'entête suivantes :

```c
void signals_handler(int signal_number);
    /* Permet de traiter les signaux entrants */
int write_vtg(int fd);
    /* Permet d'écrire la trame vtg sur le port série virtuel */
int write_gll(int fd);
    /* Permet d'écrire la trame gll sur le port série virtuel */
```

Le signal qui est repéré par le programme est le signal SIGINT, signal d'interruption de programme. Il peut être déclanché par un Ctrl+C de l'utilisateur par exemple.

**Question 6** : Hookez le simulateur pour que ce dernier ne puisse plus
                 être interrompu par le signal SIGINT (Ctrl-C) en
                 réimplémentant la fonction *printf* dans libhook.so. Pour
                 cela, utilisez la fonction *sigaction* pour mettre en place
                 un gestionnaire de signaux au sein même de la fonction
                 *printf*  réimplémentée.



**Question 7** : Comment faire pour interrompre le processus étant donné
                 que ce dernier ne répond plus au Ctrl-C? Citez deux méthodes.

On peut tuer le processus en connaissant son PID et avec la commande *kill*, ou alors en utilisant un autre signal comme SIGQUIT (Ctrl-Alt gr-\ )

**Question 8** : En regardant le fichier *gps/Makefile*, que pouvez-vous dire
                 de la règle *ok*? À quoi sert-elle et comment
                 fonctionne-t-elle?

La règle ok va lancer les règles suivantes :

```bash
ok: init libptmx libnmea_ok gps
```

La différence avec la règle *all*, c'est que la librairie libnmea sera build en version *ok* elle aussi. Lorsque l'on va dans le makefile de cette libraire, on a cela pour la règle *ok* :

```bash
ok:
  $(GCC) -D GPS_OK=1 -c -fPIC nmea.c -o nmea.o
  $(GCC) -shared -Wl,-soname,$(SONAME) -o $(SONAME) nmea.o -lm
  cp nmea.h ../../../include
  mv $(SONAME) ../../../lib
```

On voit que la librairie nmea va être construite avec le paramètre GPS_OK=1. Donc la partie de code suivante ne sera plus compilée et exécutée car la variable GPS_OK sera déjà définie :

```c
#ifndef GPS_OK
    iteration++;
    if (iteration == 2)
    {
        puts(NULL);
    }
#endif
```

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
