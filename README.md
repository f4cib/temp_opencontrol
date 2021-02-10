# temp_opencontrol
temp dev branch
How it works ?
Principe commutation antenne remote @ F6KNB (physique et logiciel)
F4CIB – Février 2021
Pour simplifier les choses on considère une recopie logicielle du boîtier physique (des contacteurs ou encodeur optiques sont possibles mais ca nécessite deux codes distincts...c’est bien plus compliqué!)

Boutons : 
0 Beverage 
1 Antennes Tx matrice band decoder
2 Antenne sur demande #1 (2 ele 40M)
3 Antenne sur demande #2 (Afrique 20M)
4 Antenne sur demande #3 (dipole 80M)
5 Set Tx

bev0 nw
bev1 ne
bev2 w
bev3 e
bev4 sw
bev5 se

En mode remote, on utilise l’ampli a transistor du synoptique, en mode contest, on utilise des amplis à tube (géré manuellement par Michel)

Fonctionnement :
Par défaut, on considère le mode Rx (le bouton 5 – Set Tx est off)
une action sur les boutons fait basculer entre les différentes antennes accessibles (bevs – tx ant – on demand ant x 3)
Default mode :
Si Bouton 0 beverage cliqué : S3 ON en position vers bevs, on récupère la dernière position bev en mémoire qu’on applique S ? on active le flag Is_bev_on (a voir si nécessaire)
Si Is_bev_on alors on regarde le statut des boutons bev_dir et on active le relais correspondant pour  choisir entre les 6 bevs
Le bouton 0 est inutile en remote, mais quasi incontournable en local.
En local, l’accès aux antennes on demand est impossible (configuration contest) il faut donc donc restreindre a beverage only (soit boîtier avec une seule commande dédiée bev, soit logiciellement)

Si bouton 1 Tx ant cliqué :S3 OFF (from PA) et S4 OFF (from band decoder antenna matrix) S5 OFF S6 OFF

Si bouton 2 on demand antenna 1 (2 ele 40M) cliqué : S3 OFF et S4 ON (from on demand ant #1) S5 OFF S6 OFF 

Si bouton 3 on demand antenna 2 (20M AF) cliqué : S3 OFF et S4 ON S5 ON (from on demand ant #2) S6 OFF 

Si bouton 4 on demand antenna 3 (dipole 80M) cliqué : S3 OFF et S4 ON  S5 ON S6 ON (from on demand ant #3)

Tx mode :
Pour changer d’antenne Tx, on clique sur bouton 5 (Set Tx) on choisit de l’antenne 1 à 4 et on valide en re-cliquant sur Set Tx on commute S4, S5, S6 en conséquence
Les antennes sont aujourd’hui des monobandes, on choisit donc une antenne qui correspond à la bande d’émission.


pins utilisées :
int buttonPin = 0;
const int ptt_InPin = 2;
const int ptt_OutPin = 9;
const int txModePin = 6;
const int txModeLedPin = 8;
const int DS_pin = 5;
const int STCP_pin = 3;
const int SHCP_pin = 7;
void setupDigitalWrites()
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
}

Tous les boutons antennes Tx sont relies sur A0 (8K2 ; 2K2 ; 3K3 ; 6K8 ; 18K(variable buttonPin) au travers d’un pont resistif et suivant la tension mesurée sur A0 on sait quel bouton a été enclenché. alimenté en 12V les valeurs lues sont 0 ; 200 ; 385 ; 590 ; 785
c’est le rôle de la fonction getPressedButton()

Tous les boutons bev vont vers A1 (8K2, 2K2, 3K3,, 6K8, 12K, 22K) alimenté en 12V les valeurs lues sont 0 ; 210 ; 395 ; 595 ; 750 ; 860
Attention a l’alimentation 4.77V alimenté en USB vs. 4.94V alimenté sous 9V (méthode à favoriser)

Le PCF8574 gère le LCD // 16x2
Le 74HC595 gère 4 relais (S3, S4, S5 et S6) bev antenne matrice ou une des 3 antennes auxiliaires.
Il faut donc rajouter une électronique qui gère la commutation des 6 beverages.
La carte W5500 gère l’Ethernet

PTT arrive d’un relais et envoie du 0/5V il est séquencé par l’Arduino et ressort en 0/5V.

Relais beverages  

How 74HC595 works:
https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
Use of 2 74HC595 to extend to 16 outputs from 3 wires:
https://www.instructables.com/Arduino-16-LEDs-using-two-74HC595-shift-registers-/

Le dialogue button vers WEB fonctionne
Dans le sens WEB vers ARDUINO seule SetTx envoie un message, les boutons 1 à 5 rien voir les serial.print ajoutés pour le debug.
AC mais sans les serial.print le sens arduino/button vers html ne marchait pas, il manquerait du délai ?


