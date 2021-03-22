# TP 1
### Quesstion 1 :
Un makefile est un fichier qui contient des paramètres pour une compilation avec gcc.<br/>
Make est un logiciel qui construit automatiquement des fichiers, souvent exécutables, ou des bibliothèques à partir d'éléments de base tels que du code source.<br/>
À la différence d'un simple script shell, make exécute les commandes seulement si elles sont nécessaires.

### Quesstion 2 :
Le compilateur utilisé est le g++ .

### Quesstion 3 :
Le fichier est TP1_Q4. la ligne pour compilé est : `g++ TP1_Q4.cpp -o hello -Wall`

### Quesstion 4 :
Le fichier que j'ai pris est ptmx.o qui se trouve dans le dossier src/lib/ptmx/<br/>
Voici la ligne de commande : `g++ -o libptmx.so -shared ptmx.o`

--------------------

# TP 2
## Exercice 1 -
### Question 1 :
Au bout de quelque seconde j'ai ceci :

	`PTTY: /dev/pts/1`
	`Segmentation fault (core dumped)`

Je peux en déduire qu'on essaye d'atteindre une zone mémoire où l'on a pas accès.

### Question 2 :
Il a reçus un signal d'erreur. On vérifie le signal du numéro avec la commande `echo $?`

### Question 3 :
La partie du code qui est fausse, c'est le `puts(NULL);` ligne 23 du fichier nmea.c.<br/>
La fonction puts doit prendre une chaine et donc il ne peut pas prendre un NULL.

### Question 4 :
On me demande de mettre un breakpoint sur le fichier, et quand j'essaie de le run, j'ai les bibliothèques libptmx.so qu'il ne trouve pas

### Question 5 :
La commande sert à avoir des informations sur les bibliothèque utilisées.<br/>
On sait qu'on a aussi la bibliothèque libnmea.so qu'il ne trouve pas.

### Question 6 :
Il fallait taper dans le terminal cette commande : `export LD_LIBRARY_PATH=$(pwd)/lib`

### Question 7 : 
Voici ce que j'ai :

    (gdb) s 
    21      if (iteration == 2)
    (gdb) n
    27	    return kmh;
    (gdb) 
    
Donc je peut en déduire que la commande s permet ....  <br/>
Et que la commande n permet de ....

### Question 8 :
Cette outils peut être intéréssante dans le contexte où nous avons pas fait le code et qu'il y a un problème, ou même dans<br/> l'import de nouvelle bibliothèque.


