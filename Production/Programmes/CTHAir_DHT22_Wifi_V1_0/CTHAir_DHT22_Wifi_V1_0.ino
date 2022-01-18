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
* - Version 1.0    :
*                   - Répartition des données sensibles dans différents fichiers locaux
*                   - initialisation du module
*                   - initialisation du capteur et mesures des paramètres
*                   - connection au réseau Wifi (timeout)
*                   - création d'un client pour le serveur TOCIO
*                   - envoi des mesures vers TOCIO
*                   - mise en sommeil 
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

// instanciation du capteur
DHTesp dht;

// constantes
 #define DHTPIN 5       // DHT relié à la broche GPIO5

// variables
float temperature = 0;
float hygrometrie = 0;
int batterie = 0;
String adresse_mac = "";
String chaine_a_envoyer = "";

void setup() {
  // initialisation de la liaison série
  Serial.begin(115200);
  delay(200);
  #if (DEBUG)
  Serial.println(" Liason USB prête !");
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
    Serial.println (hygrometrie);
    
    // calibration si la lecture  échouée
    if (isnan(hygrometrie)) hygrometrie = 999;
    Serial.println (hygrometrie);
    
    // mesure de la température
    float temperature = dht.getTemperature();
    
    // calibration si la lecture  échouée
    if (isnan(temperature)) temperature = -99.99;
    
    // mesure de la tension batterie (en pas de CAN)
    batterie = analogRead(A0);
    
    // formatage des mesures dans une chaine de caractères
    Serial.print ("hygrometrie : ");
    chaine_a_envoyer = float_to_string(hygrometrie,false, 3,0);
    Serial.print ("température : ");
    chaine_a_envoyer += float_to_string(temperature,true, 2, 2);
    
    chaine_a_envoyer += unsignedint_to_string(batterie, 4);
    
    
    #if (DEBUG)
    Serial.println ("Chaine à envoyer : <" + chaine_a_envoyer + ">");
    #endif
   
  
    // envoie les mesures
    String res = sendData(identifiant, chaine_a_envoyer);
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


String sendData(String moduleID, String data) {
  // Envoi les mesures ("data") passées en paramètre dans le module "moduleID" au serveur TOCIO.
  // @param moduleID : L'identifiant défini dans le Back Office de TOCIo pour ce module.
  // @param data : String contenant les mesures formatée selon la payload défini dans le Back Office de Tocio
  // If we are connecte to the WIFI
  moduleID.toUpperCase();

  if (WiFi.status() == WL_CONNECTED) {
  
    //  Create an https client
    WiFiClientSecure monclient;
    
    const int port = 443;
    monclient.setFingerprint(fingerprint);
    
    if (!monclient.connect(host, port)) {
      
      #if (DEBUG)
      Serial.print("connection failed.");
      #endif
      
      return "nok";
    }
    // Send data to the client avec la methode GET
    String request = url + moduleID + "/" + data;



    
    #if (DEBUG)
    Serial.println ("Request : " + request);
    Serial.println("Requesting : " + request);
    #endif
    
    monclient.print(String("GET ") + request + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    // Lecture de ce qui est renvoyée par le serveur
    while (monclient.available()) {
      String line = monclient.readStringUntil('\r');
      #if (DEBUG)
      Serial.println(line);
      #endif
    }
    
    #if (DEBUG)
    Serial.println("closing connection");
    #endif
   
    monclient.stop();
    return "ok";
  } else {
    return "nok";
  }
}


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
