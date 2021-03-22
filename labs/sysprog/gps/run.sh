#! /bin/sh

SCRIPT=`readlink -f $0`
ROOT_DIR=`dirname $SCRIPT`

echo $ROOT_DIR

export LD_LIBRARY_PATH=libptmx.so $ROOT_DIR/lib/
$ROOT_DIR/bin/gps
