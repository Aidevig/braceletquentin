# LIRE CE README EN ENTIER!

# Code pour ADAFRUIT FEATHER M0 BLUEFRUIT LE

Ce code est réalisé pour une carte adafruit feather bluefruit m0 a venir souder dans le PCB assemblé de la section schema.
Ce microcontroleur utilisant un SAMD21 et non un ATMEGA, il n'est pas compatible pour les arduinos autre que ceux embarquant un SAMD21 (arduino zero, micro?, feather m0..)

# Fonctionnalités Développées

- Gestion mémoire flash : module carte SD (connection SPI)
- Connectivité bluetooth et envois alerte basique (connection SPI intégré sur le feather m0 bluefruit)
- Enregistrement de fichiers audios (WAV 8KHz) sur carte SD grâce au module microphone max4466 adafruit
- lecture de l'accélération en 3D par accéléromètre ADXL345 (connection I2C)
- lecture du rythme cardiaque par capteur cardiaque BH1790GLC (connection I2C)
- Détection alerte si le rythme cardiaque n'est pas compris entre les seuils min et max
- Pseudo Real Time Clock qui permet de dater les logs
- Lecture des seuils min et max pour le rythme cardiaque depuis un fichier texte sur la carte SD (conf.txt)
- gestion de différents types d'appuis sur un bouton poussoir (long, 1 court, 2 courts, 3 courts) BTN1 voir aidevig_pinDefinition
- Possibilité d'activer la débug console de l'IDE arduino en passant le #DEFINE DEBUG à 1

# Fonctionnement du Bracelet

- S'allume lorsqu'on branche la batterie adafruit adaptée et se recharge lorsqu'on branche le bracelet en USB.
- A l'allumage, la LED ROUGE doit clignoter quelques secondes très rapidement. Sinon, problème: réessayer plusieurs fois.
- A l'allumage, va lire les seuils limite dans le fichier conf.txt sur la carte SD
- A l'allumage, le bracelet configure sa connection bluetooth : il s'appel "GO-SAFE"
- Enregistre toutes les secondes les valeurs de l'accéléromètre x,y,z et le rythme cardiaque en datant le log dans le fichier DATALOG.txt
- Lorsqu'on appuie longtemps sur le bouton, lance un enregistrement audio qui s'arrête lorsque celui ci est relaché. La LED rouge s'allume pendant l'enregistrement
- Lorsqu'on appuie une fois sur le bouton, envois une alerte au mobile via bluetooth
- Si le rythme cardiaque est hors des seuils minCardio et maxCardio, le bracelet va proposer une alerte (LED Rouge clignotante) pendant 7 secondes.
		*Si durant ces 7 secondes, on appuie 2 fois sur le bouton, on arrête l'alerte
		*Sinon, l'alerte est envoyée.

# TO DO

- Développer l'utilisation du bluetooth en parallèle avec un développeur de l'appli mobile
- améliorer l'utilisation des LED
- Calibrer les valeurs de l'accéléromètre et du rythme cardiaque
- Calibrer les interruptions de l'accéléromètre (voir datasheet + debug console qui affiche les IT accéléromètre triggered)
ATTENTION : les IT accéléromètre sont désactivées en mode non debug
- Informer du niveau de batterie restant lors de 3 appuis (voir tuto adafruit m0 bluefruit, liens plus bas)
- Gérer le vibreur plutôt qu'une LED lors de la proposition d'alerte
- Créer des alertes automatiques sur IT accéléromètre où valeurs particulières d'accélération

# TUTO POUR PROGRAMMER LE FEATHER M0 BLUEFRUIT LE

- PREMIEREMENT : suivre ce tuto https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/setup
- télécharger l'archive Code de ce repository et extraire dans un répertoire du nom de votre choix
- Copier LE CONTENU du dossier librairies dans le dossier librairies de votre IDE Arduino ( .../mesdocuments/arduino/librairies)
- Ouvrir le fichier protoMesureQuentin.ino du répertoire protoMesureQuentin, brancher en USB le feather M0 Bluefruit LE, sélectionner le bon port dans l'IDE et cliquer sur téléverser

# En cas de problème

Contacter Quentin Van Straaten : vanstraatenquentin@gmail.com