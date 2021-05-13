# RPI3

## Partie 3: Flashage et UBoot

* Question 1: There are two partition:
  - */dev/mmcblk0p1*: The root of the system, with the configuration for the boot of the os.
  - */dev/mmcblk0p2*: With the contents of the filesystem, with folder such as /etc of the root, the home and others.

* Question 2: TX is GPIO14 and RX is GPIO15, they are used for serial communication.

* Question 3: It has the following configuration:
  - Baud rate: 115200
  - Byte rate: 8 bit

* Question 4: The IP address of the board is 172.20.10.26. That was found by running the command *ip a* inside the board, after using the serial connection to access it.

* Question 5: The difference between root and user is that root is not configure through the users.table file, as the super user is always created. In order to access the root user from the ssd connection one needs first to connect with the normal user and then use the *su* command, as root access from the connection is not allowed. Another difference is that the root folder is accessible only with the root user.

* Question 6:
  - U-Boot fatload loads a binary file from a dos filesystem.
  - setenv sets the environment variables.
  - boot boots Linux zImage image from memory.
  
After that we did not manage to launch the system, as it never booted.


## Partie 5: GPIO et relais

* Question 1: A starting value was set for the pin, defining it as an output and with a high value. 
 We managed also to do this with a bash script, which was composed of echo commands that set the desired values within the /sys/class/pwm/ configuration files. That was used as well for the questions 3.

* Question 2, 3, 4: The scripts worked as in the examples. 

## Partie 6: I2C et BME280

* Question 1:
![Pins connection](pins.jpg)

* Question 2:
  - VCC: input
  - GND: ground
  for the bus:
    - SCL: SCK (serial clock)
    - SDA: SDI (serial data)

* Question 3: We see the following now:

  *0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f     
  00:          -- -- -- -- -- -- -- -- -- -- -- -- --  
  10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
  20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
  30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
  40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
  50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
  60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
  70: -- -- -- -- -- -- -- 77* 

* Question 4: Page 32, we see that the slave address is 0x77.

* Question 5: 
  - For the compilation of bme280.c and linux_userspace.c we used docker.
    In order to launch it: 
    `sudo docker start "id of the docker"`
    `sudo docker exec -it "id of the docker" /bin/bash`
    
    We used the pins as shown [here](https://www.velleman.eu/downloads/29/infosheets/vma335_scheme.pdf).

    It was necessary to clone the repository informed in the exercise at */root/*, from the state proposed in [this](https://github.com/BoschSensortec/BME280_driver/pull/80) pull request. Then, the source file linux_userspace.c was moved from the examples folder into the root of the project for it compiled.
  
  - For the cross compilation:
    `./../buildroot-precompiled-2017.08/output/host/usr/bin/arm-linux-gcc bme280.c linux_userspace.c -o bme`

    The output is called bme.
    
  - In order to display the sensors on the raspberry it, the file was copied from the docker and the following line was executed:`./bme /dev/i2c-1`

    The values appear, but they seem frozen.

* Extra:
  For the creation of a prototype of a weather monitoring station, the file [linux_userspace.c](linux_userspace.c) mentioned before was modified in order to change the files in */sys/class/gpio/*, alternating between green and red LEDs on when the temperature passed the mark of 25 degrees Celsius. 

  No other features were added as there were only 2 LEDs to be used.
