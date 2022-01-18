

/*
 *          PROTOTYPE DE CAPTEUR DE TEMPERATURE ET D'HUMIDITE POUR PASSERELLE
 *          
 *  - Description  :  ce programme est intégré dans le module UOF PROTO2V2 composé 
 *                    d'un ESP8266 associé à un capteur DHT22.  L'ESP est entouré de 
 *                    quelques résistances pour lui permettre de démarrer.
 *                    Il est alimenté par une batterie reliée à un chargeur intégré
 *          
 *                    Nécessite l'installation du Core ESP8266 pour arduino disponible ici : https://github.com/esp8266/Arduino
 *                    Nécessite l'installation de la bibliothéque DHT disponible ici : http://librarymanager/All#DHTesp
 *                    
 *  - Auteur       : Laurent Marchal(UBO OpenFactory)
* 
* - Date           : oct 2021
* 
* - Licence        : CC-BY-SA
* 
* 
* 
* 
* - Versions       : 
* 
*                     2.0
*                         - Répartition des données sensibles dans différents fichiers locaux
*                         - initialisation du module
*                         - alimentation du module
*                         - initialisation du capteur et mesures des paramètres
*                         - connection au réseau Wifi (timeout)
*                         - création d'un client pour le serveur TOCIO
*                         - envoi des mesures vers TOCIO (pas de fingerprint)
*                         - coupure du capteur
*                         - mise en sommeil 
*/



// inclusion des bibliothèques
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// fichiers locaux
#include "variables.h";
#include "wifi_data.h";
#include "server_data.h";
#include "type_converter.h";
#include "Tocio.h";

// instanciation du capteur
DHTesp dht;

// constantes
 #define DHTPIN 5       // DHT relié à la broche GPIO5

// variables
float temperature = 0-98.5;
float hygrometrie = 12;
int batterie = 0;
String adresse_mac = "";
String chaine_a_envoyer = "";

void setup() {
  // initialisation de la liaison série
  Serial.begin(115200);
  while (!Serial){
    delay(20);
  }

  #if (DEBUG)
  Serial.println(" Liaison USB prête !");
  #endif
  
  // initialisation du capteur sur la broche correspondante
  dht.setup(DHTPIN, DHTesp::DHT22); // Connect DHT sensor to GPIO 5

  // connection au reseau Wifi
  if (connection_WiFi()) {

    // determination de l'identifiant
    if (identifiant == "") identifiant = adresse_mac;
    
    // fin de l'initialisation
    #if (DEBUG)
    Serial.println("Initialisation terminée.");
    Serial.println("");
    Serial.print ("Identifiant du module : " + identifiant);
    Serial.println("");
    #endif
    // mesure du taux d'humidité
    hygrometrie = dht.getHumidity();
    #if (DEBUG)
    Serial.println (hygrometrie);
    #endif
    // calibration si la lecture  échouée
    if (isnan(hygrometrie)) hygrometrie = 999;
    Serial.println (hygrometrie);
    
    // mesure de la température
    temperature = dht.getTemperature();
    #if (DEBUG)
    Serial.println (temperature);
    #endif
    
    // calibration si la lecture  échouée
    if (isnan(temperature)) temperature = -99.99;
    Serial.println (temperature);
    
    // mesure de la tension batterie (en pas de CAN)
    batterie = analogRead(A0);
    
    // formatage des mesures dans une chaine de caractères
    #if (DEBUG)
    Serial.print ("hygrometrie : ");
    #endif
    chaine_a_envoyer = float_to_string(hygrometrie,false, 3,0);
    #if (DEBUG)
    Serial.print ("température : ");
    #endif
    chaine_a_envoyer += float_to_string(temperature,true, 2, 2);
    chaine_a_envoyer += unsignedint_to_string(batterie, 4);
    
    #if (DEBUG)
    Serial.println ("Chaine à envoyer : <" + chaine_a_envoyer + ">");
    #endif
   
  
    // envoie les mesures
    String res = sendDataInHTTPSRequest(chaine_a_envoyer);
    #if (DEBUG)
    Serial.println ("C/R : " + res);
    #endif
    

  }
  // mise en sommeil
  #if (DEBUG)
  Serial.print ("Mise en sommeil pour ");
  Serial.print (String (duree_mise_en_sommeil));
  Serial.println (" minutes");
  #endif
  ESP.deepSleep(duree_mise_en_sommeil*60000000);
}

void loop() {
  // rien ici
}


// **********************************************************
//*****          F O N C T I O N S
// **********************************************************



bool connection_WiFi() {
  unsigned long debut = millis();
  #if (DEBUG)
  Serial.print("connecting to ");
  Serial.println(ssid);
  #endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  #if (DEBUG)
  Serial.println ("+----------------+");
  Serial.print("Adresse MAC : ");
  #endif
  adresse_mac = WiFi.macAddress();
  #if (DEBUG)
  Serial.println(adresse_mac);
  Serial.println ("+----------------+");
  #endif
    
  while (WiFi.status() != WL_CONNECTED) {
    if ((millis() - debut) > (timeout_connection_wifi*1000)) {
      #if (DEBUG)
      Serial.println("");
      Serial.println ("Connection WIFI impossible");
      #endif
      return false;
    }
    delay(500);
    #if (DEBUG)
    Serial.print(".");
    #endif
  }
  #if (DEBUG)
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Adresse IP : ");
  Serial.println(IpAddress2String(WiFi.localIP()));
  #endif
  return true;
}
