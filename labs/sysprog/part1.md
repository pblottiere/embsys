# Partie 1: Compilation, debug, signaux et threads

Dans cette partie, nous allons se plonger dans le langage C, étudier la
chaine de compilation et explorer les signaux ainsi que les threads.


## Contenu

  * [Simulateur GPS](#simulateur-gps)
    * [Norme NMEA](#norme-nmea)
    * [Communication série](#communication-serie)
    * [Compilation du simulateur](#compilation-du-simulateur)
    * [Exécution du simulateur](#execution-du-simulateur)
    * [Terminal série (minicom)](#terminal-serie)
  * [Exercices](#exercices)
    * [Compilation et débogage](#compilation-et-debogage)
    * [Hook et signaux](#hook-et-signaux)
  * [À retenir](#a-retenir)

## Simulateur GPS

### Norme NMEA

*La norme NMEA 0183 est une spécification pour la communication entre
équipements marins, dont les équipements GPS. Elle est définie et contrôlée
par la National Marine Electronics Association (NMEA), association américaine
de fabricants d'appareils électroniques maritimes, basée à Severna Park au
Maryland (États-Unis d'Amérique).

La norme 0183 utilise une simple communication série pour transmettre une
phrase à un ou plusieurs écoutants. Une trame NMEA utilise tous les
caractères ASCII. (Wikipedia).*

Ces trames NMEA 0183 sont de tailles variables et codées en ASCII (caractères
8 bits) contrairement aux trames NMEA 2000. Elles commencent toutes par le
caractère *$* (excepté les trames venant de l'AIS commençant par *!*). Les deux
caractères suivants indiquent le type de matériel. Ainsi, on a par exemple pour
les trois premiers octets :

    * $GP : trame GPS
    * $HC : trame compas
    * $RA : trame radar
    * ...

Par exemple :

````
$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39
$GPGGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M, , *42
$GPGLL,4916.45,N,12311.12,W,225444,A
$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K
...
````

Aujourd'hui, nous allons travailler avec un simulateur GPS simplifié simulant
l'envoie de deux types de trames par liaison série virtuelle :

  * trame GLL : Geographic position, latitude / longitude
  * trame VTG : Track made good and ground speed

Ces trames sont envoyées periodiquement par le binaire *gps* que nous verrons
plus tard.

**NOTE** : Grâce aux équipements marins, une carte en ligne indique en temps réel
la position des navires : http://www.marinetraffic.com/fr/.

### Communication série

TODO

### Compilation du simulateur

TODO

### Exécution du simulateur

TODO

### Terminal série (minicom)

TODO

## Exercice 1: compilation et débogage

TODO

## Exercice 2: Hook et signaux

TODO

## À retenir

Suite à ce TP, les concepts suivants sont considérés acquis:

TODO
