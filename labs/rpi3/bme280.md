# BME280

Le BME280 est un capteur de pression, température et humidité construis par
Bosh. La ddescription commercial est
[ici](https://www.bosch-sensortec.com/bst/products/all_products/bme280).

Ce capteur peut fonctionner soit par SPI, soit par I2C. Dans le cadre de ce TP,
nous allons utiliser un bus I2C pour lire les données fournies par le capteur:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/bme280.jpeg" width="300" title="BME280">
</p>

## Activation du support de l'I2C

Pour activer le support de l'I2C via le Device Tree, il faut tout d'abord
modifier le fichier *config.txt* de la 1ère partition de la carte SD:

````
dtparam=i2c1=on
dtmparam=i2c_arm=on
````

Ensuite, démarrez la RPI3 et connectez vous à la carte. Puis chargez les
modules suivant:

```` shell
$ modprobe i2c-dev
$ modprobe i2c-bcm2835
````

Vous devriez alors avoir accès au bus I2C à travers le répertoire */dev*:

```
$ ls /dev/i2c-1
```

## i2cdetect

L'utilitaire *i2cdetect* permet de scanner les bus I2C détecter si du matériel
y est connecté.

Par exemple pour scanner le bus 1:

```` shell
$ i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
````

En vous inspirant du schéma ci-dessous, connectez le BME280 à la RPI3:

 <p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/bme280_pins.png" width="300" title="BME280">
</p>

**Question 1**: Réalisez un schéma de brochage simple de la connection entre la     
                RPI3 et le BME280.

**Question 2**: Décrivez rapidement l'utilité de chaque broche.

**Question 3**: Une fois le BME280 connecté, relancez la commande
                *i2cdetect -y 1*. Quelle différence observez-vous par
                rapport à tout à l'heure.

**Qestion 4**: En lisant la page 32 de la
[datasheet](https://www.embeddedadventures.com/datasheets/BME280.pdf), cette
               différence vous parait-elle normale?  

## Driver du BME280

Le driver officiel fournis par le constructeur est ici: https://github.com/BoschSensortec/BME280_driver

**Question 5**: Utilisez l'API C du driver pour récupérer les données de
                température, de pression et d'humidité fournies par le capteur.
