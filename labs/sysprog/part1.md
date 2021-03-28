# Partie 1: Utilisation d'un simulateur GPS

## Norme NMEA

*La norme NMEA 0183 est une spécification pour la communication entre
équipements marins, dont les équipements GPS. Elle est définie et contrôlée
par la National Marine Electronics Association (NMEA), association américaine
de fabricants d'appareils électroniques maritimes, basée à Severna Park au
Maryland (États-Unis d'Amérique). La norme 0183 utilise une simple communication
série pour transmettre une phrase à un ou plusieurs écoutants. Une trame NMEA
utilise tous les caractères ASCII. (Wikipedia).*

Ces trames NMEA 0183 sont de tailles variables et codées en ASCII (caractères
8 bits) contrairement aux trames NMEA 2000. Elles commencent toutes par le
caractère *$* (excepté les trames venant de l'AIS commençant par *!*). Les deux
caractères suivants indiquent le type de matériel. Ainsi, on a par exemple pour
les trois premiers octets :

  * $GP : trame GPS
  * $HC : trame compas
  * $RA : trame radar
  * ...

Par exemple :

````
$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39
$GPGGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M, , *42
$GPGLL,4916.45,N,12311.12,W,225444,A
$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K
...
````

Aujourd'hui, nous allons travailler avec un simulateur GPS simplifié simulant
l'envoie de deux types de trames par liaison série virtuelle :

  * trame GLL : Geographic position, latitude / longitude
  * trame VTG : Track made good and ground speed

Ces trames sont envoyées periodiquement par le binaire *gps* que nous verrons
plus tard.

*NOTE* : Grâce aux équipements marins, une carte en ligne indique en temps réel
la position des navires : http://www.marinetraffic.com/fr/.

## Communication série

Dans la vraie vie, une communication série (ou USB) transmet ces trames NMEA
et un ordinateur peut alors les récupérer.

Lors de la connexion d'un port USB-Série, une entrée est créée par le kernel
dans le répertoire */dev/* (généralement par l'utilitaire *udev*). En examinant
les sorties du kernel, on peut trouver le port exact :

````
$ dmesg
...
[15976.212024] usb 2-2: new full-speed USB device number 2 using uhci_hcd
[15976.375039] usb 2-2: New USB device found, idVendor=067b, idProduct=2303
[15976.375044] usb 2-2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[15976.375047] usb 2-2: Product: USB-Serial Controller
[15976.375049] usb 2-2: Manufacturer: Prolific Technology Inc.
[15978.090200] usbcore: registered new interface driver usbserial
[15978.090219] usbcore: registered new interface driver usbserial_generic
[15978.090234] usbserial: USB Serial support registered for generic
[15978.174057] usbcore: registered new interface driver pl2303
[15978.174078] usbserial: USB Serial support registered for pl2303
[15978.174101] pl2303 2-2:1.0: pl2303 converter detected
[15978.186178] usb 2-2: pl2303 converter now attached to ttyUSB0
````

Ici, on s'attend à avoir un port */dev/ttyUSB0*.

Dans le cadre du TP, le simulateur va lui aussi créer une entrée dans
le */dev/*. Cependant, cette entrée étant virtuelle et créée par programmation,
elle ne sera pas visible à travers les messages kernel.


## Compilation du simulateur

Le simulateur GPS se trouve dans le répertoire *gps/* qui contient lui
même :

  * src : répertoire contenant les sources
  * bin : répertoire contenant les binaires (après compilation)
  * lib : répertoire contenant les librairies (après compilation)
  * include : répertoire contenant les headers (après compilation)
  * Makefile : fichier définissant les règles de compilation
  * run.sh : fichier lançant le simulateur GPS

Lancez la compilation :

````
$ make
````

Deux librairies sont compilées *lib/libnmea.so* et *lib/libptmx.so* ainsi que
le binaire *bin/gps*.

## Questions

En vous inspirant du contenu du répertoire *gps/*, répondre aux questions
suivantes:

**Question 1**: Qu'est ce qu'un Makefile? À quoi sert make?

Un MakeFile donne les instructions de compilation à suivre. La commande make sert à compiler le projet.

**Question 2**: Quel compilateur est utilisé ici? 

GCC = GNU C Compiler

**Question 3**: Qu'est ce qu'une librairie partagée? 

Les bibliothèques partagées sont du code compilé destiné à être partagé entre plusieurs différents programmes d'un projet. Elles ne sont exécutées que lorsqu'elles sont appelées. 

Extension .so avec le numéro de version de la libraire .so.5.12.2 par exemple. .so.5 et .so.5.12 sont des liens symboliques qui pointent vers la bonne version. La dernière extension .so.5.12.2 est le fichier. Les librairies sont des binaires une fois compilées mais ne sont pas exécutables car elles n'ont pas de thread prinipal. La commande ./library_name.so.5.12.2 génère un core dump. Seule la libc-2.32.so est exécutable et donne des informations sur sa version, sa licence, le compilateur utilisé, etc.

Localisations classiques des librairies partagées :
* /usr/lib
* /lib

**Question 4**: Donnez un exemple de fichier C et la ligne de commande
                correspondante pour obtenir un binaire exécutable (un hello
                world par exemple).

Il suffit d'exécuter la commande ```gcc hello_world.c -o hello``` à la racine du dossier **gps**.
Puis ```mv hello bin/ ``` pour déplacer le binaire dans le dossier **bin** ignoré par git.
On exécute le binaire avec la commande ```./bin/hello```.

**Question 5**: Donnez un exemple de fichier C et les lignes de commandes
                correspondantes pour obtenir une librairie partagée.
                
À la racine du dossier **gps** :
1. On compile d'abord le code source de la librairie en PIC : ```gcc -c -Wall -Werror -fpic foo.c``` puis ```mv foo.o src/lib```.

In computing, position-independent code(PIC) is a body of machine code that, being placed somewhere in the primary memory, executes properly regardless of its absolute address. PIC is commonly used for shared libraries, so that the same library code can be loaded in a location in each program address space where it does not overlap with other memory in use (for example, other shared libraries). 

2. On crée une librairie partagée (.so) à partir du fichier objet (.o) : ```gcc -shared -o ./lib/libfoo.so ./src/lib/foo.o```

3. Édition de lien avec la librairie partagée : ```gcc -L ./lib -Wall -o test main.c -lfoo```

4. Chargement et exécution de la librairie : ```export LD_LIBRARY_PATH=$(pwd)/lib``` puis ```mv test bin/``` puis
```./bin/test```. Dans le terminal, on a alors la sortie suivante : ```This is a shared library test... Hello, I am a shared library```.

https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html


## À retenir

  * make et Makefile
  * compilation d'un binaire exécutable
  * compilation d'une librairie partagée
