#! /bin/sh

SCRIPT=`readlink -f $0`
echo $SCRIPT
ROOT_DIR=`dirname $SCRIPT`
echo $ROOT_DIR
export LD_LIBRARY_PATH=$ROOT_DIR/lib
$ROOT_DIR/bin/gps
