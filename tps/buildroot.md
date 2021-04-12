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

* Question 8: 
