a


sh deploy/sdk/poky-glibc-x86_64-meta-toolchain-cortexa7t2hf-neon-vfpv4-toolchain-2.6.sh

. /opt/poky/2.6/environment-setup-cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi

arm-poky-linux-gnueabi-gcc
  --sysroot=/opt/poky/2.6/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi/
  -mfpu=neon -mfloat-abi=hard \
  coucou.c -o coucou

""""
/opt/poky/2.6/sysroots/x86_64-pokysdk-linux/usr/libexec/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/8.2.0/real-ld: error: plouf uses VFP register arguments, /tmp/ccoPBAQX.o does not
/opt/poky/2.6/sysroots/x86_64-pokysdk-linux/usr/libexec/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/8.2.0/real-ld: failed to merge target specific data of file /tmp/ccoPBAQX.o
""""

=> The error you are observing happens because some parts of the code are built with the “hard float” ABI and others with “soft float”.

""""
cc1: error: -mfloat-abi=hard: selected processor lacks an FPU
""""

ARM options for gcc: https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html

Demystifying ARM Floating Point Compiler Options: https://embeddedartistry.com/blog/2017/10/9/r1q7pksku2q3gww9rpqef0dnskphtc
( the two ARM ABIs (hard-float and soft-float) are not link-compatible. Your entire program must be compiled using the same ABIs. If a pre-compiled library is not supplied with your target floating-point ABI, you will need to recompile it for your own purposes.)
