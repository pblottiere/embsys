# RPI3 Docker images

The aim is to build a minimal docker image for RPI3 with Buildroot and Yocto
in order to facilitate students' life :).

## Buildroot

Build the buildroot tarball with packages, kernel configuration, busybox
configuration, ...:

````
$ cd buildroot
$ rm buildroot-precompiled-2017.08.tar.gz
$ docker build -f Dockerfile.tarball -t embsys:rpi3-buildroot-tarball .
$ docker run -d --name embsys-rpi3-buildroot-tarball embsys:rpi3-buildroot-tarball
$ docker cp embsys-rpi3-buildroot-tarball:/root/buildroot-precompiled-2017.08.tar.gz .
$ docker stop embsys-rpi3-buildroot-tarball
$ docker rm embsys-rpi3-buildroot-tarball
$ docker rmi embsys:rpi3-buildroot-tarball
````

Build the Docker image `embsys:rpi3-buildroot` ready to be used:

````
$ cd buildroot
$ docker build -f Dockerfile -t pblottiere/embsys-rpi3-buildroot .
````

## Yocto

TODO
