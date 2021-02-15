# temp_opencontrol
temp dev branch
# How it works ?
# Principe commutation antenne remote @ F6KNB (physique et logiciel)
F4CIB – Février 2021

Pour simplifier les choses on considère une recopie logicielle du boîtier physique 
(des contacteurs ou encodeur optiques sont envisageables mais ca nécessite de modifier le code !)
<br>
_**Par défaut**, le systeme fonctionne en **mode Rx**, si l'on appuie sur un des boutons, cela ne change que pour la réception._

## Boutons
### Antennes Tx:
* **0** Antennes Tx#1 de la matrice de commutation (gérée via le décodeur de bande) (S3:OFF ; S4:OFF ; S5:OFF ; S6:OFF ; S7:OFF ; S8:OFF)
* **1** Antenne Tx#2 40M 2 éléments (rotor) 	(S3:OFF ; S4:ON ; S5:OFF ; S6:OFF ; S7:OFF ; S8:OFF)
* **2** Antenne Tx#2 20M Afrique 			(S3:OFF ; S4:ON ; S5:ON ; S6:OFF ; S7:OFF ; S8:OFF)
* **3** Antenne Tx#2  80M dipôle 			(S3:OFF ; S4:ON ; S5:ON ; S6:ON ; S7:OFF ; S8:OFF)
* **4** Antenne Tx#2 40M-6M verticale R8 		(S3:OFF ; S4:ON ; S5:ON ; S6:ON ; S7 ON ; S8:OFF)
* **5** Antenne Tx#2 30M-10M quad 			(S3:OFF ; S4:ON ; S5:ON ; S6:ON ; S7:ON ; S8:ON)
* **9** Set Tx (permet de changer l'antenne Tx)

### Beverage
* **bev0** Active les Beverage (S3 ON)
* **bev1** Beverage NW
* **bev2** Beverage NE
* **bev3** Beverage W
* **bev4** Beverage E
* **bev5** Beverage SW
* **bev6** Beverage SE

## Fonctionnement :
Par défaut, on considère le mode Rx (le bouton 9 – Set Tx est off)
<br>une action sur les boutons fait basculer entre les différentes antennes accessibles en reception (Beverage ; Tx#1 ; Tx#2)
### Default mode :
Si Bouton 0 beverage cliqué : S3 ON en position vers bevs, on récupère la dernière position bev en mémoire qu’on applique S ? on active le flag Is_bev_on (a voir si nécessaire)
<br>Si Is_bev_on alors on regarde le statut des boutons bev_dir et on active le relais correspondant pour  choisir entre les 6 bevs
<br>Le bouton 0 est inutile en remote, mais quasi incontournable en local. (Voir s'il faut le garder)
<br>
_En local, l’accès aux antennes on demand est impossible (configuration contest) il faut donc donc restreindre a beverage only (soit boîtier avec une seule commande dédiée bev, soit logiciellement)_

### Tx mode :
Pour changer d’antenne Tx, on clique sur bouton 5 (Set Tx) on choisit de l’antenne 1 à 4 et on valide en re-cliquant sur Set Tx on commute S4, S5, S6 en conséquence
<br>Les antennes sont aujourd’hui des monobandes, on choisit donc une antenne qui correspond à la bande d’émission.

## pins utilisées :
* int buttonPin = 0;
* const int ptt_InPin = 2;
* const int ptt_OutPin = 9;
* const int txModePin = 6;
* const int txModeLedPin = 8;
* const int DS_pin = 5;
* const int STCP_pin = 3;
* const int SHCP_pin = 7;
<br>
```void setupDigitalWrites()
{
	digitalWriteFast(ptt_InPin, HIGH);
	digitalWriteFast(ptt_OutPin, LOW);
	digitalWriteFast(txModePin, HIGH);
	digitalWriteFast(txModeLedPin, HIGH);
}
void setupPinMode()
{
	pinModeFast(buttonPin, INPUT_PULLUP);
	pinModeFast(DS_pin, OUTPUT);
	pinModeFast(STCP_pin, OUTPUT);
	pinModeFast(SHCP_pin, OUTPUT);
	pinModeFast(ptt_InPin, INPUT);
	pinModeFast(ptt_OutPin, OUTPUT);
	pinModeFast(txModePin, INPUT);
	pinModeFast(txModeLedPin, OUTPUT);
}```

## Cablâge:
Tous les boutons antennes Tx sont relies sur A0 (8K2 ; 2K2 ; 3K3 ; 6K8 ; 18K(variable buttonPin) au travers d’un pont resistif et suivant la tension mesurée sur A0 on sait quel bouton a été enclenché. alimenté en 12V les valeurs lues sont 0 ; 200 ; 385 ; 590 ; 785
<br>c’est le rôle de la fonction getPressedButton()
<br>
<br>Tous les boutons bev vont vers A1 (8K2, 2K2, 3K3,, 6K8, 12K, 22K) alimenté en 12V les valeurs lues sont 0 ; 210 ; 395 ; 595 ; 750 ; 860
<br>_**Attention a l’alimentation 4.77V alimenté en USB vs. 4.94V alimenté sous 9V (méthode à favoriser)**_
<br>
<br>Le PCF8574 gère le LCD // 16x2
<br>Le 74HC595 gère 4 relais (S3, S4, S5 et S6) bev antenne matrice ou une des 3 antennes auxiliaires.
<br>Il faut donc rajouter une électronique qui gère la commutation des 6 beverages.
<br>La carte W5500 gère l’Ethernet

<br>PTT arrive d’un relais et envoie du 0/5V il est séquencé par l’Arduino et ressort en 0/5V.
<br>
<br>Relais beverages  

## How 74HC595 works:
https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
<br>Use of 2 74HC595 to extend to 16 outputs from 3 wires:
<br>https://www.instructables.com/Arduino-16-LEDs-using-two-74HC595-shift-registers-/
