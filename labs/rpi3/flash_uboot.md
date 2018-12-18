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

**Question 1**: Une fois le flashage terminé, combien y-a t-il de partition
                sur la carte SD? Que contiennent-elles?

**Question 2**: Lire la
                [datasheet](https://components101.com/microcontrollers/raspberry-pi-3-pinout-features-datasheet)
                de la RPI3. Quels sont les ports TX/RX?

Ensuite, branchez l'adaptateur USB-TTL sur les ports TX/RX et ouvrez un
terminal série (gtkterm, minicom, ...). Finalement, connectez vous au réseau
avec un cable Ethernet, insérez la carte SD et démarrez la RPI3.

**Quesion 3**: Quelle est la configuration du port série permettant une
               communication avec la RPI3 (baud, etc)?

Puis, connectez vous en tant que *user* sur la RPI3 (lire le fichier
*users.tables* pour déterminer le mot de passe).

**Question 4**: Déterminez l'adresse IP de votre RPI3. Quelle commande
                avez-vous utilisé?

**Question 5**: Testez la connection ssh en tant que *user* et *root*. Quelle
                différence observez-vous? Pourquoi? Où est-ce configuré?

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

Cette fois, U-Boot est compilé et le binaire *u-boot.bin* résultant se
trouve dans *outout/build/images*. La première étape est donc de copier ce
binaire sur la 1ère partition de la carte SD.

Ensuite, il faut créer un fichier *boot.source* sur le conteneur Docker
contenant la configuration du bootloader:

````
mmc dev 0
fatload mmc 0:1 ${kernel_addr_r} zImage
fatload mmc 0:1 0x2000000 bcm2710-rpi-3-b.dtb
setenv bootargs 8250.nr_uarts=1 root=/dev/mmcblk0p2 rootwait console=ttyS0,115200
bootz ${kernel_addr_r} - 0x2000000
````

**Question 6**: En cherchant sur le net, trouvez l'utilité des commandes U-Boot
                *fatload*, *setenv* et *bootz*.

Puis compiler ce fichier avec *mkimage*:

````
# ./output/host/bin/mkimage -A arm -O linux -T script -C none -a 0x00000000 \
    -e 0x00000000 -n boot.scr -d boot.source boot.scr
````

Ensuite copiez le fichier *boot.scr* sur la 1ère partition de la carte SD aux
côtés du binaire de U-Boot.

La dernière étape consiste à modifier le fichier
[config.txt](https://www.raspberrypi.org/documentation/configuration/config-txt/)
pour indiquer au firmware de la RPI3 de charger U-Boot au lieu du kernel:

````
kernel=u-boot.bin
enable_uart=1
dtoverlay=pi3-disable-bt
````

C'est ensuite U-Boot qui se chargera de charger le kernel comme indiqué dans
le fichier *boot.source* (cf étape précédente).

Désormais, connectez vous avec un terminal série et démarrez la RPI3. Appuyez
sur n'importe quelle touche de votre clavier dans le terminal série pour
interrompre la phase de boot et ainsi ouvrir le prompt U-Boot:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/uboot_prompt.png" width="550" title="Github Logo">
</p>

Dans ce prompt, la commande *help* permet de lister les commandes disponibles.

**Question 7**: À quoi sert la commande *version*? Que retourne t-elle comme
                informations?

Pour reprendre la phase normale de boot et démarrez le kernel, lancez la
commande *boot*.

### U-Boot et server TFTP

L'objectif de cette partie est de charger l'image du kernel *zImage* non pas à
partir de la carte SD comme dans la partie précédente, mais grâce à un serveur
TFTP.

**Question 8**: Trouvez une documentation en ligne afin de configurer un serveur
                TFTP sur votre machine hôte. Note: il faut que la machine hôte
                et la RPI3 soit dans le même sous-réseau (autrement dit, il faut
                qu'elles puissent se pinger).

Pour tester le bon fonctionnement du serveur, lancez la commande suivante à
partir de la RPI3 pour récupérer le fichier *zImage*:

```
# tftp <tftp_server_ip> -r zImage -g
```

Si le fichier *zImage* est bien récupéré, cela signifie que le serveur TFTP est
correctement configuré.

Ensuite, modifiez le fichier *boot.source* pour obtenir:

```
mmc dev 0
fatload mmc 0:1 0x2000000 bcm2710-rpi-3-b.dtb
setenv autoload no
dhcp
tftp ${kernel_addr_r} ${serverip_tftp}:zImage
setenv bootargs 8250.nr_uarts=1 root=/dev/mmcblk0p2 rootwait console=ttyS0,115200
bootz ${kernel_addr_r} - 0x2000000
```

**Question 8**: À quoi servent les comandes *dhcp* et *tftp*?

Puis recompilez ce fichier avec *mkimage* comme précédemment. Pensez ensuite à
copier le fichier *boot.scr* résulant sur la 1ère partition de la RPI3.

Finalement, redémarrez la RPI3 et intérrompez la phase de boot pour arriver
dans le prompt de U-Boot. Sauvegardez l'addresse IP de votre serveur TFTP:

```
U-Boot> setenv serverip_tftp <tftp_server_ip>
U-Boot> saveenv
```

**Question 9**: Décrivez les commandes précédente.

Finalement, redémarrez une dernière fois la RPI3 et observez le chargement du
kernel:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/uboot_tftp_kernel.png" width="550" title="Github Logo">
</p>
