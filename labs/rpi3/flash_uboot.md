# Partie 3: Flashage de la carte et Bootloader

Dans cette partie, nous allons voir comment flasher l'OS sur la carte ainsi
qu'étudier le bootloader U-Boot.

### Préliminaires

Ce TP est à réaliser à partir de la même image Docker que la
[Partie 1](buildroot.md):


````
$ docker rmi pblottiere/embsys-rpi3-buildroot
$ docker pull pblottiere/embsys-rpi3-buildroot
$ docker run -it pblottiere/embsys-rpi3-buildroot /bin/bash
# cd /root
# tar zxvf buildroot-precompiled-2017.08.tar.gz
````

### Flashage

Avec le résultat de la compilation de Buildroot, il y a 2 possibilités pour
flasher l'OS sur la carte SD cible:
- mode manuel: partitionnement de la carte, copie du bootloader, ...
- mode "automatique": 1 seule commande à exécuter, rien à comprendre

Le mode automatique est évidemment le plus simple. Cependant, si vous souhaitez
configurer finement le comportement de votre RPI3 (par exemple utiliser un
bootloader, spécifique), il est indispensable de comprendre le mode manuel.

De plus, le mode automatique n'est possible que grâce à l'écriture de scripts
reprenant les étapes du flashage manuel.

#### Automatique

Tout d'abord récupérer l'image complet de la carte SD du conteneur Docker sur
votre machine hôte:

```` shell
$ docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/sdcard.img .
````

Ensuite, sur une carte SD disponible à travers */dev/sdX* (remplacer *X* par le
path de votre carte. *dmesg* peut vous aider):

```` shell
$ sudo dd if=sdcard.img of=/dev/sdX
````

Insérer la carte SD dans la RPI3 et brancher les adaptateurs USB-TTL
sur les portd TX/RX
(cf [datasheet](https://components101.com/microcontrollers/raspberry-pi-3-pinout-features-datasheet) de la carte).
Utiliser un terminal série (minicom, gtkterm ou autre) pour établir une
communication avec la carte.

#### Manuel (juste pour information, à ne pas faire)

Pour flasher la carte manuellement, les étapes suivantes sont à réaliser dans
l'ordre à partir de la machine hôte:

1- Nettoyage
2- Partitionnement
3- Formattage
4- Copie du bootloader et de l'image kernel (entre autre) sur la 1ère partition
5- Extraction du RFS sur la 2ème partition

Tout d'abord, sur une carte SD disponible à travers */dev/sdX* (remplacer *X*
par le path de votre carte. *dmesg* peut vous aider), il faut nettoyer la
carte SD:

```` shell
$ dd if=/dev/zero of=/dev/sdX
````

Ensuite, informez vous sur l'utilisation de la commande *fdisk* afin de
partitionner la carte SD tel que:
- 1ère partition de 128M ( *+128M* ) avec flag de boot (commande *a*) et type W95 FAT32 (LBA) (commande *t* puis code *c*)
- 2ème partition avec le reste et la config par défaut

Puis, formattez les partitions:

```` shell
$ sudo mkfs.vfat /dev/sdX1
$ sudo mkfs.ext4 /dev/sdX2
````

En utilisant la commande *fdisk* et la commande *p* de son interpreteur pour
afficher la configuration de la carte, vous devriez alors obtenir ceci si vous
avez réalisé les étapes précédentes correctement:

```` shell
$ fdisk /dev/sdX
Command (m for help): p
Disk /dev/sdX: 14.9 GiB, 15931539456 bytes, 31116288 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000

Device     Boot Start    End Sectors Size Id Type
/dev/sdX1  *        1  65536   65536  32M  c W95 FAT32 (LBA)
/dev/sdX2       65537 188416  122880  60M 83 Linux

Command (m for help):
$ lsblk -fs
NAME  FSTYPE LABEL     UUID                                 MOUNTPOINT
sdX1  vfat   boot      7794-9F86
└─sdX
sdX2  ext4   root      7e0c7c79-a446-46bc-b103-aeebc167ca13
└─sdX
````

Intéressons-nous maintenant au RFS. Pour information, Buildroot ne génère
pas de tarball de rootfs par défaut. Il faut reconfigurer le build
(*make menuconfig*) et activer la construction d'un tar. Ensuite recompiler
avec *make* et récupérer la tarball *rootfs.tar* sur la machine hôte.

Préparer manuellement la carte SD (vous aurez besoin de récupérer en local
quelques fichiers générés par buildroot):

```` shell
$ mkdir /media/sd
$ mount /dev/sdX1 /media/sd
$ cp bcm2710-rpi-3-b.dtb /media/sd
$ cp bcm2710-rpi-cm3.dtb /media/sd
$ cp -r rpi-firmware/* /media/sd
$ cp zImage /media/sd
$ umount /media/sd
$
$ mount /dev/sdX2 /media/sd
$ tar xf rootfs.tar -C /media/sd
$ umount /media/sd
````

Démarrer la RPI3 et établir une communication série (via des outils comme
minicom, gtkterm, putty, cu, ...).

### U-Boot

Comme nous l'avons vu précédemment, la RPI3 boot et est parfaitement
fonctionnelle. Par contre, dans le monde industriel, il est très souvent
nécessaire d'avoir un bootloader permettant par exemple de charger des images
depuis le réseau ou bien apportant le support du Device Tree (on verra cette
notion plus tard).

L'objectif de cette partie est donc de voir comment compiler, installer et
utiliser [U-Boot](https://www.denx.de/wiki/DULG/Manual).

Tout d'aboord, afin de compiler U-Boot pour notre carte, il faut modifier
les options suivantes dans la configuration de Buildroot:
- Build System: KCONFIG
- Board defconfig: rpi_3_32b (regarder dans le répertoire configs du code uboot ds build pour trouver le bon nom à utiliser)
- Cocher *U-Boot needs dtc*
- *mkimage* pour *host*

Ensuite, il suffit de recompiler le tout avec *make*, mais nous n'allons pas
le faire durant ce TP, car c'est trop long. Nous allons partir d'un Buildroot
spécifique précompilé:

````
$ docker rmi pblottiere/embsys-rpi3-buildroot-uboot
$ docker pull pblottiere/embsys-rpi3-buildroot-uboot
$ docker run -it pblottiere/embsys-rpi3-buildroot-uboot /bin/bash
# cd /root
# tar zxvf buildroot-precompiled-2017.08.tar.gz
````

Ensuite créer un nouveau fichier *boot.source* contenant:

````
fatload mmc 0:1 ${kernel_addr_r} zImage
fatload mmc 0:1 ${fdt_addr_r} bcm2710-rpi-3-b.dtb
setenv bootargs earlyprintk dwc_otg.lpm_enable=0 console=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=noop noinitrd rw rootwait
bootz ${kernel_addr_r} - ${fdt_addr_r}
````

Et compiler ce fichier avec *mkimage*:

````
root@xxxxxxxxxxxx:/# ./output/host/bin/mkimage -A arm -O linux -T script -C none -a 0x00000000 -e 0x00000000 -n boot.scr -d boot.source  boot.scr
````

Ensuite copier le binaire *u-boot.bin*, le *fichier boot.scr* et le fichier
*config.txt* suivant dans la 1ère partition de la carte SD:

````
kernel=u-boot.bin
enable_uart=1
dtoverlay=pi3-disable-bt
````

Tester la configuration.
