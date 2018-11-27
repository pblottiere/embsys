# Partie 3: Multiplexage, threads, mutex et IPC

Pour cette partie, compilez le simulateur en mode sans-échec:

````
$ cd gps
$ make clean
$ make ok
````

## Exercice 1 : Multiplexage

Lors du lancement du simulateur GPS, nous obtenons le message suivant :

````
$ sh run.sh
PTTY: /dev/pts/3
````

**Question 1** : Selon vous, à quoi correspond le champs indiqué par
                *PTTY*?

Pour la suite, placez vous dans le répertoire *gps_reader* contenant :

  * reader.c : le code du reader que nous allons modifier
  * Makefile : les règles de compilation
  * util.c / util.h : des fonctions utiles pour l'exercice

Pour compiler, lancez :

````
$ make
````

Un binaire *gps_reader* est alors généré.

Lancez le reader sans paramètre pour avoir l'aide et en déduire son utilisation.
Puis exécutez le avec les paramètres nécessaires et observez les trames NMEA.

**Question 2** : En regardant le code de *reader.c*, y a-t-il quelque chose qui
                 vous chagrine?

**Question 3** : Grâce à des recherches Internet (ou en fouinant dans le code
                 du simulateur), déterminez dans quelle trame et dans quel champs
                 l'heure est définie.

**Question 4** : Quelles fonctions sont utilisées dans *reader.c* pour
                 ouvrir/écouter/lire/fermer le port virtuel du simulateur?
                 Comment s'appelle ce type de programmation?

**Question 5** : Modifiez le code de *reader.c* afin qu'il puisse écouter les
                 trames provenant de deux simulateurs GPS différents (ports
                 paramétrables au lancement). Vérifiez le bon fonctionnement en
                 lançant deux instances du simulateur GPS.

**Question 6** : Utilisez *syslog* pour afficher l'heure dans la console ainsi
                 que le PID du père.

**Question 7** : Modifier la conf du démon syslog pour logger dans un
                 un fichier spécifique.


### À retenir

  * comment utiliser *getopt* pour passer des paramètres en ligne de commande
    à votre application
  * comment utiliser *select* pour monitorer plusieurs file descriptors
  * l'utilisation globale de *syslog*


## Exercice 2 : Mémoire partagée et sémaphore

Comme vu précédement, le simulateur GPS envoie les données sur un port virtuel
à travers /dev/pts/X.

Le binaire *shm_writer* présent dans le répertoire *shm_writer* lit les données
envoyées par le simulateur GPS sur le port virtuel et écrit en mémoire partagée
les informations *latitude*, *longitude* et *time*.

Pour compiler le binaire *shm_writer* :

````
$ cd shm_writer
$ make
````

Les données en mémoire partagée sont représentées via la structure SHDATA
définie dans le header *shm_writer/shdata.h* :

````
struct SHDATA
{
    float latitude;
    float longitude;
    int time;
};
````

Le but de cet exercice est de lire la mémoire partagée remplie par le binaire
*shm_writer* et d'afficher ces informations à l'écran toutes les secondes en
modifiant le code de *shm_reader*.

Tout d'abord, ouvrez un terminal et lancez le simulateur GPS :

````
$ cd gps
$ sh run.sh
PTTY: /dev/pts/X

````

Ouvrez un second terminal et lancez le binaire *shm_writer* :

````
$ cd shm_writer
$ ./shm_writer -p /dev/pts/X -s myshm -l lock

````

**Question 1** : Selon vous, à quoi correspond le paramètre *myshm* indiquée via
                 l'option *-s* de *shm_writer*? Et *lock*?

**Question 2** : Où peut-on trouver la représentation du segment de mémoire
                 partagée sur le système de fichiers?

**Question 3** : Faites un schéma bloc des différents éléments mis en jeu.

Placez vous dans le répertoire *shm_writer*.

**Question 4** : En étudiant la fonction *hndopen* implémentée dans le fichier
                 *handler.c*, décrivez les fonctions utilisées pour gérer le
                 segment de mémoire partagée.

**Question 5** : Quelle fonction utilise le paramètre *myshm* passé en ligne de
                 commande?

**Question 6** : Quel flag en particulier indique une *création* de segment
                 et pas seulement une ouveture en lecture/écriture?

Placez vous maintenant dans le répertoire 2_sysprog_part2/src/shm_reader.

**Question 7** : Modifiez la fonction *hndopen* implémentée dans *handler.c*
                 pour ouvrir le segment de mémoire partagée en lecture/écriture.
                 Les champs *shm*, *shmfd* et *shdata* de la structure
                 *handlers* passée en paramètre doivent être mis à jour.

En voulant compiler le binaire *shm_reader*, vous devez obtenir ceci :

````
$ make
/tmp/ccawsZJY.o: In function `hndopen':
handler.c:(.text+0x66): undefined reference to `shm_open'
collect2: error: ld returned 1 exit status
````

Pour utiliser les fonctions liées à la mémoire partagée, la librairie *realtime*
est nécessaire.

**Question 8** : En s'inspirant de *shm_writer/Makefile*, modifiez le fichier
                 *shm_reader/Makefile* pour que la compilation passe.

Une fois la compilation réalisée avec succès, exécutez *shm_reader*. Vous devez
obtenir :

````
$ make
$ ./shm_reader -s myshm -l lock
time : XXXXXX

time : XXXXXX

...
````

**Question 9** : Expliquez l'évolution de la valeur *time* affichée à l'écran.

Dans le main de *shm_reader*, le paramètre *handlers* est défini en tant que
variable globale.

**Question 10** : Quelle est la particularité d'une variable globale? Comment
                  fait-on pour définir une telle variable?

**Question 11** : Dans le *main* de shm_reader.c, complétez la boucle *while* de
                  la fonction shmreader afin que les champs *latitude* et
                  *longitude* du segment de mémoire partagée *handlers.shdata*
                  soient affichés en même temps que le champ *time*.

**Question 12** : Inspirez vous de la fonction *decode_frame* de
                  shm_writer/shm_writer.c pour lever/baisser le sémaphore lors
                  de la lecture du segment de mémoire partagée dans la fonction
                  *shmreader* de *shm_reader.c*.

Recompilez et exécutez *shm_reader*. Vous devez obtenir :

````
$ make
$ ./shm_reader -s myshm -l sem
time : XXXX
latitude : XXXX
longitude : XXXX

time : XXXX
latitude : XXXX
longitude : XXXX

...
````

Modifiez la fonction *sem_open* utilisée dans la fonction *hndopen* et définie
dans *shm_writer/handler.c* de cette manière :

````
// handlers->sem = sem_open(opts.sem, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, 1);
handlers->sem = sem_open(opts.sem, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, 0);
````

Recompilez *shm_writer*. Relancez *shm_writer* puis *shm_reader*.

**Question 13** : Que se passe t-il côté *shm_reader*? Pourquoi? Quel effet a eu
                  la modification précédente?

Rétablissez l'appel à *sem_open* du fichier *shm_writer/handler.c* ainsi :

````
handlers->sem = sem_open(opts.sem, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, 1);
````

Lorsque l'utilisateur interrompt le processus via Ctrl-C, la fonction *hndclose*
n'est jamais appellée et les handlers d'entrées/sorties ne sont pas fermés
correctement.

**Question 14** : Mettez en place un gestionnaire de signaux qui appelle la
                  fonction *hndclose* lors d'une interruption. Inspirez vous de
                  *shm_writer/shm_writer.c*.

**Question 15** : Comparez la fonction *hndclose* définie dans
                  *shm_writer/handler.c* avec celle définie dans
                  *shm_reader/handler.c*. Quelle différence voyez vous?
                  Expliquez.

### À retenir

  * la notion de variable globale
  * les fonctions permettant de gérer les segments de mémoire partagée :
    *shm_open*, *ftruncate*, *mmap* et *shm_unlink*.
  * l'utilisation de *sem_open*, *sem_wait* et *sem_post*.

## Exercice 3 : thread et mutex

Dans le cadre de ce troisième exercice, nous allons naviguer dans le code
source du répertoire *converter*.

Les coordonnées latitude et longitude écrites en mémoire partagée par le
binaire *shm_writer* sont définies en degré et minute (norme NMEA). Le
but est ici de mettre en place un thread chargé de convertir ces coordonnées au
format decimal et de les rendre disponibles à travers une structure globale
accessible par tous les threads du binaire *converter* et protégée par mutex.

Le simulateur *gps* et *shm_writer* doivent toujours être actifs pour la suite
de l'exercice.

Placez vous dans le répertoire *converter*.

Dans un premier temps, compilez et exécutez *converter* :

````
$ cd converter
$ make
$ ./converter -s myshm -l lock
time : 0
latitude : 0
longitude : 0

time : 0
latitude : 0
longitude : 0
...
````

La fonction affichant periodiquement les coordonnées decimales est exécutée
dans un premier thread par la fonction *display* définie dans *converter.c*.

Dans la suite de l'exercice, le but est de mettre en place un deuxième thread
chargé de mettre à jour les coordonnées decimales qui sont affichées par le
premier thread, le tout vérouillé par un mutex.

**Question 1** : Faites un schéma bloc des éléments à mettre en jeu pour
                 atteindre notre but (simulateur GPS et shm_writer compris).

**Question 2** : Décrivez les deux fonctions utilisées dans converter.c pour la
                 mise en place du thread1 affichant les coordonnées.

**Question 3** : Utilisez la fonction *pthread_create* afin d'exécuter la
                 fonction *convert* dans un deuxième thread.

Comme dans l'exercice précédent, la structure projetée en mémoire partagée est
accessible à travers la variable globale *handlers.shdata*.

**Question 4** : Modifiez la fonction *convert* afin que la variable globale
                 *decimal_coord* soit mise à jour avec les coordonnées
                 decimales. Vous pouvez utiliser la fonction *to_decimal* pour
                 la conversion.

Compilez et exécutez :

````
$ make
$ ./converter -s myshm -l lock
time : XXXX
latitude : XXXX
longitude : XXXX

time : XXXX
latitude : XXXX
longitude : XXXX
...
````

**Question 5** : Utilisez la variable mutex *mut* passée en paramètre des
                fonctions *convert* et *display* pour verrouiller l'accès à
                *decimal_coord* entre les 2 threads.

### À retenir

  * les fonctions de base permettant de gérer les threads : *pthread_create*,
    et *pthread_join*.
  * les mutex avec *pthread_mutex_lock* et *pthread_mutex_unlock*.
