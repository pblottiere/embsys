# Partie 1: Construction d'OS avec Yocto et chaine de cross-compilation

Dans cette partie, nous allons voir comment recompiler *from scratch*
un système d'exploitation pour la RPI3 dans un conteneur Docker avec
le projet Yocto.


Links:
- https://www.yoctoproject.org/docs/1.7/yocto-project-qs/yocto-project-qs.html
- http://www.linuxembedded.fr/2015/12/yocto-comprendre-bitbake/
- https://www.smile.eu/sites/default/files/2018-02/MiniBook_Yocto_VF.pdf

« Build Appliance BA 1.7 » (Dizzy) : VM de build cross-plateforme

GUI: Hob / Toaster

Large support matériel: x86, x86-64, ARM, MIPS, and PPC-based, ...

UI Yocto de ref pour système embarqué: Sato (Gnome Mobile-Based UI)

Image résultante facile à tester dans Quick EMUlator (QEMU)

Yocto:
- layers
- recette
- distro

BitBake + certains composant d'OE => Poky

BitBake:
- Outil de build d'image Linux provenant de Open Embedded intialement
- Brique fondamentale de Yocto

http://git.yoctoproject.org/cgit/cgit.cgi/meta-raspberrypi

http://framboisepi.fr/construction-dune-image-raspberry-pi-avec-le-bsp-yocto-v2/

## BUILD

https://medium.com/smileinnovation/introduction-%C3%A0-yocto-db56a550ae51

prendre branches thud (https://wiki.yoctoproject.org/wiki/Stable_branch_maintenance)

The variable ${TOPDIR} represents the build directory by default.
debug-tweaks will enable root account without password. ssh-server-openssh will install an ssh server using openssh. And the ssh server will be started automatically during boot-up.
