// Supports PDF d'Ateliers Arduino par www.mon-club-elec.fr
// http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ATELIERS
// par X. HINAULT - tous droits réservés - 2013 - GPLv3

// code : 12. Exemple 1-wire : DS18B20 : Détecter le capteur et obtenir son adresse

// --- Inclusion des librairies utilisées ---

#include <OneWire.h> // librairie pour capteur OneWire

// --- Déclaration des constantes ---

// --- constantes des broches ---

const int broche_OneWire=2; //declaration constante de broche 

// --- Déclaration des variables globales ---
byte adresse[8]; // Tableau de 8 octets pour stockage du code d'adresse 64 bits du composant One Wire

// --- Déclaration des objets utiles pour les fonctionnalités utilisées ---
OneWire  capteur(broche_OneWire);  // crée un objet One Wire sur la broche voulue

void setup()   { // debut de la fonction setup()

// --- ici instructions à exécuter au démarrage --- 

Serial.begin(115200); // initialise connexion série à 115200 bauds
// IMPORTANT : régler le terminal côté PC avec la même valeur de transmission 


//---- détection des capteurs présents sur le bus One Wire

Serial.println("*** Capteur present sur le bus 1-wire *** ");

while (capteur.search(adresse)!= true) { }// attend qu'un capteur soit détecté
  // la fonction search renvoie la valeur VRAI si un élément 1-wire est trouvé. Stocke son adresse dans le tableau adresse
  // adresse correspond à l'adresse de début du tableau adresse[8] déclaré ... 

  Serial.print (" 1 capteur 1-wire present avec code adresse 64 bits : ");
  
  //--- affichage des 64 bits d'adresse au format hexadécimal
   for(int i = 0; i < 8; i++) { // l'adresse renvoyée par la fonction search est stockée sur 8 octets
   
    if (adresse[i]<16) Serial.print('0'); // pour affichage des O poids fort au format hexadécimal
    Serial.print(adresse[i], HEX); // affiche 1 à 1 les 8 octets du tableau adresse au format hexadécimal
    Serial.print(" ");

     } // fin for 

 //---- test du type de capteur ---- 
  // le type du capteur est donné par le 1er octet du code adresse 64 bits
  // Valeur 0x28 pour capteur type DS18B20, 0x10 pour type DS18S20, 0x22 pour type DS1820
  if (adresse[0]==0x28) Serial.println ("Type : Capteur temperature DS18B20.");  
  
  //----- contrôle du code CRC ----
  // le dernier octet de l'adresse 64bits est un code de contrôle CRC 
  // à l'aide de la fonction crc8 on peut vérifier si ce code est valide
  if (capteur.crc8( adresse, 7) == adresse[7]) // vérification validité code CRC de l'adresse 64 bits
  // le code CRC de l'adresse 64 bits est le 8ème octet de l'adresse (index 7 du tableau)
  {
    Serial.println ("Verification du code CRC de l'adresse 64 bits de ce capteur : VALIDE !"); 
  }
  else
  {
    Serial.println ("Verification du code CRC de l'adresse 64 bits de ce capteur : NON VALIDE !");     
  }
  
  Serial.println("------------"); 
  Serial.println(); 

} // fin de la fonction setup()

void loop(){ // debut de la fonction loop()

// --- ici instructions à exécuter par le programme principal --- 

} // fin de la fonction loop() - le programme recommence au début de la fonction loop sans fin

