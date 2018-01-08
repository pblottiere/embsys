# Partie 2 : Domoticz

Dans le cadre d'un système domotique, 2 types de matériels peuvent être
différenciés:

- les capteurs qui mesurent une grandeur physique
- les actionneurs qui permettent de réaliser une action

Dans le cas des capteurs, la grandeur mesurée doit être envoyée au serveur
alors que dans le cas des actionneurs, c'est le serveur qui envoie la commande
à réaliser. Par exemple, un capteur de température pourra envoyer la valeur
mesurée toutes les 5 secondes vers le serveur. En revanche, si un utilisateur
souhaite allumer une lumière, c'est le serveur qui va envoyer une commande.


# Contenu

  * [Capteurs](#capteurs)
    * [Création](#création)
    * [API JSON](#api-json)
    * [Programme C](#pgrogramme-c)
  * [Actionneurs](#actionneurs)
    * [Création](#création)
    * [Action](#api-json)
    * [Programme C](#pgrogramme-c)


## Capteurs

Le but est ici de créer sur Domoticz un capteur virtuel de température.
Ensuite, nous pourrons mettre à jour sa température en ligne de commande,
en shell, en Python, en C, ...

### Création

Pour créer un capteur virtuel, aller dans l'onglet **Setup** puis dans le
menu **Hardware**. Créer alors un capteur virtuel en indiquant un nom et
le type **Dummy**:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_dummy.png" width="600" title="Github Logo">
</p>

Finalement, cliquer sur **Create Virtual Sensors** et indiquer **Temperature**
comme type:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_virtual.png" width="600" title="Github Logo">
</p>

Le nouveau capteur est alors disponible dans l'onglet **Temperature** de
l'interface:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_temp0.png" width="600" title="Github Logo">
</p>

### API JSON

Domoticz fournit une API JSON très complète avec une documentation
plutôt fournie : https://www.domoticz.com/wiki/Domoticz_API/JSON_URL's.

Par exemple, nous pouvons récupérer tous les capteurs de température
actuellement utilisés sur le serveur:

```` shell
$ curl "http://192.168.1.30:8080/json.htm?type=devices&filter=temp&used=true"
{
   "ActTime" : 1515344429,
   "result" : [
      {
         "AddjMulti" : 1.0,
         "AddjMulti2" : 1.0,
         "AddjValue" : 0.0,
         "AddjValue2" : 0.0,
         "BatteryLevel" : 255,
         "CustomImage" : 0,
         "Data" : "0.0 C",
         "Description" : "",
         "Favorite" : 0,
         "HardwareID" : 5,
         "HardwareName" : "temp0",
         "HardwareType" : "Dummy (Does nothing, use for virtual switches only)",
         "HardwareTypeVal" : 15,
         "HaveTimeout" : false,
         "ID" : "14058",
         "LastUpdate" : "2018-01-07 16:52:10",
         "Name" : "temp0",
         "Notifications" : "false",
         "PlanID" : "0",
         "PlanIDs" : [ 0 ],
         "Protected" : false,
         "ShowNotifications" : true,
         "SignalLevel" : "-",
         "SubType" : "LaCrosse TX3",
         "Temp" : 0.0,
         "Timers" : "false",
         "Type" : "Temp",
         "TypeImg" : "temperature",
         "Unit" : 1,
         "Used" : 1,
         "XOffset" : "0",
         "YOffset" : "0",
         "idx" : "8"
      }
   ],
   "status" : "OK",
   "title" : "Devices"
}
````

On observe ici que l'ID correspondant à notre capteur est **8** d'après le
champs *idx*.

Grâce à ce champs, il est possible d'utiliser l'API pour mettre à jour la
valeur courante du capteur de température:

```` shell
$ curl "http://192.168.1.30:8080/json.htm?type=command&param=udevice&idx=8&nvalue=0&svalue=20"
````

L'interface est alors mise à jour avec la nouvelle température:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_temp0_update.png" width="350" title="Github Logo">
</p>


### Programme C

Écrire un programme C (ou C++) permettant de mettre à jour un capteur de
température grâce à la librairie [libcurl](https://curl.haxx.se/libcurl/).
Le programme pourra être utilisé comme ci-dessous:

```` shell
$ ./myprog <IP> <PORT> <IDX> <TEMP>
````

Une fois testé sur la machine hôte, utiliser la chaîne de cross-compilation
générée par Buildroot (cf TODO) pour générer un binaire exécutable sur la RPI3.


Finalement, tester le programme sur la carte.


## Actionneurs

Le but est ici de créer sur Domoticz un switch booléean permettant de réaliser
une action. Dans ce cas, au moment de l'intéraction utilisateur (switch ON), un
message sera envoyé sur le réseau. Ce message sera ensuite récupéré et une action
simple sera réalisée.


### Création

Pour créer un swicth, aller dans l'onglet **Setup** puis dans le menu
**Hardware**. Créer alors un switch en indiquant un nom et le type **Dummy**:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_switch.png" width="450" title="Github Logo">
</p>

Finalement, cliquer sur **Create Virtual Sensors** et indiquer **Temperature**
comme type:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_switch_type.png" width="300" title="Github Logo">
</p>

Le nouveau capteur est alors disponible dans l'onglet **Switches** de
l'interface. L'état par défaut est OFF:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_switch_off.png" width="600" title="Github Logo">
</p>


### Action

Un utilisateur peut cliquer sur l'ampoule du switch pour changer l'état
courant. Une action peut alors être cablée en fonction du passage ON->OFF ou
OFF->ON. Pour cela, cliquer sur **Edit** au niveau du switch et renseigner
les champs **On Action** et **Off Action** avec un script shell de mise à jour:

<p align="center">
  <img src="https://github.com/pblottiere/embsys/blob/master/labs/rpi3/imgs/domoticz_switch_scripts.png" width="450" title="Github Logo">
</p>

Ensuite, créer ces 2 scripts dans le répertoire **/opt/domoticz/scripts** sur
la carte RPI3:

```` shell
$ cd /opt/domoticz/scripts
$ touch switch0_on.sh switch0_off.sh
$ chmox a+x swicth0_on.sh switch0_off.sh
````

Implémenter des actions simples dans ces scripts (comme
``$ echo "ACTION ON $(date)" >> /tmp/switch0.log``) et cliquer sur le switch
pour vérifier le bon fonctionnement.

### Programme C

Étant donné que le matériel à allumer/éteindre via l'action ne sera pas
connecté à la carte hébergeant le serveur Domoticz, une communication réseau
est nécessaire.

Un client doit envoyer un message indiquant le statut ON ou OFF à un serveur
chargé de réaliser l'action correspondante.

Écrire donc 2 programmes (un client et un serveur) et faire en sorte que le
client envoie le message correspondant (ON ou OFF) lorsque l'utilisateur
clic sur le switch.

Finalement, cross-compiler ces 2 programmes et écrire un script d'init.d
pour lancer le programme serveur au moment du démarrage de la carte. Tester.
