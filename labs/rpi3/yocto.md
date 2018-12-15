# Partie 3 : Construction du système d'exploitation avec Yocto

Dans cette partie, nous allons voir comment recompiler *from scratch*
un système d'exploitation pour la RPI3 dans un conteneur Docker avec
le projet Yocto.

### Préliminaires

Tout d'abord, téléchargez l'image Docker suivante:

````
$ docker rmi pblottiere/embsys-rpi3-yocto
$ docker pull pblottiere/embsys-rpi3-yocto
````

Ensuite, créez un conteneur à partir de cette image et listez les fichiers
contenus dans */root/*:

````
$ docker run -it pblottiere/embsys-rpi3-yocto /bin/bash
# ls /root/
poky-thud-precompiled.tar.gz
````

### Découverte de Yocto

La tarball `poky-thud-precompiled.tar.gz` est la version précompilée de la
version `thud` de la distribution de référence de Yocto: `Poky`? Vous pouvez
voir la roadmap des releases ici: https://wiki.yoctoproject.org/wiki/Releases.

Décompresser la tarball:

```
# tar zxvf poky-thud-precompiled.tar.gz
# cd poky-thud-precompiled
```

**Question 1**: En étudiant le contenu du fichier *build/conf/bblayers.conf*,
                déterminez quelle layer ajoute le support pour la RasperryPi.

**Question 2**: Où pouvons-nous retrouver le répertoire associée à cette layer?

**Question 3**: En étudiant le contenu du fichier *build/conf/local.conf*,
                indiquez à travers quelle variable on définit le type de carte
                pour laquelle la distribution Poky doit être compilée.

Ici, la compilation a déjà été réalisée pour gagner du temps pendant le TP, et
ce grâce aux commandes suivantes:

````
# bitbake core-image-minimal
# bitbake meta-toolchain
````

**Question 4**: En cherchant dans la documentation de Yocto, indiquez à quoi
                servent ces 2 commandes.

### Chaîne de cross-compilation

Le résultat de la compilation se trouve dans le répertoire *tmp* (il a été
en partis nettoyé pour diminuer la taille de l'image Docker).

Contrairement à Buildroot, la chaîne de cross-compilation n'est pas disponible
à travers le répertoire résultant de la compilation, mais doit être installée
manuellement dans un répertoire spécifique. Pour cela, réaliser l'opération
suivante afin d'installer le chaine de cross-compilation dans le répertoire
*/opt/poky/2.6*:

````
# apt-get update
# apt-get install gcc xz-utils python
# sh build/tmp/deploy/sdk/poky-glibc-x86_64-meta-toolchain-cortexa7t2hf-neon-vfpv4-toolchain-2.6.sh
````

Ensuite, il faut faire un `source` d'un script particulier pour avoir accès
aux divers outils de la toolchain (en réalité, les variables d'envrionnement
du système sont modifiées, comme par exemple $PATH):

````
# source /opt/poky/2.6/environment-setup-cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi
# echo $PATH
````

Une fois cette étape réalisée, vous avez notamment accès au cross-compilateur:

````
# arm-poky-linux-gnueabi-gcc --version
arm-poky-linux-gnueabi-gcc (GCC) 8.2.0
````

Sur le conteneur Docker, créez un fichier *helloworld.c*:

```` c
#include <stdio.h>

int main()
{
  printf("Hello World!\n");
}
````

Ensuite, lancez la commande ci-dessous pour compiler le fichier C précédent:

````
# arm-poky-linux-gnueabi-gcc \
  --sysroot=/opt/poky/2.6/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/
  -mfpu=neon \
  helloworld.c -o helloworld
````

Vous devriez obtenir l'erreur suivante:

````
/opt/poky/2.6/sysroots/x86_64-pokysdk-linux/usr/libexec/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/8.2.0/real-ld: error: helloworld uses VFP register arguments, /tmp/ccoPBAQX.o does not
/opt/poky/2.6/sysroots/x86_64-pokysdk-linux/usr/libexec/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/8.2.0/real-ld: failed to merge target specific data of file /tmp/ccoPBAQX.o
````

Cette erreur signifie que la toolchain est compilée avec une ABI différente
de celle qu'utilise *arm-poky-linux-gnueabi-gcc* par défaut.

**Question 5**: Cherchez dans la
                [documentation](https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html)
                des options de compilations pour ARM les valeurs possibles
                du paramètre `-mfloat-abi`. Tester les paramètres et indiquer
                laquelle permet de compiler correctement.

Un bon article: https://embeddedartistry.com/blog/2017/10/9/r1q7pksku2q3gww9rpqef0dnskphtc

### Image

Les images résultantes de la compilation sont dans le répertoire
*build/tmp/deploy/images/raspberrypi3*.

Pour flasher le système complet sur une carte sd, une seule commande suffit:

````
$ dd if=core-image-minimal-raspberrypi3.rpi-sdimg \
  of=/dev/sdX
````
