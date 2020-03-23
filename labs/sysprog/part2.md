# Partie 2: Compilation, debug et gestionn# Partie 2: Compilation, debug et gestionnaire de signaux


## Exercice 1 : GDB et fichier core

Une fois le simulateur GPS compilé, le lancer grâce au script *gps/run.sh* :

````
$ sh run.sh
PTTY: /dev/pts/X
````

**Question 1** : Que se passe-t-il au bout de quelques secondes? Qu'en
                 déduisez vous?
Une erreur Segmentation fault interrompt le programme. Un accès mémooire invalide
à été détecté.

**Question 2** : Quel signal a reçu le processus pour se terminer ainsi? Comment
                vérifiez vous le numéro du signal reçu?
echo $?
la réponse est 139 ce qui correspond au signal SIGSEGV. 

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

Le code essaie d'accéder à un fichier qui n'existe pas :
62	../sysdeps/x86_64/multiarch/strlen-avx2.S: Aucun fichier ou dossier de ce type.


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

Starting program: /home/affraike/Documents/Sys_Embarques/embsys/labs/sysprog/gps/bin/gps 
/home/affraike/Documents/Sys_Embarques/embsys/labs/sysprog/gps/bin/gps: error while loading shared libraries: libptmx.so: cannot open shared object file: No such file or directory


Suite au problème repéré, allez dans le répertoire *gps/bin* et lancez la
commande suivante :

````
ldd ./gps
````

**Question 5** : À quoi sert la commande *ldd*? Quelle information
                supplémentaire cela vous apporte-t-il?

Cela permet de visualiser les dépendances du programme avec les librairies partagées.

**Question 6** : Comment résoudre ce problème en tant qu'utilisateur? N'hésitez
                 pas à regarder le fichier *gps/run.sh*.

LD_LIBRARY_PATH=$(pwd)/lib ./bin/gps

Relancez *ldd* puis GDB pour vérifier que votre solution a porté ses fruits.

**Question 7** : Quelle est la différence entre les commandes *s* et *n* dans
                 le prompt gdb suite à un breakpoint?
step
Continue running your program until control reaches a different source line, then stop it and return control to GDB.
next
Continue to the next source line in the current (innermost) stack frame. This is similar to step, but function calls that appear within the line of code are executed without stopping. Execution stops when control reaches a different line of code at the original stack level that was executing when you gave the next command.

Il existe aussi une version de GDB pour déboguer à distance. Il y
a alors un GDBServer tournant sur la cible où le programme à déboguer est
exécuté. Ensuite, un client GDB tourne sur la machine servant à déboguer
et communique avec le serveur grâce au réseau.

**Question 8** : Dans quel contexte ce type d'outils peut être intéressant?
Si on cherche a optimiser son OS, il s'agit d'un outils de base utilisant peu de ressources.

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
