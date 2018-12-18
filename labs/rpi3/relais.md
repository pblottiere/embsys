# Partie 5 : GPIO et relais

Numérotation des pins pour la RPi3 Model B v1.2:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/rpi3pins.png" width="550" title="Github Logo">
</p>


## GPIO et LED

### Sysfs

Dans l'espace utilisateur, les GPIO sont accessible via le système de fichier
dans **/sys/class/gpio**. Cette option est disponible seulement si on a installé
la lib **libsysfs** au moment de la compilation avec Buildroot.

Réaliser le montage suivant en utilisant la GPIO 17 (n'oubliez pas que la
broche la plus courte de la LED est le **-**):

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/led_res.jpg" width="350" title="Github Logo">
</p>

**Question 1**: Grâce à la documentation du kernel sur
                [sysfs](https://www.kernel.org/doc/Documentation/gpio/sysfs.txt),
                allumez et éteignez la LED connectée à la GPIO 17.

### Python

Il existe aussi un paquet python pour la RPi. Ce paquet, installé via
buildroot, est nommé **python-rpi-gpio**. La documentation est
[ici](https://sourceforge.net/p/raspberry-gpio-python/wiki/Home/).

**Question 2**: Utilisez cette API Python pour allumer/éteindre la LED
                connectée à la GPIO 17 (faire attention au mode BCM ou BOARD).


### PWM

Le PWM, ou Pulse Mith Modulation, permet de simuler un signal continu en
moyenne grâce à des signaux discrets (de valeur constante fixe). En image,
cela donne ceci :

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/pwm.png" width="500" title="Github Logo">
</p>

Deux paramètres permettent donc de gérer la tension moyenne souhaitée :
- le *duty_cycle*
- la *period*

Dans l'espace utilisateur, la logique d'utilisation est la même que pour les
GPIO : on passe par des fichiers virtuels de /sys/class pour le contrôle.

La carte RPi3 possède un connecteur J8 où se trouve des pins PWM. Ici, nous
allons utiliser la GPIO 1 (pin 12).

Pour faire du PWM via sysfs, voici la démarche théorique (théorique car
pour une raison qui m'échappe encore, le fichier
 **/sys/class/pwm/pwmchip0** n'est pas créé par le kernel: inspecter les
overlays):

```` shell
$ cd /sys/class/pwm/
$ ls
pwmchip0
$ export PWM_CHIP=pwmchip0
$ export PWM=12
$ cd pwmchip0/
$ ls
device     export     npwm       power      subsystem  uevent     unexport
$ echo $PWM > /sys/class/pwm/$PWM_CHIP/export
$ ls
device     npwm       pwm4       uevent
export     power      subsystem  unexport
$ echo 1 > /sys/class/pwm/$PWM_CHIP/pwm$PWM/enable
$ echo 50000 > /sys/class/pwm/$PWM_CHIP/pwm$PWM/duty_cycle
$ echo 100000 > /sys/class/pwm/$PWM_CHIP/pwm$PWM/period
````

Par contre nous pouvons utiliser l'API Python sans encombre:

```` python
# python
Python 2.7.13 (default, Jan  7 2018, 19:42:20)
[GCC 6.4.0] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import RPi.GPIO as GPIO
>>> GPIO.setmode(GPIO.BOARD)
>>> GPIO.setup(12, GPIO.OUT)
>>> p = GPIO.PWM(12, 0.5)
>>> p.start(1)
>>> p.stop()
````

**Question 3**: Modifiez la fréquence et le duty_cycle pour faire varier la
                luminosité de votre LED.

## Relais

L'intérêt d'un relai est de contrôler un appareil allimenté en 230V (ou 5V,
12V, etc) à partir d'un GPIO. Typiquement, ouvrir une porte de garage!

Réaliser le montage suivant avec:
- les pins PWR/GND pour remplacer la pise 220V
- une LED à la place du réveil

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/relai.png" width="350" title="Github Logo">
</p>

**Question 4**: Testez l'allumage de la LED via le contrôle de la GPIO par Sysfs
                et Python.
