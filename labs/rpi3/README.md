# RPI3

Notation:

```` shell
$ ls # commande sur la machine hôte
root@a11a05dcac96:/# ls # commande sur le conteneur Docker
````

## Docker

Si jamais il y a un problème de proxy, on peut configurer docker pour modifier ses accès (https://stackoverflow.com/questions/26550360/docker-ubuntu-behind-proxy).
Pour les machines de TP, les étapes à suivre sont les suivantes:
- création du fichier config
```` shell
$ mkdir /etc/systemd/system/docker.service.d
$ cat /etc/systemd/system/docker.service.d/http-proxy.conf
````

- ajouter les informations suivantes:
```
[Service]
Environment="HTTP_PROXY=http://192.168.1.10:3128/"
Environment="HTTPS_PROXY=https://192.168.1.10:3128/"
Environment="SOCKS_PROXY=socks://192.168.1.10:822/"
Environment="NO_PROXY=localhost,127.0.0.0/8,ensieta.ecole,ensieta.fr,ensta-bretagne.fr"
```



Récupération d'une image Debian stable vierge en local:

```` shell
$ docker pull debian:stable
$ docker images
REPOSITORY                  TAG             IMAGE ID        CREATED         SIZE
debian                      stable          49e46d4f55fe    4 weeks ago     100M
$ docker run -it debian:stable /bin/bash
````

Démarrage d'un nouveau conteneur en mode shell à partir de l'image Debian:

```` shell
$ docker run -it debian:stable /bin/bash
root@a11a05dcac96:/#
````

Connexion à un conteneur existant à partir de son ID:

```` shell
$ docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
a11a05dcac96   debian:stable   "/bin/bash"   About 1 hour ago   Up 2 seconds          dazzling_colden
$ docker exec -it a11a05dcac96 /bin/bash
````

Arrêt d'un conteneur existant et actif:

```` shell
$ docker stop a11a05dcac96
$ docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
$ docker ps -a
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
a11a05dcac96   debian:stable   "/bin/bash"   About 1 hour ago   Up 2 seconds          dazzling_colden
````

Démarrage d'un conteneur existant arrêté:

```` shell
$ docker container start a11a05dcac96
$ docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
a11a05dcac96   debian:stable   "/bin/bash"   About 1 hour ago   Up 2 seconds          dazzling_colden
````

Récupération d'un fichier présent sur un conteneur:

```` shell
$ docker cp a11a05dcac96:/file/path/within/container /host/path/target
````

## Buildroot

Compilation des images pour RPI3 dans un conteneur Docker à partir d'une image
Debian stable.

### Compilation

```` shell
root@a11a05dcac96:/# cd /root
root@a11a05dcac96:/# wget https://buildroot.org/downloads/buildroot-2017.08.tar.gz
root@a11a05dcac96:/# tar zxvf buildroot-2017.08.tar.gz
root@a11a05dcac96:/# make raspberrypi3_defconfig
root@a11a05dcac96:/# make menuconfig
````

Dans le menu de configuration, indiquer le nombre de jobs à 4 pour la
compilation puis sauvegarder la configuration. Ensuite, lancer la compilation
(vous devrez probablement installer des dépendances pour arriver au bout de la
phase de compilation)...

```` shell
root@a11a05dcac96:/# make
````

NOTE: ceci peut vous aider un passer un *git clone* particulièrement lourd

```` shell
root@a11a05dcac96:/# git config --global http.postBuffer 1048576000
````

### Flashage

#### Automatique

En mode automatique sur une carte SD */dev/sdX* (remplacer *X* par le path de
votre carte. *dmesg* peut vous aider):

```` shell
$ docker cp a11a05dcac96:/root/buildroot-2017.08/output/images/sdcard.img /home/user/
$ sudo dd if=sdcard.img of=/dev/sdX
````

Insérer la carte SD dans la RPI3 et brancher les adaptateurs USB-TTL
correctement (cf datasheet de la carte). Utiliser un terminal série (minicom ou
autre) pour établir une communication avec la carte.

#### Manuel

Paritionner manuellement la carte SD avec *fdisk* tel que:
- 1ère partition de 128M ( *+128M* ) avec flag de boot (commande *a*) et type W95 FAT32 (LBA) (commande *t* puis code *c*)
- 2ème partition avec le reste et la config par défaut

Formatter les partitions:

```` shell
$ sudo mkfs.vfat /dev/sdX1
$ sudo mkfs.ext4 /dev/sdX2
````

Le résultat des étapes précédentes:

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
└─sdc
````

Buildroot ne génère pas de tarball de rootfs par défaut. Il faut reconfigurer
le build (*make menuconfig*) et activer la construction d'un tar. Ensuite
recompiler avec *make* et récupérer la tarball *rootfs.tar* sur la machine
hôte.

Préparer manuellement la carte SD (vous aurez besoin de récupérer en local
quelques fichiers générés par buildroot):

```` shell
$ mkdir /media/sd
$ mount /dev/sdX1 /media/sd
$ cp bcm2710-rpi-3-b.dtb /media/sd
$ cp bcm2710-rpi-cm3.dtb /media/sd
$ cp rpi-firmware/* /media/sd
$ cp zImage /media/sd
$ umount /media/sd
$
$ mount /dev/sdX2 /media/sd
$ tar xf rootfs.tar -C /media/sd
$ umount /media/sd
````

Démarrer la RPI3 et établir une communication série.

### Customisation

Configurer Buildroot (*make menuconfig*) pour:
- modifier le hostname
- ajouter un mot de passe root
- ajouter un utilisateur (et un mot de passe associé)
- activer SSH
- installer ntp/ntpdate
- installer domoticz

### Dockerfile

Se renseigner sur la notion de Dockerfile et en écrire un (automatisation!).

### QEMU et chroot

Émulation de carte arm avec QEMU:

````
$ sudo apt-get install binfmt-support qemu-user-static
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

### Uboot

Bootloader industriel. Configurer dans Buildroot les options du bootloader:
- Build System: KCONFIG
- Board defconfig: rpi_3_32b (regarder dans le répertoire configs du code uboot ds build)
- cocher *U-Boot needs dtc*
- mkimage pour host

Recompiler le tout avec *make*.

Ensuite créer un nouveau fichier *boot.source* contenant:

````
fatload mmc 0:1 ${kernel_addr_r} zImage
fatload mmc 0:1 ${fdt_addr_r} bcm2710-rpi-3-b.dtb
setenv bootargs earlyprintk dwc_otg.lpm_enable=0 console=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=noop noinitrd rw rootwait
bootz ${kernel_addr_r} - ${fdt_addr_r}
````

Et compiler ce fichier avec *mkimage*:

````
root@a11a05dcac96:/# ./output/host/bin/mkimage -A arm -O linux -T script -C none -a 0x00000000 -e 0x00000000 -n boot.scr -d boot.source  boot.scr
````

Ensuite copier le binaire *u-boot.bin*, le *fichier boot.scr* et le fichier
*config.txt* suivant dans la 1ère partition de la carte SD:

````
kernel=u-boot.bin
enable_uart=1
dtoverlay=pi3-disable-bt
````
