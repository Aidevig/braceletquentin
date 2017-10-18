# Schema Electronique

Le fichier schema electronique est un fichier Eagle. Il contient le schéma électronique complet du prototype. Pour le réaliser, j'ai utilisé les schémas mis à disposition par adafruit sur ses différents modules. Ils sont aisément trouvables.

# Composants :
Microcontrolleur: 
Adafruit Feather M0 Bluefruit LE	https://www.adafruit.com/product/2995

Accéléromètre:
ADXL345 							https://www.adafruit.com/product/1231

Capteur Cardiaque:
BH1790GLC							http://www.rohm.com/web/global/products/-/product/BH1790GLC

micro:
Electret Microphone Amplifier - MAX4466 with Adjustable gain	https://www.adafruit.com/product/1063

# PCB
Le répertoire PCB contient le .brd (format eagle). Il contient également les gerbers et la BOM.

# Commander un PCB
Afin de commander un pcb assemblé il suffit d'utiliser les fichiers contenus dans l'archive PCB-BODYGUARD.rar et la BOM car les composants sont déjà correspondants entre la BOM et le .brd

Passer par le site EMSPROTO et compter deux semaines pour des prix corrects.
	
# A faire
-La carte électronique est conçue sans bouton bien que les trous soient présent pour venir en souder deux. Il faut également ajouter un vibreur sur le pin A0.
Ceci permet d'ajouter ces périphériques directement sur la structure mécanique de l'objet.
-La carte électronique vient également sans son microcontrôleur feather M0 Bluefruit qui doit être soudé dessus. Il est immaginable d'ajouter un connecteur à la place afin d'éviter d'avoir à souder le microcontrôleur.
-La batterie utilisée est une batterie adafruit compatible avec le Feather M0 Bluefruit LE, elle se recharge lorsque le bracelet est branché en USB. Le bracelet doit être allumé pour se recharger.
-Un fil de la batterie doit être coupé et soudé a un switch intégré à la structure mécanique afin de pouvoir allumer et éteindre le bracelet 	
	
# ATTENTION
Afin d'optimiser l'espace sur la carte, j'ai supprimé les boutons poussoirs. Il ne reste que le trou pour les connecter aux pins du microcontrôleur. Il faudra également connecter l'autre entrée à la masse.
Par conséquent, les fichiers eagles du schéma et de la board ne sont plus linkés. AUCUNE MODIFICATION SUR L'UN N'AURA D'EFFET SUR L'AUTRE
	