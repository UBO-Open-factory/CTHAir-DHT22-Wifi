//
//
//	identifiant du module pour reconnaissance par TOCIO
//
//si l'identifiant est vide alors l'adresse MAC sera envoyée pour identifier le module.
//Si l'identifiant n'est pas vide il sera envoyé pour identifier le module

// autorise ou non les sorties sur le port série
#define DEBUG  true

// identifiant du module sur TOCIO
String identifiant = "xxxx"; 

// timeout de connection au Wifi en secondes (mini 1)
unsigned long timeout_connection_wifi = 30;

// durée de mise en sommeil entre 2 mesures (en minutes mini 1)
unsigned long duree_mise_en_sommeil = 1;
