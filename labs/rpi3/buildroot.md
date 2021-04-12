# Partie 1: Construction d'OS avec Buildroot et chaine de cross-compilation

Dans cette partie, nous allons voir comment recompiler *from scratch*
un système d'exploitation pour la RPI3 dans un conteneur Docker avec
le projet Buildroot.

### Préliminaires

Tout d'abord, téléchargez l'image Docker suivante:

````
$ docker rmi pblottiere/embsys-rpi3-buildroot
$ docker pull pblottiere/embsys-rpi3-buildroot
````

Ensuite, créez un conteneur à partir de cette image et listez les fichiers
contenus dans */root/*:

````
$ docker run -it pblottiere/embsys-rpi3-buildroot /bin/bash
# ls /root/
buildroot-precompiled-2017.08.tar.gz
````

### Découverte de Buildroot

La tarball `buildroot-precompiled-2017.08.tar.gz` est une version modifiée de
la version officielle de Buildroot disponible ici:
https://buildroot.org/downloads/buildroot-2017.08.tar.gz.

Vous pouvez considérer cette tarball comme un
[BSP](https://en.wikipedia.org/wiki/Board_support_package) construit
spécifiquement pour répondre aux besoins du TP.

Par rapport à la version officielle, il y a notamment en plus:
- un fichier de configuration `configs/embsys_defconfig` pour Buildroot
- un fichier de configuration pour busybox `busybox.config`
- un fichier décrivant les utilisateurs cibles `users.table`

De plus, toutes les dépendances de compilation sont déjà installées dans
l'image Docker et le système d'exploitation est précompilé (pour gagner du
temps).

Décompressez la tarball pour étudier son contenu et retrouver les fichiers
cités précédement:

````
# tar zxvf buildroot-precompiled-2017.08.tar.gz
# cd buildroot-precompiled-2017.08
````

**Question 1**: Décriver de manière plus précise l'utilité ainsi que la syntaxe
                de chacun des 3 fichiers mentionnés ci-dessus.
                
                
Embsys_defconfig
configuration des paramètres de Buildroot pour la compilation

Busybox.config :
Définitions des paramètres de busybox pour optimiser l’environnement d’outils et d’applications

Users.table :
Sometimes it is needed to add specific users in the target system. To cover this requirement, Buildroot provides support for so-called users tables



Par défaut, le projet Buildroot fournit des configurations pour certaines
cartes dans le répertoire *configs*.

**Question 2**: En considérant que la cible est une carte RaspberryPi3 avec un
                OS 32 bits, quel est le fichier de configuration Buildroot par
                défaut à utiliser?
                
raspberrypi3_defconfig

**Question 3**: Que contient le répertoire *package* et à quoi servent les
                sous-répertoires et fichiers associés?
                     
Package contient les règles de compilation. Et les sous répertoires sont propres aux différents langages de programmation


Désormais, lancez la commande suivante:

```
# make embsys_defconfig
```

**Question 4**: À quoi sert la commande précédente?

Lance une IHM utilisant Kconfig pour configurer le kernel

Maintenant, lancez la commande suivante pour afficher le menu de configuration:

````
# make menuconfig
````

**Question 5**: En naviguant dans le menu, repérez:
- l'architecture matérielle cible :
ARM (little endian)

- le CPU ciblé :
Architecture Variant (cortex-A53)

- l'ABI (en rappellant la signification de celle choisie) :
EABIhf (Embedded-ABI Hard Float)

- la librairie C utilisée :
uClibc-ng

- la version du cross-compilateur
GCC compiler Version (gcc 6.x)

- la version du kernel
Kernel version (Custom Git repository)  --->                         │ │  
│ │    (https://github.com/raspberrypi/linux.git) URL of custom repository        │ │  
│ │    (9126e25b0934bd7bd843763310ea4b34c6e139d0) Custom repository version   

Il est possible de rechercher une chaine de caractère avec la commande */*
(comme dans VIM).

**Question 6**: En recherchant dans l'interface de Buildroot, déterminez si le
                paquet *openssh* sera compilé et disponible dans l'OS cible. De
                même, retrouver cette information en analysant le fichier de
                configuration *embsys_defconfig*.
                
Nous trouvons BR2_PACKAGE_OPENSSH = y donc il sera compilé et disponible dans l’OS cible

**Question 7**: Qu'est ce que busybox? À quoi sert la commande
                *make busybox-menuconfig*? Qu'obtiens t'on et que pouvons
                nous faire?
                
                
Busybox est utilisé dans les équipements basés sur des distributions customisées pour définir l’environnement d’outils et d’applications fournis à travers le RFS

La configuration de Busybox utilise aussi KConfig et est disponible grâce à la commande make menuconfig

Cela nous permet de choisir les packages utilisés par busybox et sa configuration en général



Par défaut, le bootloader de la RPI3 est utilisé. D'ailleurs, vous pouvez
constater en allant dans le menu *Bootloaders* de l'interface de
configuration qu'aucun bootloader n'est actuellement sélectionné. Nous
verrons dans la partie suivante comment utiliser U-Boot à la place.

### Chaîne de cross-compilation

Une fois correctement configuré, il suffit de lancer la compilation avec la
commande *make*. Le résultat de la compilation est alors une image du kernel
ainsi que le bootloader et un RFS (notamment).

Cependant, l'étape de configuration précise et de compilation peut être longue
(plusieurs heures). C'est pour cette raison que l'OS est précompilé dans
l'image Docker que nous utilisons.

**Question 8**: Que contient le répertoire *output/host*? À quoi correspond
                le binaire *output/host/usr/bin/arm-linux-gcc*?
                
You may want to compile, for your target, your own programs or other software that are not packaged in Buildroot. In order to do this you can use the toolchain that was generated by Buildroot.
The toolchain generated by Buildroot is located by default in output/host/
Donc dans ce répertoire nous avons les configurations de la toolchain

output/host/usr/bin/arm-linux-gcc déclare les options liées au cross-compilateur



Sur le conteneur Docker, créez un fichier *helloworld.c*:

```` c
#include <stdio.h>

int main()
{
  printf("Hello World!\n");
}
````

Ensuite, nous pouvons compiler et utiliser la commande *file* pour observer
l'architecture cible du binaire généré:

```` shell
# gcc helloworld.c -o hw
# file hw
hw: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, not stripped
````

**Question 9**: Décrire le résultat de la commande *file*. Que se passe t-il
                si vous exécutez la commande *./hw*?
                
Permet d'identifier le type de fichier grâce à son MIME en utilisant les tests magic sur le contenu des fichiers (en général les premiers bits). Les tests sont fournis par le paquet libmagic1.

Nous avons le type de fichier, la taille, sa façon d’être executé, son interprêteur et la version du kernel visées

Avec ./hw nous exectutons le programme

Cette fois, lancez la commande suivante à partir du répertoire contenant
Buildroot:

````
# ./output/host/usr/bin/arm-linux-gcc helloworld.c -o hw
````

**Question 10**: Utilisez la commande *file* sur le binaire résultant.
                 Quelle différences constatez vous par rapport au cas précédent
                 (binaire généré avec gcc)? Que se passe t-il si vous essayez
                 d'exécuter la commande *./hw*? Expliquez pourquoi.
                 
hw: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped

Nous avons une architecture matérielle et une ABI spécifique car c’est ce que nous avons défini dans Buildroots


### Images

**Question 11**: Que contient le répertoire *output/images*? Décrivez notamment
                 l'utilité des fichiers *rootfs.tar*, *zImage* et *sdcard.img*.
                 
Il contient l’image du kernel linux

**Question 12**: Que vous dis les résultats de la commande *file* lorsque vous
                 l'utilisez sur les fichiers *zImage* et *sdcard.img*?
                 
zImage: Linux kernel ARM boot executable zImage (little-endian)
sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors

Ensuite, lancez les commandes suivantes:

````
# mkdir /tmp/rootfs
# tar -xf output/images/rootfs.tar -C /tmp/rootfs
````

**Question 13**: Que contient le répertoire */tmp/rootfs*?

Rien, malheureusement

### Compilation : À ne pas faire pendant le TP (trop long)

Si vous souhaitez compiler vous même les images, vous pouvez repartir de
l'image Docker précédente et lancer la commande *make*:

```` shell
$ docker run -it pblottiere/embsys-rpi3-buildroot /bin/bash
# cd /root
# tar zxvf buildroot-2017.08-precompiled.tar.gz
# cd buildroot-2017.08-precompiled
# make embsys_defconfig
# make
````

Si vous avez des problèmes de proxy pendant la compilation:

#### Sur la machine hôte

Si jamais il y a un problème de proxy sur la machine hôte, on peut configurer
docker pour modifier ses accès
(https://stackoverflow.com/questions/26550360/docker-ubuntu-behind-proxy).

Tout d'abord, créer le fichier de configuration:

```` shell
$ mkdir /etc/systemd/system/docker.service.d
$ touch /etc/systemd/system/docker.service.d/http-proxy.conf
````

Ensuite, ajouter les informations concernant le proxy. Par exemple:

```
[Service]
Environment="HTTP_PROXY=http://192.168.1.10:3128/"
Environment="HTTPS_PROXY=https://192.168.1.10:3128/"
Environment="SOCKS_PROXY=socks://192.168.1.10:822/"
Environment="NO_PROXY=localhost,127.0.0.0/8,ensieta.ecole,ensieta.fr,ensta-bretagne.fr"
```

Finalement, relancer docker:

``` shell
$ systemctl daemon-reload
$ systemctl restart docker
```

Des `export` peuvent aussi être ajoutés au `.bashrc`:

```` shell
$ export http_proxy=http://192.168.1.10:3128
$ export ftp_proxy=ftp://192.168.1.10:3128
$ export https_proxy=https://192.168.1.10:3128
$ export socks_proxy=socks://192.168.1.10:822
````

Dans ce dernier cas, la configuration du navigateur de la machine hôte doit
être modifié pour prendre en compte le proxy.


#### Sur le conteneur

Si une fois connecté dans un conteneur vous avez des problèmes de connexion
avec apt, il faut aussi penser à configurer le proxy.

Pour apt:

``` shell
$ cd /etc/apt
$ echo 'Acquire::http::Proxy "http://192.168.1.10:3128";
Acquire::ftp::Proxy "ftp://192.168.1.10:3128";
Acquire::https::Proxy "https://192.168.1.10:3128";
Acquire::socks::Proxy "socks://192.168.1.10:822";' > apt.conf
$ apt update
```

Avec wget, modifier le fichier `/etc/wgetrc` tel que:

```` shell
# You can set the default proxies for Wget to use for http, https, and ftp.
# They will override the value in the environment.
https_proxy = https://192.168.1.10:3128
http_proxy = http://192.168.1.10:3128
ftp_proxy = http://192.168.1.10:3128

# If you do not want to use proxy at all, set this to off.
use_proxy = on
````

Avec git:

```` shell
$ git config --global http.proxy http://192.168.1.10:3128
````
