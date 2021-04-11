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

Ce champ correspond à l'adresse du port virtuel par le processus du simulateur GPS. Il s'agit d'un pseudo-terminal (émulé) qui possède comme un vrai terminal un processus associé.

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

````
./gps_reader -p /dev/pts/3
$GPVTG,054.7,T,034.4,M,005.6,010.3,K
$GPGLL,4836.90,N,00741.30,E,140613,A
$GPVTG,054.8,T,034.5,M,005.6,010.3,K
$GPGLL,4836.96,N,00741.36,E,140617,A
$GPVTG,054.8,T,034.5,M,005.6,010.3,K
````

**Question 2** : En regardant le code de *reader.c*, y a-t-il quelque chose qui
                 vous chagrine?

Pourquoi on vérifie la lecture de données GPS sur plusieurs *file descriptors* (**fd**) sachant que les données GLL et VTG sont envoyées sur un unique **fd** qui correspond à celui du port virtuel **ptmx** ? Pourquoi utiliser un multiplexage d'entrées sachant qu'on écoute qu'un port ici ?

**Question 3** : Grâce à des recherches Internet (ou en fouinant dans le code
                 du simulateur), déterminez dans quelle trame et dans quel champs
                 l'heure est définie.

L'heure est dans la tram GLL en avant-dernière position (6ème position) : $GPGLL,4836.90,N,00741.30,E,**140613**,A. Ici, l'heure est 14:06:13 UTC.

**Question 4** : Quelles fonctions sont utilisées dans *reader.c* pour
                 ouvrir/écouter/lire/fermer le port virtuel du simulateur?
                 Comment s'appelle ce type de programmation?

Les fonctions utilisées dans *reader.c* sont :
* **open** pour ouvrir le port virtuel du simulateur et récupérer le descripteur de fichier associé.
* **select** pour écouter les ```fd + 1``` descripteurs de fichier de lecture de données dans l'ensemble *fdset*.
* **read** pour lire les données du descripteur de fichier **fd** et les mettre dans le *buffer* **buff**.
* **close** pour fermer le port virtuel associé au descripteur de fichier **fd**.
Il s'agit d'un multiplexage d'entrées : le processus **gps_reader** peut donc gérer l'arrivée d'informations à travers plusieurs canaux de type différents.

**Question 5** : Modifiez le code de *reader.c* afin qu'il puisse écouter les
                 trames provenant de deux simulateurs GPS différents (ports
                 paramétrables au lancement). Vérifiez le bon fonctionnement en
                 lançant deux instances du simulateur GPS.

````
./gps_reader -p /dev/pts/3 -q /dev/pts/6
/dev/pts/3
fd = 3 
/dev/pts/6
fd = 4 
GPS fd = 3 ------- $GPVTG,059.5,T,039.2,M,010.3,019.1,K
GPS fd = 4 ------- $GPVTG,058.8,T,038.5,M,009.6,017.7,K
GPS fd = 3 ------- $GPGLL,4905.62,N,00810.04,E,171422,A
GPS fd = 4 ------- $GPGLL,4900.94,N,00805.36,E,171422,A
````

**Question 6** : Utilisez *syslog* pour afficher l'heure dans la console ainsi
                 que le PID du père.


**Question 7** : Modifier la conf du démon syslog pour logger dans un
                 un fichier spécifique.

Pour configurer *rsyslog* comme on souhaite, ```cd /etc/```. On vérifie lle fichier de configuration global *rsyslog.conf* et notamment la présence de cette ligne : ```$IncludeConfig /etc/rsyslog.d/*.conf```.

Puis ```cd /etc/rsyslog.d/``` pour créer une configuration personnalisée *30-test.conf* contenant par exemple les lignes suivantes :
````
#if $programname == 'test' then /var/log/test.log
local0.=info /var/log/test.log
local0.=debug /dev/console #/dev/pts/3 /proc/5362/fd/1
````
Par défaut, *rsyslog* utilise la facilité **LOG_USER**, mais pour un usage local, on peut aussi utiliser **LOG_LOCAL0** à **LOG_LOCAL7**. Ici, si la facilité utilisée lors de l'appel à *syslog* est **LOG_LOCAL0** et la priorité est **LOG_INFO** alors le message sera écrit dans le log personnalisé *test.log*. Si la priorité est **LOG_DEBUG**, le message sera écrit dans la console.

Par défaut, *rsyslog* écrit ses messages dans */var/log/syslog*. Ici, ils sont écrits dans */var/log/test.log*.

On exécute ensuite ```sudo service rsyslog restart``` pour prendre en compte les changements de configuration.

Dans le fichier *reader.c*, la fonction **logger** permet décrire des messages de priorité *info* contenant le PID du père dans le *test.log*, et des messages *debug* pour la console.

Puis, on lance 2 instances du simlateur et le *gps_reader*. On obtient alors dans le *test.log*:
````
Apr  5 18:00:18 aspire-a515-54g test[19016]: PPID 9331
````
On a donc les informations suivantes :
* la date et l'heure
* le nom de la machine utilisée
* le nom du log
* [le PID du processus de log]
* le message du log : le PID du père (ici la console) du processus de log 

Liste des références :
* ```man 3 syslog``` -> priority = facility | level
* https://unix.stackexchange.com/questions/11953/make-a-log-file
* https://unix.stackexchange.com/questions/40533/lubuntu-12-04-syslog-to-custom-file-not-var-log-syslog-but-var-log-mylog
* https://www.thegeekdiary.com/understanding-rsyslog-filter-options/


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

Le paramètre *myshm* indiqué via l'option *-s* de *shm_writer* correspond au nom du segments de mémoire partagée à ouvrir, et *lock* au nom du sémaphore qui contrôlera les accès en lecture/écriture à cette mémoire partagée.

**Question 2** : Où peut-on trouver la représentation du segment de mémoire
                 partagée sur le système de fichiers?

On trouves les PID des 2 processus *gps* et *shm_writer* via :
````
$ ps -a
  PID TTY          TIME CMD
10535 pts/1    00:00:00 sh
10538 pts/1    00:00:00 gps
10539 pts/0    00:00:00 shm_writer
10622 pts/3    00:00:00 ps
````
Le segment de mémoire partagée est rattaché à un descripteur de fichier qui sera alors commun à ces 2 processus.
````
estellearrc@aspire-a515-54g:/proc/10538/fd$ ls
0  1  2  3
estellearrc@aspire-a515-54g:/proc/10539/fd$ ls
0  1  2  3  4
````
Sachant que 0 (stdin), 1 (stdout) et 2 (stderr) sont réservés, le seul descripteur commun est 3. Il s'agit donc du descripteur de fichier de la mémoire partagée entre les 2 processus. Le descripteur de fichier 4 correspond au fichier de sauvegarde (*backing file*) qui est synchronisé avec la mémoire partagée par le système.

https://opensource.com/article/19/4/interprocess-communication-linux-storage

**Question 3** : Faites un schéma bloc des différents éléments mis en jeu.

Voir *img/shared-memory.png*.

Placez vous dans le répertoire *shm_writer*.

**Question 4** : En étudiant la fonction *hndopen* implémentée dans le fichier
                 *handler.c*, décrivez les fonctions utilisées pour gérer le
                 segment de mémoire partagée.

### Initialisation

````
// init handlers
    hndinit(handlers);

void hndinit(struct HANDLERS *handlers)
{
    handlers->gpsfd = -1;
    handlers->sem = NULL;
    handlers->semname = NULL; 
    handlers->shmfd = -1;
    handlers->shm = NULL; 
    handlers->shdata = NULL; 
}
````

On a :
* **handlers->gpsfd** : descripteur de fichier dans lequel le processus *gps* écrit ses données
* **handlers->sem** : pointeur sur le sémaphore qui contrôle les accès en lecture/écriture du segment de mémoire partagée
* **handlers->semname** : nom du sémaphore
* **handlers->shmfd** : descripteur de fichier du fichier de sauvegarde pour le processus d'écriture dans la mémoire partagée *shm_writer*
* **handlers->shm** : nom du segment de mémoire partagée entre le processus *gps* et *shm_writer*
* **handlers->shdata** : pointeur sur les données gps dans le segment de mémoire partagée

### Ouverture du sémaphore
````
handlers->sem = sem_open(opts.sem, O_RDWR | O_CREAT,
                             S_IRUSR | S_IWUSR, 1);
````
On a :
* **opts.sem** : nom du sémaphore
* **O_RDWR | O_CREAT** : variables drapeaux qui contrôlent l'opération d'appel à la fonction *sem_open*. **O_CREAT** indique que le sémaphore est créé s'il n'existe pas, et **O_RDWR** indique que les opérations de lecture et d'écriture sont possibles à l'ouverture du sémaphore.
* **S_IRUSR | S_IWUSR** : permissions d'accès au sémaphore. Les opérations d'écriture et de lecture sont autorisées pour le créateur du sémaphore, ici le processus *shm_writer*.
* La valeur **1** indique que le sémaphore est déverrouillé. Le processus *shm_writer* a le droit de réaliser une opération de lecture (ou d'écriture)dans le segment de mémoire partagée. La valeur **0** indique bien sûr que le sémaphore est verouillé et que donc aucune opération ne peut être réalisée sur le segmet de mémoire partagée.

### Ouverture du port GPS
````
handlers->gpsfd = open(opts.port, O_RDWR | O_NOCTTY);
````

On a :
* **opts.port** : adresse (dans le système de fichiers) du port à ouvrir
*  **O_RDWR | O_NOCTTY** : **O_RDWR** autorise les opérations d'écriture et de lecture sur ce port, **O_NOCTTY** indique que le fichier ouvert n'est pas le terminal de contrôle du processus *gps* à l'ouverture du port.

### Ouverture de la mémoire partagée et projection

````
handlers->shmfd = shm_open(opts.shm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
````
La fonction *shm_open* retourne un descripteur de fichier pour le fichier de sauvegarde que le système synchronise avec la mémoire partagée.

````
ftruncate(handlers->shmfd, sizeof(handlers->shdata));
````
La fonction *ftruncate* alloue un certain nombre de bits (ici la taille de la structure de données *SHDATA*) au fichier de sauvegarde. Les processus *shm_writer* et *shm_reader* accèdent uniquement à la mémoire partagée et non pas au fichier de sauvegarde créé.

````
handlers->shdata = mmap(NULL, sizeof(handlers->shdata), PROT_READ | PROT_WRITE, MAP_SHARED, handlers->shmfd, 0);

````
La fonction *mmap* crée un nouveau *mapping* dans l'espace d'adresses virtuelles du processus appelant, et renvoie un pointeur vers la mémoire partagée.
On a :
* **NULL** : l'adresse de début du segment de mémoire partagée. Si elle est **NULL**, le système choisit l'adresse à laquelle créerle segment de mémoire partagée.
* **sizeof(handlers->shdata)** donne la longueur (en bits) du segment de mémoire partagée à mapper/synchroniser avec le fichier de sauvegarde.
*  **PROT_READ | PROT_WRITE** : drapeaux indiquant que le segment de mémoire partafée est protégé en lecture et en écriture (en accord avec le mode d'ouverture du fichier de sauvegarde).
* **MAP_SHARED** indique que les mises à jour du segment de mémoire partagée sont visibles par les autres processus ayant accès au segment, et que ces mises à jour sont reportées dans le fichier de sauvegarde.
* **handlers->shmfd** : descripteur de fichier du fichier de sauvegarde (*backing file* ou *underlying file*).
* **0** : la correspondance ou *mapping* entre le segment de mémoire partagée et le fichier de sauvegarde commence au premier bit du fichier de sauvegarde.

**Question 5** : Quelle fonction utilise le paramètre *myshm* passé en ligne de
                 commande?

La fonction *parse_args* du fichier *util.c* utilise le paramètre *myshm*.

**Question 6** : Quel flag en particulier indique une *création* de segment
                 et pas seulement une ouveture en lecture/écriture?

Il s'agit du flag **O_CREAT**.

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

On ajoute la librairie *realtime* en ajoutant l'option de compilation ```-lrt``` dans le *shm_reader/Makefile*.

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

````
time: 134709

time: 134709

time: 134709

time: 134709

time: 134709

time: 134709

time: 134709

time: 134709

time: 134713

time: 134713

time: 134713
````

La valeur de *time* affichée à l'écran est l'heure système à laquelle le gps a acquis sa donnée gps GLL et VTG, ici 13:47:09 pour la première donnée GPS.

### Code GPS 
````
 while(1)
    {
        sleep(2);
        write_gll(ptmx.fd);

        sleep(2);
        write_vtg(ptmx.fd);
    }
````

### Code lecteur de la mémoire partagée
````
 while(1)
    {
        if(handlers.shdata != NULL)
        {
            printf("\n");
            printf("time: %d\n", handlers.shdata->time);
            fflush(stdout);
        }
        usleep(500000);
    }
````

Une nouvelle donnée GPS (GLL et VTG) est publiée toutes les 4 secondes, et le lecteur de la mémoire partagée affiche les données toutes les 0.5 secondes. C'est pourquoi on a 8 fois l'affichage de la même heure (13:47:09 ici).

Dans le main de *shm_reader*, le paramètre *handlers* est défini en tant que
variable globale.

**Question 10** : Quelle est la particularité d'une variable globale? Comment
                  fait-on pour définir une telle variable?

Une variable globale est accessible et modifiable par l'ensemble des fichiers d'un même processus auxquels elle est incluse. Une variable globale est déclarée dans un *header* (fichier .h) pour qu'elle soit connue de tous lors de la compilation. 

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
On obtient :
````
time: 143345
latitude: 4918.629883
longitude: 823.070007

time: 143345
latitude: 4918.629883
longitude: 823.070007

time: 143349
latitude: 4918.689941
longitude: 823.130005

time: 143349
latitude: 4918.689941
longitude: 823.130005

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

Plus aucune donnée GPS n'est affichée dans le terminal. En effet, le dernier argument de la fonction *sem_open* définit la valeur initiale pour le nouveau sémaphore créé. La valeur **1** indique que le sémaphore est déverrouillé. Le processus *shm_writer* a le droit de réaliser une opération de lecture (ou d'écriture) dans le segment de mémoire partagée. 

Du côté du *shm_reader*, le sémaphore est ouvert avec la valeur **0** par défaut. La valeur **0** indique bien sûr que le sémaphore est verouillé et que donc aucune opération ne peut être réalisée sur le segment de mémoire partagée par le processus *shm_reader*.

Ainsi, si l'ouverture du sémaphore est initialisée à **0** par les 2 processus *shm_writer* et *shm_reader*, le sémaphore est verrouillé, personne ne peut accéder à la mémoire partagée et rien ne se passe.

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
Côté *shm_writer*, la fonction *hndclose* possède les lignes supplémentaires suivantes :
````
    // close gps port
    if (handlers->gpsfd != -1)
        close(handlers->gpsfd);

    // unlink semaphore
    if (handlers->semname != NULL)
        sem_unlink(handlers->semname);

    // unlink shm
    if (handlers->shm != NULL)
        shm_unlink(handlers->shm);
````
En effet, le processus *shm_writer* :
* lit les données sur le port virtuel du GPS, il faut donc fermer le fichier correspondant via la fonction *close*.
* a créé le sémaphore pour le segment de mémoire partagée avec le processus *shm_reader*. Il détruit donc le sémaphore via la fonction *sem_unlink* une fois que tous les processus, qui avaient le sémaphore ouvert, l'ont fermé.
* a crée le segment de mémoire partagée avec le processus *shm_reader*. Il détruit donc le segment via la fonction *shm_unlink* une fois que tous les processus, qui avaient le segment ouvert, l'ont fermé.

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
