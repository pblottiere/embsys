#! /bin/bash

. ./oe-init-build-env rpi3-build
bitbake-layers add-layer ../meta-raspberrypi
echo "MACHINE = \"raspberrypi3\"" >> conf/local.conf
echo "INHERIT += \"rm_work\"" >> conf/local.conf
bitbake core-image-minimal
bitbake meta-toolchain
