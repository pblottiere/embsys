# Partie 2: Compilation, debug, signaux et threads


## Exercice 1 : GDB et fichier core

### Les questions

Une fois le simulateur GPS compilé, le lancer grâce au script
*labs/gps/run.sh* :

````
$ sh run.sh
PTTY: /dev/pts/X
````

**Question 1** : Que se passe-t-il au bout de quelques secondes? Qu'en
                 déduisez vous?

**Question 2** : Quel signal a reçu le processus pour se terminer ainsi? Comment
                vérifiez vous le numéro du signal reçu?

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

Suite au problème repéré, allez dans le répertoire *labs/sysprog/gps/bin* et
lancez la commande suivante :

````
ldd ./gps
````

**Question 5** : À quoi sert la commande *ldd*? Quelle information
                supplémentaire cela vous apporte-t-il?

**Question 6** : Comment résoudre ce problème en tant qu'utilisateur? N'hésitez
                 pas à regarder le fichier *labs/sysprog/gps/run.sh*.

Relancez *ldd* puis GDB pour vérifier que votre solution a porté ses fruits.

**Question 7** : Quelle est la différence entre les commandes *s* et *n* dans
                 le prompt gdb suite à un breakpoint?

Il existe aussi une version de GDB pour déboguer à distance. Il y
a alors un GDBServer tournant sur la cible où le programme à déboguer est
exécuté. Ensuite, un client GDB tourne sur la machine servant à déboguer
et communique avec le serveur grâce au réseau.

**Question 8** : Dans quel contexte ce type d'outils peut être intéressant?

### Ce qu'il faut retenir

Les concepts suivants sont considérés comme acquis:

  * l'utilité de *ulimit* et comment déclencher la génération d'un fichier core
  * à quoi sert *GDB* et comment l'utiliser
  * l'utilité de *ldd*
  * pourquoi, quand et comment utiliser la variable d'environnement
    *LD_LIBRARY_PATH*
