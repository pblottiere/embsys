# Autotools

Un outil de build permet de gérer les différentes phases de travail de
déploiement d'un logiciel :

  * configuration
  * compilation
  * installation / désinstallation

Il existe de nombreux systèmes de build, en fonction des langages utilisés, des
plateformes et plus généralement des besoins et exigences de chacun. Pendant
cet exercice, nous allons utiliser un système de build classique : les
autotools.

Tout d'abord rendez-vous dans le répertoire *project*. Dans ce mini-projet, les
autotools sont utilisés pour gérer l'étape de compilation du client de notre
serveur de LED. Il y a aussi un mini serveur permettant de tester le bon
fonctionnement de notre application.

```` bash
$ cd project
$ ls
autogen.sh  configure.ac  Makefile.am
````

Les fichiers *configure.ac* et *Makefile.am* sont les marques de fabriques
des autotools. On peut alors lancer la phase de configuration :

```` bash
$ ./autogen.sh
$ ./configure
...
config.status: creating config.h
config.status: executing depfiles commands
config.status: executing libtool commands
configure:

  autotools 0.1 - Configuration Report

 prefix:            /usr/local
 exec-prefix:       ${prefix}
 bin path:          ${exec_prefix}/bin
 lib path:          ${exec_prefix}/lib
 include path:      ${prefix}/include
 etc path:          ${prefix}/etc
 data path:         ${prefix}/share
````

Cette phase de configuration, lancée dans un premier temps sans options, vérifie
si tout le nécessaire (librairies, headers, ...) est présent pour que la phase
de compilation se passe bien.

**Question 1** : En analysant le contenu du fichier *configure.ac*, déterminez
                 quel header est vérifié pendant la phase de configuration.

**Question 2** : Modifiez le fichier *configure.ac* pour forcer l'étape de
                 configuration à vérifier la présence d'un header inexistant
                 sur le système. Relancez la phase de configuration et observez
                 les plaintes! Remettez les choses dans l'ordre lorsque vous
                 avez terminé.

**Question 3** : En cherchant sur le net, déterminez à quoi sert la macro
                 *AC_ENABLE_SHARED* utilisée dans le fichier *configure.ac*.

**Question 4** : À quoi sert le fichier *Makefile.am*?

Lorsque la phase de configuration se passe bien, on peut lancer la phase de
compilation :

```` bash
$ make
````

Suite à cette étape, un binaire *src/client/client* est généré.

**Question 5** : Modifiez le fichier *Makefile.am* pour qu'il compile le
                 fichier *src/miniserver/miniserver.c* en un binaire nommé
                 *miniserver*.

Le miniserveur écoute sur un port réseau et log les messages reçus grâce à
syslog. Le client, quant à lui, envoie des messages sur le réseau à une adresse
IP particulière et sur un port particulier (par défaut le port 5000 codé en
dur).

**Question 6** : Grâce à l'aide des binaires *client* et *miniserver* (lancez
                 les binaires sans options), testez leur bon fonctionnement en
                 local.

Lors de la phase de compilation, le compilateur natif est utilisé. Dans le cadre
d'une cross-compilation, on peut utiliser l'option *--host* lors de la
configuration pour indiquer l'utilisation d'un compilateur spécifique au moment
du make. Par exemple :

````
$ ./configure --host=arm-linux
...
checking for arm-linux-gcc... arm-linux-gcc
...
````

Lors de la compilation, le compilateur *arm-linux-gcc* sera alors utilisé.
Cependant, il faut pour cela que le path du cross-compilateur soit dans la
variable d'environnement PATH.

````
$ export PATH=$PATH:<PATH_VERS_LE_BSP>/buildroot/output/host/usr/bin
````

Suite à cela, le cross-compilateur doit être accessible dans le shell sans
avoir besoin d'indiquer un chemin absolu.

**Question 7** : Relancez la phase de configuration/compilation pour
                 cross-compiler le binaire miniserver pour la RPI3.

**Question 8** : Utilisez le client natif côté PC fixe et le serveur
                 cross-compilé sur la RPI3 pour vérifier leur bon
                 fonctionnement.

Placez vous dans le répertoire *project* et lancez la commande
*make maintainer-clean*.

**Question 9** : En observant l'effet, déterminez à quoi sert cette
                 dernière commande.

Dans le cadre d'un projet utilisant les autotools, le fichier *.gitignore*
contiendra ceci:

```` bash
Makefile.in
aclocal.m4
autom4te.cache/
config.h.in
config/
configure
Makefile
config.h
config.status
libtool
m4/
stamp-h1
.deps
.dirstamp/
````
