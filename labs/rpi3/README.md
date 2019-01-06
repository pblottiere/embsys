# RPI3

Le but de ce laboratoire est d'élaborer un système domotique complet via
l'utilisation de cartes RPI3 et de divers capteurs (température, humidité,
pression, caméra, ...).

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/rpi3.png" width="300" title="Github Logo">
</p>

Une 1ère carte sera chargée d'héberger un serveur de domotique:
[Domoticz](https://domoticz.com/). Ce serveur est le point d'entrée pour
l'utilisateur pour contrôler son système (récupération de la température,
allumer une lumière, ...).

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_sensors.jpg" width="500" title="Github Logo">
</p>

Les capteurs seront quand à eux connectés sur une 2ème carte. Des drivers
devront être codés en C et divers protocoles seront étudiés (en fonction du
type de capteur: GPIO, i2C, 1-Wire, ...). Cette 2ème carte sera chargée
d'envoyer les données de ces capteurs au serveur Domoticz via le réseau.

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/arch.png" width="550" title="Github Logo">
</p>

Pour l'instant, voici les étapes à suivre pour construire progressivement ce
dont nous avons besoin:

[Partie 1 : Construction du système d'exploitation avec Buildroot](buildroot.md)

[Partie 2 : Émulation de la RPI3 avec QEMU et chroot](qemu.md)

[Partie 3 : Construction du système d'exploitation avec Yocto](yocto.md)

[Partie 4 : Flashage et U-Boot](flash_uboot.md)

[Partie 5 : GPIO et relais](relais.md)

[Partie 6 : I2C et BME280](bme280.md)

TODO: [Partie 7 : 1 Wire et DS18B20](ds18b20.md)

TODO: [Partie 8 : TMP36 et Convertisseur Analogique Numérique](can.md)

TODO: [Partie 9 : Communication en RS232](rs232.md)

[Partie 10 : Communiquer avec Domoticz](domoticz.md)
