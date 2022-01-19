# CTHAir-DHT22-Wifi
Capteur de température et d'hygrométrie (DHT22) connecté à un réseau Wifi

Le capteur présenté ici permet de mesurer la température et l'hygrométrie d'un lieu puis d'envoyer ces informations par Wifi à un serveur de données.


<h1>Description</h1>
    Il intègre :
    <ul>
        <li>un capteur spécifique pouvant mesurer à la fois la température et l'hygrométrie de l'air</li>
        <li>un microcontrôleur gérant le fonctionnement de l'ensemble</li>
        <li>un chargeur de batterie  (https://www.robotshop.com/eu/fr/chargeur-batteries-lipo-micro-usb-adafruit.html?gclid=Cj0KCQiAnaeNBhCUARIsABEee8XIQpiwdCSFdvlMACDXwCc1oCrDVzvkIQyEiWymK_8KR6TJM6AMqEUaAtSwEALw_wcB)</li>
        <li>un interrupteur On/Off</li>
        <li>Quelques composants discrets pour le fonctionnement du microcontôleur.</li>
    </ul>
    Il se présente sous la forme d'un petit boitier de 58 x 50 x 26 mm perc de quelques trous intégrant la carte électronique et une batterie.

<h1>Fonctionnement</h1>
    Le module réalise une mesure de température, une mesure d'hygrométrie et une mesure de la tension de la batterie.<br>
    Il se connecte ensuite à un réseau Wifi dont on lui a comuniqué les paramètres (SSID et mot de passe).<br>
    Puis il envoie les données sur un centre de données  dont on lui a communiqué l'adresse. Parmi les données envoyées figure un identifiant qui permettra au centre de données de savoir de quel module il reçoit des informations et donc comment les enregistrer.<br>
    Enfin il se met en veille pour une durée programmée après laquelle il recommencera un cycle de mesure/envoi/sommeil...

<h1>Electronique</h1>
    <h2>Version 2.4</h2>
    Elle est composée :
    <ul>
        <li>d'une carte électronique qui mesure 36 x 44 mm. Elle est composée d'un circuit simple face. Le schéma électrique et le schéma d'implantation sont données dans le répertoire "Electronique" au format Eagle et PDF.</li>
        <li>d'un ESP8266-12 Wifi (https://www.conrad.fr/p/joy-it-esp8266-module-uart-wifi-esp12-f-1-pcs-1707668?gclid=Cj0KCQiAnaeNBhCUARIsABEee8VpehfSateDr-OI0zyCJifhymOjrz4KTu_t2pbg9GdaGwhpkJJzVcgaAu0AEALw_wcB&gclsrc=aw.ds&utm_campaign=shopping-feed&utm_content=free-google-shopping-clicks&utm_medium=surfaces&utm_source=google&utm_term=1707668&vat=true). Il est programmé pour réaliser toutes les fonctions du module. </li>
        <li>d'un chargeur pour batteries LiPo (https://www.adafruit.com/product/1904). Ce chrageur peut accepter toue batterie LiPo de 3.7V jusqu'à 5000 mAh. Il possède un prise batterie spécifique (JST2) ainsi qu'un prise de charge type USB mico.</li>
        <li>d'un capteur de température et hygrométrie relative (HR) DHT22 (https://www.robot-maker.com/shop/capteurs/31-dht22-31.html). Il peut mesurer des températures comprises entre 140 et +80°C avec une marge d'erreur de 0.5°C. Il peut mesurer un taux d'humidité de 0% à 100% avec une marge d'erreur de 2%.</li>
        <li>un régulateur "low drop" (placé côté pistes de la carte électronique) dont le rôle est de ramener la tension batterie à une tension de acceptable pour le fonctionnement du module (typiquement 3,3 V).</li>
        <li>Quelques composants dont les rôles sont multiples (filtrage d'alimentation, polarisation de certaines broches de l'ESP8266 pour le démarrage).</li>
    </ul>
    L'assemblage de la carte électronique est décrit das le document "Assemblage" dans le répertoire "Documentation".
    <br>
    <h2>Version 3.1</h2>
    Elle est  composée des mêmes composants que l'électronique V2.4. La seule différence tient dans la carte électronique dont le schéma électrique est légèrement différent pour alimenter le DHT22 par un GPIO de l'ESP.
    Son montage est identique à la carte V2.4 à l'exception de l'emplacement des résistances. Les fichiers Eagle fournis donnent la nouvelle implantation.


<h1>Programme</h1>
    Le programme est décomposé en plusieurs fichiers disponibles dans le répertoire "programmation".
    L'ESP se programme (par exemple) avec l'IDE Arduino et à l'aide du programmateur décrit ci-dessous.
    <h2>Version 1.0</h2>
    Cette version utilise le protocole HTTP associé à un Fingerprint.
    <ul>
        <li>Le fichier "CTHAir_DHT22_Wifi_v1_0.ino" qui contient le programme principal.</li>
        <li>Le fichier "variables.h" qui contient les variables principales de fonctionnement du programme. Notamment l'identifiant du module qui sera transmis avec les données mesurées. Cet identifiant permettra qu centre de données de savoir quel module envoie des données.</li>
        <li>Le fichier "wifi_data.h" qui contient les paramètres de connection à un réseau Wifi. Les deux paramètres sont a adapter en fonction du réseau auquel on veut se connecter.</li>
        <li>Le fichier "server_data.h" qui contient les paramètres du serveur de données sur lequel on veut envoyer les données (hote, URL, port et fingerprint).</li>
        <li>Le fichier "type_converter.h" qui contient quelques fonctions utiles pour trnasformer des variables numériques en chaines de caractères formatées.</li>
    </ul>
    il est nécessaire de renseigner : 
        ssid et mod de passe dans le fichier "wifi_data.h"
        host, url et fingerprint dans le fichier "server_data.h"
        identifiant dans "variables.h"
    <br>
<h2>Version 1.1</h2>
    Dérivée de la version 1.0 elle n'utilise plus de fingerprint. LE reste fonctionne de la même manière.
    Il sera donc nécessaire de renseigner les mêmes données que dans la version 1.0 (sauf le fingerprint) avant de programmer l'ESP12.
    <h2>Version 2.0</h2>
    Similaire à la version 1.0 sur le plan de l'organisation. Seul la partie principale change pour s'adapter à l'alimentation du DHT22 par un GPIO.
    De ce fait il sera nécessaire de renseigner les mêmes données que dans la version 1.0 (sauf le fingerprint) avant de programmer l'ESP12.
<h1>Table de compatibilité</h1>
    Le tableau suivant indique les compatibilités entre le matériel et le logiciel.
<table>
    <tr>
        <td></td>
        <td><b>UOFPROTO2</b></td>
        <td><b>UOFPROTO3</b></td>
    </tr>
    <tr>
        <td><b>Electronique</b></td>
        <td>V 2.4</td>
        <td>V 3.1</td>
    </tr>
    <tr>
        <td><b>Programme</b></td>
        <td>V1.0<br>V1.1</td>
        <td>V2.0</td>
    </tr>
    <tr>
        <td><b>Boitier</b></td>
        <td>V1</td>
        <td>V1</td>
    </tr>
    <tr>
        <td><b>Programmateur</b></td>
        <td>V1</td>
        <td>V1</td>
    </tr>
</table>

<h1>Programmateur V1</h1>
    La programmation de l'ESP nécessite passe par une liaison UART et un convertisseur UART/USB pour communiquer avec un ordinateur de bureau. La solution adoptée a été de modifier un NodeMCU en enlevant l'ESP présent sur celui-ci et en le remplaçant par deux connecteurs. Ceci permet d'insérer des ESP à programmer.<br>
    Mise en garde : le désoudage de l'ESP et la soudure des connecteurs est un peu délicate. Une photo du programmateur est dsponible dans le répertoire "programmateur"

<h1>Batteries</h1>
    Les batteries utilisables sont de type LiPo 3.7V. Plusieurs capacités sont disponibles (250 mAh, 400 mAh, 1000 mAh et 5000 mAh).
