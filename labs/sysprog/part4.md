# Partie 4 : Programmation Réseau


Le but de l'exercice est d'envoyer periodiquement les coordonnées de latitude
et longitude sur le réseau. L'envoi se fera par UDP.

Le simulateur *gps* et *shm_writer* doivent toujours être actifs pour la suite
de l'exercice. Pour cet exercice, on doit aussi compiler et exécuter le binaire
*net_reader* dans un terminal séparé :

````
$ cd net_reader
$ make
$ ./net_reader -p 8888

````

Dans le cadre de ce troisième exercice, nous allons naviguer dans le code
source du répertoire *forwarder*.

**Question 1** : À quoi correspond ici le paramètre 8888? Quel outil
                 pouvez vous utiliser pour vérifier qu'une connexion réseau
                 est bien ouverte?

**Question 2** : En analysant le fichier *handler.c*, indiquez les fonctions
                 liées à la programmation de socket.

**Question 3** : Toujours dans handler.c, quels flags indiquent une connexion
                 UDP? Quelle est la particularité de l'UDP par rapport à du
                 TCP?

**Question 4** : Encore dans handler.c, à quoi sert le premier paramètre de la
                 fonction *inet_aton*?

**Question 5** : Dans la fonction *forward* du fichier forwarder.c, utilisez la
                fonction *sendto* pour envoyer la variable *message* sur le
                réseau.

Compilez puis exécutez correctement le binaire *forwarder*. Vous devriez
obtenir ceci du côté *net_reader* :

````
$ ./net_reader -p 8888
received: 000000/49.3055/8.3795
received: 223222/49.3065/8.3805
...
````

**Question 6** : Faites un schéma bloc de tous éléments mis en jeu pour arriver
                 ici.

## À retenir

  * les fonctions liées à la programmation réseau
