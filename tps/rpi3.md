# RPI3

## Partie 3: Flashage et UBoot

* Question 1: There are two partition:
  - */dev/mmcblk0p1*: The root of the system, with the configuration for the boot of the os.
  - */dev/mmcblk0p2*: With the contents of the filesystem, with folder such as /etc of the root, the home and others.

* Question 2: TX is GPIO14 and RX is GPIO15, they are used for serial communication.

* Question 3: It has the following configuration:
  - Baud rate: 115200
  - Byte rate: 8 bit
  - 

* Question 4: The IP address of the board is 172.20.10.26. That was found by running the command *ip a* inside the board, after using the serial connection to access it.

* Question 5: The difference between root and user is that root is not configure through the users.table file, as the super user is always created. In order to access the root user from the ssd connection one needs first to connect with the normal user and then use the *su* command, as root access from the connection is not allowed. Another difference is that the root folder is accessible only with the root user.

* Question 6:
  - U-Boot fatload loads a binary file from a dos filesystem.
  - setenv sets the environment variables.
  - boot boots Linux zImage image from memory.


## Partie 5:: Flashage et UBoot 

* Question
