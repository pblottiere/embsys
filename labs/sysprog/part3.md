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

**Question 7** : TODO!!!! Modifier la conf du démon syslog pour logger dans un
                 un fichier spécifique!


### À retenir

  * comment utiliser *getopt* pour passer des paramètres en ligne de commande
    à votre application
  * comment utiliser *select* pour monitorer plusieurs file descriptors
  * l'utilisation globale de *syslog*
