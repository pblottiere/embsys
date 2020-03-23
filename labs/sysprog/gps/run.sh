#! /bin/sh

SCRIPT=`readlink -f $0`
ROOT_DIR=`dirname $SCRIPT`

export LD_LIBRARY_PATH=$ROOT_DIR/lib
$ROOT_DIR/bin/gps
