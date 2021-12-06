# CTHAir-DHT22-Wifi
Capteur de température et d'hygrométrie (DHT22) connecté à un réseau Wifi

Le capteur présenté ici permet de mesurer la température et l'hygrométrie d'un lieu puis d'envoyer ces informations par Wifi à un serveur de données.


##Description
    Il intègre :
        - un capteur spécifique pouvant mesurer à la fois la température et l'hygrométrie de l'air
        - un microcontrôleur gérant le fonctionnement de l'ensemble
        - un chargeur de batterie  (https://www.robotshop.com/eu/fr/chargeur-batteries-lipo-micro-usb-adafruit.html?gclid=Cj0KCQiAnaeNBhCUARIsABEee8XIQpiwdCSFdvlMACDXwCc1oCrDVzvkIQyEiWymK_8KR6TJM6AMqEUaAtSwEALw_wcB)
        - un interrupteur On/Off
        - Quelques composants discrets pour le fonctionnement du microcontôleur.

    Il se présente sous la forme d'un petit boitier de 58 x 50 x 26 mm perc de quelques trous intégrant la carte électronique et une batterie.

##Fonctionnement
    Le module réalise une mesure de temparature, une mesure d'hygrométrie et une mesure de la tension de la batterie.
    Il se connecte ensuite à un réseau Wifi dont on lui a comuniqué les paramètres (SSID et mot de passe).
    Puis il envoie les données sur un centre de données  dont on lui a communiqué l'adresse. Parmi les données envoyées figure un identifiant qui permettra au centre de données de savoir de quel module il reçoit des informations et donc comment les enregistrer.
    Enfin il se met en veille pour une durée programmée après laquelle il recommencera un cycle de mesure/envoi/sommeil...

##Electronique
    L'électronique est composée  
        - d'une carte électronique qui mesure 36 x 44 mm. Elle est composée d'un circuit simple face. Le schéma électrique et le schéma d'implantation sont données dans le répertoire "Electronique" au format Eagle et PDF.
        - d'un ESP8266-12 Wifi (https://www.conrad.fr/p/joy-it-esp8266-module-uart-wifi-esp12-f-1-pcs-1707668?gclid=Cj0KCQiAnaeNBhCUARIsABEee8VpehfSateDr-OI0zyCJifhymOjrz4KTu_t2pbg9GdaGwhpkJJzVcgaAu0AEALw_wcB&gclsrc=aw.ds&utm_campaign=shopping-feed&utm_content=free-google-shopping-clicks&utm_medium=surfaces&utm_source=google&utm_term=1707668&vat=true). Il est programmé pour réaliser toutes les fonctions du module. 
        - d'un chargeur pour batteries LiPo (https://www.adafruit.com/product/1904). Ce chrageur peut accepter toue batterie LiPo de 3.7V jusqu'à 5000 mAh. Il possède un prise batterie spécifique (JST2) ainsi qu'un prise de charge type USB mico.
        - d'un capteur de température et hygrométrie relative (HR) DHT22 (https://www.robot-maker.com/shop/capteurs/31-dht22-31.html). Il peut mesurer des températures comprises entre 140 et +80°C avec une marge d'erreur de 0.5°C. Il peut mesurer un taux d'humidité de 0% à 100% avec une marge d'erreur de 2%.
        - un régulateur "low drop" (placé côté pistes de la carte électronique) dont le rôle est de ramener la tension batterie à une tension de acceptable pour le fonctionnement du module (typiquement 3,3 V).
        - Quelques composants dont les rôles sont multiples (filtrage d'alimentation, polarisation de certaines broches de l'ESP8266 pour le démarrage).

        L'assemblage de la carte électronique est décrit das le document "Assemblage" dans le répertoire "Documentation".

##Programme
    Le programme est décomposé en plusieurs fichiers disponibles dans le répertoire "programmation".
        Le fichier "CTHAir_DHT22_Wifi.ino" qui contient le programme principal.
        Le fichier "variables.h" qui contient les variables principales de fonctionnement du programme. Notamment l'identifiant du module qui sera transmis avec les données mesurées. Cet identifiant permettra qu centre de données de savoir quel module envoie des données.
        Le fichier "wifi_data.h" qui contient les paramètres de connection à un réseau Wifi. Les deux paramètres sont a adapter en fonction du réseau auquel on veut se connecter.
        Le fichier "server_data.h" qui contient les paramètres du serveur de données sur lequel on veut envoyer les données (hote, URL, port et fingerprint).
        Le fichier "type_converter.h" qui contient quelques fonctions utiles pour trnasformer des variables numériques en chaines de caractères formatées.


    L'ESP se programme (par exemple) avec l'IDE Arduino et à l'aide du programmateur décrit ci-dessous.

##Programmateur

    La programmation de l'ESP nécessite passe par une liaison UART et un convertisseur UART/USB pour communiquer avec un ordinateur de bureau. La solution adoptée a été de modifier un NodeMCU en enlevant l'ESP présent sur celui-ci et en le remplaçant par deux connecteurs. Ceci permet d'insérer des ESP à programmer.
    
    Mise en garde : le désoudage de l'ESP et la soudure des connecteurs est un peu délicate. Une photo du programmateur est dsponible dans le répertoire "programmateur"


##Batteries
    Les batteries utilisables sont de type LiPo 3.7V. Plusieurs capacités sont disponibles (250 mAh, 400 mAh, 1000 mAh et 5000 mAh).