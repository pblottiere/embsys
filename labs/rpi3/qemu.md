# Partie 2: QEMU

Dans cette partie, nous allons voir comment se servir de chroot et QEMU pour
naviguer dans le RFS compilé par Buildroot pour la RPI3.

### Préliminaires

Ce TP est à réaliser à partir de la même image Docker que la
[Partie 1](buildroot.md):


````
$ docker run -it embsys:rpi3-buildroot /bin/bash
# cd /root/
# tar zxvf buildroot-2017.08.tar.gz
# cd buildroot-2017.08
````

### QEMU et chroot

Tout d'abord, il faut installer les paquets nécessaires pour utiliser QMEU dans
le conteneur Docker:

````
# apt-get install binfmt-support qemu-user-static
````

Émulation de carte arm avec QEMU:

````
$ sudo mount /dev/sdX2 /media/sd
$ cd /media/sd
$ mount --bind /dev dev/
$ mount --bind /proc proc/
$ sudo cp /usr/bin/qemu-arm-static usr/bin/
$ sudo chroot . bin/busybox ash
root@hostname:  $
````

Pour démonter:

````
$ sudo umount /media/sd/dev
$ sudo umount /media/sd/proc
$ sudo umount /media/sd/
````
