# Partie 1: Construction d'OS avec Buildroot et chaine de cross-compilation 

* Question 1: 
  * *configs/embsys_defconfig*: It is buildroot's configuration file, with specifications of how the system is to be built, what is has access to and the tools available. There are fields in the configuration file for each of the aspects of the system being specified, using boolean or simple strings to configure.
  * *busybox.config*: It is the configuration file use by busybox, the toolbox used to facilitate the building of embedded systems. The configuration is done via a list of flags to be enabled.
  * *users.table*: It is a file used by buildroot to specify the basics of each user, such as their home and their default shell. Each line corresponds to a user.

* Question 2: Considering that it is the 32 bits version of a raspberry 3, it should use the file *configs/raspberrypi3_defconfig*.

* Question 3: The folder *package/* contains all of the packages to be made available by the system, containing the build information needed to create them, as well as the validity hashes and variables used set during the compilation. Sometimes they are grouped in subfolders when they correspond to the same category.

* Question 4: The command compiles the base necessary to customize the rest of the building process, creating an initial .config file that will then be customizable trough the make menuconfig.

* Question 5:
  * Architecture: ARM little endian
  * CPU: ARMV8
  * ABI: EABIhf, meaning that the board has a floating point unit and it doesn't have to be emulated via software.
  * C library: uClibc-ng
  * Cross compiler: GCC 6.x
  * Kernel: Custom kernel from raspberry pi's repository.
  
* Question 6: Yes, openssh will be compiled and that can be seen in the Target packages -> Network applications section of the menuconfig. The same can be determined with the existence of the flag BR2_PACKAGE_OPENSSH=y in the configuration file.

* Question 7: Busybox is a set of tools made available to make easier the development of embedded systems, aiding the with multiple packages made available withing a single binary. The make command displays the configuration interface related to busybox's available packages, allowing us to customize that is to be included when compiling the system with it.

* Question 8: *output/host/* contains the generated toolchain from the cross compilation. The mentioned binary is the gcc compiler built for the targeted architecture (ARM, in the raspberry pi).

* Question 9: The file was created in the src folder, inside the uncompressed tarball. The output of *file hw* is hw: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=aeb0a450fe32efb19ded5a57c8e9129c2d6b283a, not stripped. It is the specification of the contents of the file. When it is run, it displays "Hellow world!".

* Question 10: When run, it displays hw: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped. It is a binary generated for a different architecture (ARM, as opposed to x86-64). This happens because it used the package generated aiming at the other architecture, as intended.

* Question 11: *output/images/* contains the compiled images of the system. Examples are the one made for the sdcard (*sdcard.img*), a compressed image (*zImage*), and the initial RAM disk (*rootfs.tar*).

* Question 12: zImage outputs "zImage: Linux kernel ARM boot executable zImage (little-endian)", and sdcard.img outputs "sdcard.img: DOS/MBR boot sector; partition 1 : ID=0xc, active, start-CHS (0x0,0,2), end-CHS (0x4,20,17), startsector 1, 65536 sectors; partition 2 : ID=0x83, start-CHS (0x4,20,18), end-CHS (0x1d,146,54), startsector 65537, 409600 sectors". This can be understood as the first one being self contained and executable, and the second one just an image to be booted. 

* Question 13: In */tmp/rootfs/* can be found the extracted root file system, which is the base system for the operating system being built. 
