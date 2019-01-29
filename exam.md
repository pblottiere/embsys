# CAMERA IP

L'objectif est ici de mettre en place une caméra IP sur servomoteur à l'aide
d'une RPI3.

L'utilisateur doit donc pouvoir:

1. prendre 1 photo et la visualiser
2. contrôler la position de la caméra grâce au servomoteur

Pour cela, l'utilisateur possède simplement l'addresse IP de la RPI3.

## Architecture

Côté architecture, il est conseillé de développer:

- un client pour l'utilisateur permettant de capturer une image et de gérer
  la position du servomoteur (via l'IP de la RPI3)
- un serveur côté RPI3 recevant les commandes venant de l'utilisateur
  pour gérer le servomoteur
- un serveur côté RPI3 recevant les commandes venant de l'utilisateur
  pour gérer la caméra

Pour information, le nombre de lignes de code permettant d'avoir un produit
complet et opérationnel est évalué à 1000 LOC environ (pour vous donner un
ordre de grandeur).

### Client

Pour le client, vous êtes libre d'utiliser le language que vous voulez. Vous
pouvez aussi proposer soit un outil en ligne de commande, soit une interface
graphique.

### Serveur servomoteur

Pour contrôler un servomoteur via une RPI3, il faut utiliser une pin GPIO en
mode PWM. Il faut donc ici utiliser le module Python `RPi` comme lors des TPs.

Le serveur permettant de contrôler le servomoteur doit donc être développé en
Python.

### Serveur caméra

Afin de contrôler la caméra via la RPI3, il est conseillé ici d'utiliser l'API
C V4L (expliquer plus en détail après). Le serveur de contrôle de la caméra
doit donc être développé en C.

## Livrables attendus

Livrables:

- dépôt Github dédié à votre projet
- le code
- README exhaustif (comment utiliser Buildroot, flasher la carte, compiler et
  installer votre outil, documentation utilisateur, etc)

Critères de notation:

- Produit fonctionnel
- Architecture du code et modularité (librairies, ...)
- Outils de build (autotools, CMake ou Makefile)
- Utilisation des notions vues en TP (getopt, syslog, signal handler, ...)
- Normes de codage

## RPI

### Buildroot

Pour avoir le support de la caméra sur la RPI3, un firmware spécifique
doit être compilé avec Buildroot. Pour cela, il faut activer l'option
`BR2_PACKAGE_RPI_FIRMWARE_X` dans le fichier de configuration de Buildroot.

De plus, afin d'intéragir avec la caméra, l'API de
[V4L](https://linuxtv.org/downloads/v4l-dvb-apis/uapi/v4l/v4l2.html) est
conseillée. Il est donc nécessaire d'activer l'option `BR2_PACKAGE_LIBV4L`
dans le fichier de configuration Buildroot.

Votre système d'exploitation précompilé grâce à Buildroot est disponible
via une image Docker (comme lors des TPs):

````
$ docker pull pblottiere/embsys-rpi3-buildroot-video
````

Pour créer un conteneur:

````
$ docker run -it pblottiere/embsys-rpi3-buildroot-video /bin/bash
docker# cd /root
docker# ls
buildroot-precompiled-2017.08.tar.gz
docker# tar zxvf buildroot-precompiled-2017.08.tar.gz
````

Le nécessaire pour flasher la carte RPI3 avec le support de la caméra est
alors disponible:

- `sdcard.img` à flasher sur la carte SD avec la commande `dd`
- `start_x.elf` et `fixup_x.dat` à copier avec la commande `cp` sur la 1ère
  partition de la carte SD

Il faut finalement modifier le fichier `config.txt` de la 1ère partition
de la carte SD pour ajouter:

````
start_x=1
gpu_mem=128
````

### V4L

Une fois la RPI3 démarrée, il faut tout d'abord charger le module kernel
`bcm2835-v4l2` grâce à la commande `modprobe` pour utiliser la caméra. Suite à
cela, le device `video0` doit être accessible à travers le FS:

````
rpi3# ls /dev/video0
````

Afin de tester le bon fonctionnement de la caméra, vous pouvez dans un
premier temps récupérer le projet `https://github.com/twam/v4l2grab` et le
cross-compiler grâce à la chaîne de cross-compilation fourni par Buildroot
dans le conteneur Docker. Le projet `v4l2grab` utilise les Autotools comme
outil de build, vous pouvez donc vous référer à la documentation suivante
pour obtenir les options nécessaire à la cross-compilation:
https://www.gnu.org/software/autoconf/manual/autoconf-2.69/html_node/Hosts-and-Cross_002dCompilation.html
(note: il est possible que vous deviez modifier le code pour compiler avec
succès).

Puis, une fois le résultat de la cross-compilation copié sur la carte RPI3,
exécuter `v4l2grab` pour capturer une image et vérifier le contenu de l'image:

````
rpi3# ./v4l2grab -d /dev/video0 -o image.jpg
````

Finalement, étudiez le contenu du fichier `v4l2grab.c` pour en extraire le
minimum utile dans votre contexte.
