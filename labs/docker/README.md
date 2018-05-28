# Docker

*Docker est un logiciel libre qui automatise le déploiement d'applications dans des conteneurs logiciels. Selon la firme de recherche sur l'industrie 451 Research, « Docker est un outil qui peut empaqueter une application et ses dépendances dans un conteneur isolé, qui pourra être exécuté sur n'importe quel serveur ». Ceci permet d'étendre la flexibilité et la portabilité d’exécution d'une application, que ce soit sur la machine locale, un cloud privé ou public, une machine nue, etc...*

## images et pull

Récupération d'une image Debian stable vierge en local:

```` shell
$ docker pull debian:stable
$ docker images
REPOSITORY                  TAG             IMAGE ID        CREATED         SIZE
debian                      stable          49e46d4f55fe    4 weeks ago     100M
$ docker run -it debian:stable /bin/bash
````

## run

Démarrage d'un nouveau conteneur en mode shell à partir de l'image Debian:

```` shell
$ docker run -it debian:stable /bin/bash
root@xxxxxxxxxxxx:/#
````

## ps et exec

Connexion à un conteneur existant à partir de son ID:

```` shell
$ docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
xxxxxxxxxxxx   debian:stable   "/bin/bash"   About 1 hour ago   Up 2 seconds          dazzling_colden
$ docker exec -it xxxxxxxxxxxx /bin/bash
````

## stop et start

Arrêt d'un conteneur existant et actif:

```` shell
$ docker stop xxxxxxxxxxxx
$ docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
$ docker ps -a
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
xxxxxxxxxxxx   debian:stable   "/bin/bash"   About 1 hour ago   Up 2 seconds          dazzling_colden
````

Démarrage d'un conteneur existant arrêté:

```` shell
$ docker container start xxxxxxxxxxxx
$ docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED            STATUS        PORTS   NAMES
xxxxxxxxxxxx   debian:stable   "/bin/bash"   About 1 hour ago   Up 2 seconds          dazzling_colden
````

## cp

Récupération d'un fichier présent sur un conteneur:

```` shell
$ docker cp xxxxxxxxxxxx:/file/path/within/container /host/path/target
````
