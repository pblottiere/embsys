#! /bin/sh

cd ..
SCRIPT=`readlink -f $0`
ROOT_DIR=`dirname $SCRIPT`/gps
echo $ROOT_DIR

export LD_LIBRARY_PATH=$ROOT_DIR/lib
LD_PRELOAD=libhook.so $ROOT_DIR/bin/gps
$ROOT_DIR/bin/gps
