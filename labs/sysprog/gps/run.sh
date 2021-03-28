#! /bin/sh

SCRIPT=`readlink -f $0` #qui doit interpréter le script, ici shell, mais ça pourrait être python
ROOT_DIR=`dirname $SCRIPT`
echo $ROOT_DIR

export LD_LIBRARY_PATH=$ROOT_DIR/lib 
# export LD_LIBRARY_PATH=$(pwd)/lib 
ldd .$ROOT_DIR/bin/gps
# ldd ./bin/gps
# rajoute un chemin pour aller chercher les librairies partagées aux chemins classiques /lib /usr/lib /usr/local/lib
$ROOT_DIR/bin/gps
