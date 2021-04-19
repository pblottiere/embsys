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

Le fichier :
* de configuration `configs/embsys_defconfig` pour Buildroot permet de configurer les options du kernel (visibles dans le *menuconfig*). Le langage utilisé est *Kconfig*. Les options peuvent être activées (*set*), désactivées (*is not set*) ou traduit l'utilisation d'un module kernel (*=y* pour *yes* ou *=value* pour affecter *value* à cette option).
* de configuration pour busybox `busybox.config` : la configuration de Busybox utilise aussi KConfig et est
disponible grâce à la commande make menuconfig. Il fournit un environnement d'outils et d'applications de base (*cd*, *ls*, *cat*, etc.).
* décrivant les utilisateurs cibles `users.table` : définit les uilisateurs et les dossiers auxquels il a accès.

The syntax to create users is inspired by the makedev syntax, above, but is specific to Buildroot.

The syntax for adding a user is a space-separated list of fields, one user per line; the fields are:

````
username uid group gid password home shell groups comment
user -1 users_group -1 =user1* /home/user /bin/sh -
````

Where:
* username is the desired user name (aka login name) for the user. It can not be root, and must be unique. If set to -, then just a group will be created.
* uid is the desired UID for the user. It must be unique, and not 0. If set to -1, then a unique UID will be computed by Buildroot in the range [1000…1999]
* group is the desired name for the user’s main group. It can not be root. If the group does not exist, it will be created.
* gid is the desired GID for the user’s main group. It must be unique, and not 0. If set to -1, and the group does not already exist, then a unique GID will be computed by Buildroot in the range [1000..1999]
* password is the crypt(3)-encoded password. If prefixed with !, then login is disabled. If prefixed with =, then it is interpreted as clear-text, and will be crypt-encoded (using MD5). If prefixed with !=, then the password will be crypt-encoded (using MD5) and login will be disabled. If set to *, then login is not allowed. If set to -, then no password value will be set.
* home is the desired home directory for the user. If set to -, no home directory will be created, and the user’s home will be /. Explicitly setting home to / is not allowed.
* shell is the desired shell for the user. If set to -, then /bin/false is set as the user’s shell.
* groups is the comma-separated list of additional groups the user should be part of. If set to -, then the user will be a member of no additional group. Missing groups will be created with an arbitrary gid.
* comment (aka GECOS field) is an almost-free-form text. 

Par défaut, le projet Buildroot fournit des configurations pour certaines
cartes dans le répertoire *configs*.

**Question 2**: En considérant que la cible est une carte RaspberryPi3 avec un
                OS 32 bits, quel est le fichier de configuration Buildroot par
                défaut à utiliser?

Le fichier de configuration Buildroot à utiliser est : ```/configs/raspberrypi3_defconfig```.

**Question 3**: Que contient le répertoire *package* et à quoi servent les
                sous-répertoires et fichiers associés?

Le répertoire *package* contient l'ensemble des modules kernel installés dans le conteneur docker. Chaque sous-répertoire contient un fichier de configuration des options du package en langage Kconfig, et un Makefile qui permet de compiler le package. Il contient également des patchs qui traduisent les étapes de compilation, un fichier hash qui contient la clé de hashage des licenses.

Désormais, lancez la commande suivante:

```
# make embsys_defconfig
```

**Question 4**: À quoi sert la commande précédente?

La commande précédente permet de compiler les options de configuration de Buildroot pour la machine hôte pour le projet embsys.

Maintenant, lancez la commande suivante pour afficher le menu de configuration:

````
# make menuconfig
````

**Question 5**: En naviguant dans le menu, repérez:
- l'architecture matérielle cible : CPU
- le CPU ciblé : ARM (little endian)
- l'ABI (en rappellant la signification de celle choisie) : EABIhf (Embedded Application Binary Interface hard floating point model)
- la librairie C utilisée : uClibc
- la version du cross-compilateur : Buildroot 2017.08
- la version du kernel : 4.9.x

Il est possible de rechercher une chaine de caractère avec la commande */*
(comme dans VIM).

**Question 6**: En recherchant dans l'interface de Buildroot, déterminez si le
                paquet *openssh* sera compilé et disponible dans l'OS cible. De
                même, retrouver cette information en analysant le fichier de
                configuration *embsys_defconfig*.

Le package *openssh* sera complilé puisque dans le fichier *Config.in* l'option *BR2_PACKAGE_OPENSSL=y* est activée. On la retrouve bien dans le fichier de configuration *embsys_defconfig*

**Question 7**: Qu'est ce que busybox? À quoi sert la commande
                *make busybox-menuconfig*? Qu'obtiens t'on et que pouvons
                nous faire?

Busybox fournit un environnement d'outils et d'appplications à travers le RFS. La commande *make busybox-menuconfig* permet de compiler les options de configuration de busybox pour avoir l'IHM du menu de configuration de busybox. On peut donc modifier la configuration de busybox via l'IHM.

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

Le binaire correspond à la chaîne de cross-compilation pour l'architecture cible *arm-linux* utilisant le compilateur *gcc*.

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

La commande *file* donne des informations sur le binaire généré comme le type de fichier (système de fichier ELF + processeur 64 bits + type d'obejt, ici librairie partagée), l'architecture matérielle cible, l'interréteur nécessaire epour exécuter le binaire, le kernel cible, l'identifiant de compilation. En exécutant la commande *./hw*, on exécute le binaire sur notre machine hôte.

Cette fois, lancez la commande suivante à partir du répertoire contenant
Buildroot:

````
# ./output/host/usr/bin/arm-linux-gcc helloworld.c -o hw
````

**Question 10**: Utilisez la commande *file* sur le binaire résultant.
                 Quelle différences constatez vous par rapport au cas précédent
                 (binaire généré avec gcc)? Que se passe t-il si vous essayez
                 d'exécuter la commande *./hw*? Expliquez pourquoi.
````
hw: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped
````
On observe les différences suivantes :
* le type de fichier : système de fichier ELF, processeur 32 bits, binaire exécutable
* l'architecture logicielle ARM
* la FPU (Floating Point Unit) : EABI5
* l'interpréteur : /lib/ld-uClibc.so.0

### Images

**Question 11**: Que contient le répertoire *output/images*? Décrivez notamment
                 l'utilité des fichiers *rootfs.tar*, *zImage* et *sdcard.img*.

Ce dossier contient l'ensemble des fichiers issus de la cross compilation :
* Le fichier *rootfs.tar* définit l'architecture du système de fichiers pour la machine cible. 
* Le fichier *zImage* est le bootloader pour la machine cible : Linux kernel ARM boot executable zImage (little-endian). 
* Le fichier *sdcard.img* est l'image ISO de l'OS à flasher sur la machine cible, ici la RPi3.

**Question 12**: Que vous dis les résultats de la commande *file* lorsque vous
                 l'utilisez sur les fichiers *zImage* et *sdcard.img*?

````
# file zImage
zImage: Linux kernel ARM boot executable zImage (little-endian)
````

````
# file sdcard.img 
sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors
````
Cette commande donne les partitions qui seront créés dans l'OS, la première de 65536 octets, la deuxième de 409600 octets.

Ensuite, lancez les commandes suivantes:

````
# mkdir /tmp/rootfs
# tar -xf output/images/rootfs.tar -C /tmp/rootfs
````

**Question 13**: Que contient le répertoire */tmp/rootfs*?

Ce dossier contient donc le système de fichiers (architecture des répertoires) et les outils de base fournit par busybox (*ls*, *mv*, etc.) qui sera sur la machine cible, ici la RPi3.

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
