# TP 4
### Question 1 :
<ul>
<li>Le fichier `configs/embys_defconfig` permet de savoir quel option de compilation, extension kernel, packages, tout ce qu'il y a sur un OS de base est fixé ou pas pour pouvoir reproduire la construction.<br>
Sa syntaxe est la suivante :<br>
`# NOM_FICHER = information dessus ou emplacement`<br>
et s'il n'y est pas fixé, il sera comme cela :<br>
`# NOM_FICHER is not set`</li>

<li>Le fichier `busybox.config` à la même utilité que embsys_defconfig, mais pour busybox. Sa syntax est la même pour le fichier embsys_defconfig.</li>

<li>Le fichier `users.table` permet d'ajouter au système un ou plusieurs utilisateurs.<br>
Sa syntax est la suivante:<br>
username uid group gid password home shell groups comment<br>
Je vous renvoie sur ce [lien](http://buildroot.org/downloads/manual/manual.html#makeuser-syntax) pour savoir à quoi correspond chaque mot-clé.</li>
</ul>

### Question 2 :
<p> il faudrait utiliser le fichier `configs/raspberrypi3_64_defconfig`</p>

### Question 3 :
<p> Dans le repertoire package, il y a les différent package pour python, php, et autres. Les sous-répertoir et fichiers associés servent à installer et faire fonctionner correctement le package en question pour l'os</p>

### Question 4 :
<p>Cette commande à permi de reconstruire un os avec le fichier embsys.</p> 

### Question 5 :
<ul>
	<li> L'architecture cible est ARM (little endian)</li>
	<li> Le CPU est cortex-A53</li>
	<li> L'ABI est EABIhf qui correspond à Extended Application Binary InterfaceHard FLoat</li>
	<li> La librairie C utilisé est uClibc-ng </li>
	<li>La version du cross-compilateur est gcc 6.x</li>
	<li> La version du kernel est `Custom Git repository`, voici son [url](https://github.com/raspberrypi/linux.git)</li>
</ul>

### Question 6 :
Avec l'interface de Buildroot, je l'ai retouvé et il sera compilé et disponible pour l'os. Et dans le fichier, à la ligne 2283, on peut retrouvé ceci `BR2_PACKAGE_OPENSSH=y` diant qu'il y serait.

### Question 7 :
Busybox est un programme libre qui permet de combiné plusieurs utilitaires UNIX dans un petit fichier exécutable.<br>
La commande `make busybox-menuconfig` permet d'installer le menuconfi de busybox.<br>
On obtient un menu de configuration où l'on peut modifier dives paramètres comme les parmètre de busybox, ou encore le login/password.

### Question 8 :
Le repertoir `outpu/host` contient des dossiers ce nommant : `arm-buildroot-linux-uclibcgnueabihf`, `bin`, `doc`, `etc`, `include`, `lib`, `libexec`, `man`, `sbin`, `share`, et `urs`.<br>
De mon point de vue, le fichier `arm-linux-gcc` sert pour la compilation avec gcc.

### Question 9 :
La commande `file` nous permet d'avoir plus d'information sur le binaire `hw`, ceci inclu son format x86-64, sa version, s'il est dynamiquement lié, sont interprétation, par quel Kernel/OS et la version, et d'autre paramètre que je n'ai pas compris.<br>
La commande `./hw` va permettre d'exécuté le programme C compilé, ce qui affiche le hello word.

### Question 10 :
On constacte qu'il y a des valeurs qui ont changé, qui se sont ajouté et supprimer, comme le `x86-64` a été remplacer par `ARM`, ou pour la version, il est précisé EABI5.<br>
Je ne peux pas exécuté le binaire, voici l'erreur : `bash: ./hw: cannot execute binary file: Exec format error`

### Question 11 :
Il y a différent fichier `.dbt`, `vfat`, `ext2`, `ext4` avec `rpi-firmware`, `sdcard.img`, `zImage`.
<ul>
	<li> rootfs.tar  est une image appelée INITial RamDisk (également appelée initrd) qui contient tout ce dont le noyau aura besoin pour s’amorcer dans un état où le système de fichiers racine peut être monté.</li>
	<li> zImage est l'image binaire réelle du noyau compilé. C’est ce que le chargeur de démarrage va charger et tenter d’exécuter.</li>
	<li>sdcard.img est le conteneur de l'image où le système a été généré.  </li>
</ul>

### Question 12 :
Pour zImage, voici ce que sa me donne : `zImage: Linux kernel ARM boot executable zImage (little-endian)`<br>
Et pour sdcard.img voilà ce que sa me donne : `sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors`<br>

### Question 13 :
J'ai des dossiers de système, comme pour la racine de GNU/Linux.
