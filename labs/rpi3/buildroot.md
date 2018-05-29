# Partie 1: Construction du système d'exploitation avec Buildroot et tests avec QEMU

Dans cette partie, nous allons voir comment recompiler *from scratch*
un système d'exploitation pour la RPI3.

### Préliminaires

Plus tard dans le TP, nous aurons besoin d'une image Docker spécifique. Le
téléchargement étant un peu long, ouvrez un terminal, lancez la commande
suivante et passez à la suite du TP:

```` shell
$ docker pull pblottiere/dominus:bulidroot-rpi3
````

### Découverte de Buildroot

Dans un premier temps, téléchargez et décompressez la tarball de buildroot:

```` shell
$ wget https://buildroot.org/downloads/buildroot-2017.08.tar.gz
$ tar zxvf buildroot-2017.08.tar.gz
````

Par défaut, le projet Bulidroot fournit des configurations pour certaines
cartes (le terme générique sous-jacent est BSP, soit Board Support Package)
dans le répertoire *configs*.

**Question 1**: En considérant que la cible est une carte RaspberryPi3 avec un
                 OS 32 bits, quel fichier de configuration doit-on utiliser?

**Question 2**: Que contient le répertoire *package*?

**Question 3**: Décrivez l'utilité des différents ficiers du répertoire
                *package/openssh*.

**Question 4**: À quoi servent les fichiers du répertoire
                *boards/raspberrypi3*?

Pour sélectionner un paquet à cross-compiler (et donc retrouver les outils
fournit par le paquet en question sur notre RFS), il suffit de se rendre dans
le menu *Target packages*, de chercher le paquet désiré dans les sous-menu (à
l'aide de la commande *\*, comme dans VIM), puis de le sélectionner.

**Question 5**: Dans l'interface de configuration, sélectionnez le paquet
                *openssh* (au passage, à quoi sert ce paquet?). Ensuite,
                sauvegardez la configuration et éditez le fichier *.config*.
                Qu'observez vous lorsque vous recherchez la chaîne de
                caractère *OPENSSH*? Que cela signifie t-il?

Désormais, lancez la commande suivante:

```` shell
$ make raspberrypi3_defconfig
````

**Question 5**: À quoi sert la commande précédente?

Ensuite, lancez la commande suivante pour afficher le menu de configuration:

```` shell
$ make menuconfig
````

**Question 6**: En naviguant dans le menu, repérez:
- l'architecture matérielle cible
- le CPU ciblé
- l'ABI (en rappellant la signification de celle choisie)
- la librairie C utilisée
- la version du cross-compilateur
- la version du kernel

**Question 7**: Qu'est ce que busybox? À quoi sert la commande
                *make busybox-menuconfig*? Qu'obtiens t'on et que pouvons
                nous faire?

Par défaut, le bootloader de la RPI3 est utilisé. D'ailleurs, vous pouvez
constater en allant dans le menu *Bootloaders* de l'interface de
configuration qu'aucun bootloader n'est actuellement sélectionné. Nous
verrons dans la partie suivante comment utiliser u-boot à la place.

### Système d'exploitation compilé

Une fois correctement configuré, il suffit de lancer la compilation avec la
commande *make*. Le résultat de la compilation est alors une image du kernel
ainsi que le bootloader et un RFS (notamment).

Cependant, l'étape de configuration précise et de compilation peut être longue
(plusieurs heures). Dans le cadre de ce TP, nous allons donc partir d'une image
Docker contenant un environnement Buildroot préparé avec un résultat précompilé
(au lieu de lancer la compilation complète).

Pour cela, nous allons utiliser l'image Docker téléchargée en début de TP:

```` shell
$ docker run -it pblottiere/dominus:buildroot-rpi3 /bin/bash
root@xxxxxxxxxxxx:/# cd /root
root@xxxxxxxxxxxx:/# tar zxvf buildroot-2017.08-compiled.tar.gz
````

**Question 7**: Que contient le répertoire *output/host*? À quoi correspond
                le binaire *output/host/usr/bin/arm-linux-gcc*?

Sur le conteneur Docker, créez un fichier *helloworld.c*:

```` shell
#include <stdio.h>

int main()
{
  printf("Hello World!\n");
}
````

Ensuite, nous pouvons compiler et utiliser la commande *file* pour observer
l'architecture cible du binaire généré:

```` shell
$ gcc helloworld.c -o hw
$ file helloworld
hw: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, not stripped
````

**Question 8**: Lancez la commande *./output/host/usr/bin/arm-linux-gcc helloworld.c -o hw*
                puis utilisez la commande *file* sur le binaire résultant.
                Quelle différences constatez vous par rapport au cas précédent
                (binaire généré avec gcc)? Que se passe t-il si vous essayez de
                l'exécuter?

**Question 9**: Que contient de répertoire *output/images*? Décrivez notamment
                l'utilité des fichiers *rootfs.tar*, *zImage* et *sdcard.img*.

TODO: rajouter fichier uboot

**Question 10**: Que vous dis les résultats de la commande *file* lorsque vous
                 l'utilisez sur les fichiers *zImage* et *sdcard.img*?

Ensuite, lancez les commandes suivantes:

```` shell
root@xxxxxxxxxxxx:/# mkdir /tmp/rootfs
root@xxxxxxxxxxxx:/# tar -xf output/images/rootfs.tar -C /tmp/rootfs
````

**Question 11**: Que contient le répertoire */tmp/rootfs*?

Maintenant que nous avons le système d'exploitation de compilé avec Buildroot,
nous allons rapidement voir comment émuler une RPI3 avec le RFS construis ici.

### Compilation : À ne pas faire pendant le TP (trop long)

Si vous souhaitez compiler vous même les images, vous pouvez repartir de
l'image Docker précédente et lancer la commande *make*:

```` shell
$ docker run -it pblottiere/dominus:buildroot-rpi3
root@xxxxxxxxxxxx:/# cd /root
root@xxxxxxxxxxxx:/# tar zxvf buildroot-2017.08-compiled.tar.gz
root@xxxxxxxxxxxx:/# cd buildroot-2017.08-compiled
root@xxxxxxxxxxxx:/# make
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
