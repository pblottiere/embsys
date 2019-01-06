# Partie 2: QEMU

Dans cette partie, nous allons voir comment se servir de chroot et QEMU pour
naviguer dans le RFS compilé par Buildroot pour la RPI3.

### Préliminaires

Tout d'abord, la machine hôte doit posséder le paquet suivant:

````
$ apt-get install binfmt-support
````

Sinon, ce TP est à réaliser à partir de la même image Docker que la
[Partie 1](buildroot.md):

````
$ docker run -it --privileged pblottiere/embsys-rpi3-buildroot /bin/bash
# cd /root/
# tar zxvf buildroot-precompiled-2017.08.tar.gz
# cd buildroot-precompiled-2017.08
````

**Question 1**: À quoi sert l'option *--priviliged* lors du lancement d'un
                nouveau coneneur Docker?

### QEMU et chroot

Tout d'abord, il faut installer les paquets nécessaires pour utiliser QEMU dans
le conteneur Docker:

````
# apt-get update
# apt-get install binfmt-support qemu-user-static
````

Ensuite créez un simple fichier C et compilez le avec le cross-compilateur
fourni par Buildroot (cf [Partie 1](buildroot.md)):

````
# printf '#include <stdio.h>\nint main(){ printf("Hello Worlds!"); }\n' > hw.c
# ./output/host/usr/bin/arm-linux-gcc hw.c -o hw
````

Puis, pour émuler le RFS d'une carte ARM avec QEMU:

````
# mkdir -p /tmp/rootfs
# tar -xf output/images/rootfs.tar -C /tmp/rootfs
# cp hw /tmp/rootfs/
# cd /tmp/rootfs
# mount --bind /dev dev/
# mount --bind /proc proc/
# cp /usr/bin/qemu-arm-static usr/bin/
# update-binfmts --enable qemu-arm
# chroot . bin/busybox ash
root@hostname:  $
````

**Question 2**: À quoi sert la commande *chroot*?

Ensuite, exécutez le binaire cross-compilé *hw* dans l'environnement *chroot*.

**Question 3**: Que se passe-t-il? Pourquoi?

Finalement, sortir de l'environnement du chroot (Ctrl-D) et démonter les
volumes:

````
$ umount /tmp/rootfs/dev
$ umount /tmp/rootfs/proc
````
