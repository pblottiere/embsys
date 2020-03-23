#! /bin/sh

SCRIPT=`readlink -f $0`
ROOT_DIR=`dirname $SCRIPT`/../gps

export LD_LIBRARY_PATH=$ROOT_DIR/lib
export LD_PRELOAD=libhook.so


$ROOT_DIR/bin/gps
