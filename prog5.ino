// Supports PDF d'Ateliers Arduino par www.mon-club-elec.fr
// http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ATELIERS
// par X. HINAULT - tous droits réservés - 2013 - GPLv3

// code : 17. Exemple 1-wire : DS18B20 : Mesure de la température à l'aide de capteurs multiples 

// --- Inclusion des librairies utilisées ---
#include <OneWire.h> // librairie pour capteur OneWire

// --- constantes des broches ---
const int broche_OneWire=2; //declaration constante de broche 

// --- Déclaration des variables globales ---
byte adresse[8][10]; // Tableau de 8 octets pour stockage du code d'adresse 64 bits du composant One Wire - jusqu'à 10 capteurs ici 
byte adresseCourante[8]; // Tableau de 8 octets pour stockage du code d'adresse courante
int compt=0; // variable de comptage du nombre de capteurs sur le bus One Wire

byte data[12]; // Tableau de 12 octets pour lecture des 9 registres de RAM et des 3 registres d'EEPROM du capteur One Wire
int temperature=0; // variable entiere pour stocker la température brute
float temperaturef=0.0; // variable décimale pour stocker la température réelle

//---- code d'instruction du capteur
const int modeLecture=0xBE;
const int lancerMesure=0x44;

// --- Déclaration des objets utiles pour les fonctionnalités utilisées ---
OneWire  capteur(broche_OneWire);  // crée un objet One Wire sur la broche voulue

void setup()   { // debut de la fonction setup()

Serial.begin(115200); // initialise connexion série à 115200 bauds
// IMPORTANT : régler le terminal côté PC avec la même valeur de transmission 

//---- détection des capteurs présents sur le bus One Wire
Serial.println("*** Liste des elements presents sur le bus 1-wire *** ");


while (capteur.search(adresseCourante)== true) { // tant qu'un nouveau capteur est détecté
  // la fonction search renvoie la valeur VRAI si un élément 1-wire est trouvé. Stocke son adresse dans le tableau adresse
  // adresse correspond à l'adresse de début du tableau adresse[8] déclaré ... 

  // ce code est exécuté pour chaque capteur détecté
  compt=compt+1; // incrémente la variable de comptage du nombre de compteurs
  Serial.print ("Numero ");
  Serial.print (compt);
  Serial.print (" : 1 capteur 1-wire present avec code adresse 64 bits : ");  // la fonction search renvoie la valeur VRAI si un élément 1-wire est trouvé. Stocke son adresse dans le tableau adresse
  // adresse correspond à l'adresse de début du tableau adresse[8] déclaré ... 

  //--- affichage des 64 bits d'adresse au format hexadécimal
   for(int i = 0; i < 8; i++) { // l'adresse renvoyée par la fonction search est stockée sur 8 octets
   
    if (adresseCourante[i]<16) Serial.print('0'); // pour affichage des O poids fort au format hexadécimal
    Serial.print(adresseCourante[i], HEX); // affiche 1 à 1 les 8 octets du tableau adresse au format hexadécimal
    Serial.print(" ");
    
    adresse[i][compt]=adresseCourante[i]; // mémorise dans le tableau 2D l'adresse courante

     } // fin for 

  Serial.println(); 

  //---- test du type de capteur ---- 
  // le type du capteur est donné par le 1er octet du code adresse 64 bits
  // Valeur 0x28 pour capteur type DS18B20, 0x10 pour type DS18S20, 0x22 pour type DS1820
  if (adresseCourante[0]==0x28) Serial.println ("Type : Capteur temperature DS18B20.");
  
  
  //----- contrôle du code CRC ----
  // le dernier octet de l'adresse 64bits est un code de contrôle CRC 
  // à l'aide de la fonction crc8 on peut vérifier si ce code est valide
  if (capteur.crc8( adresseCourante, 7) == adresseCourante[7]) // vérification validité code CRC de l'adresse 64 bits
  // le code CRC de l'adresse 64 bits est le 8ème octet de l'adresse (index 7 du tableau)
  {
    Serial.println ("Verification du code CRC de l'adresse 64 bits de ce capteur : VALIDE !"); 
  }
  else
  {
    Serial.println ("Verification du code CRC de l'adresse 64 bits de ce capteur : NON VALIDE !");     
  }
  
  Serial.println("------------"); 

} // fin boucle while de test présence capteur

if (compt==0) // si aucun capteur n'a été détecté
{
  Serial.println("Aucun capteur present sur le bus 1-wire"); // affiche message + saut de ligne
}
else // si au moins 1 capteur a été détecté
{
  Serial.print(compt); // affiche nombre de capteurs détectés
  Serial.println (" capteur(s) detecte(s) sur ce bus 1-wire");   // affiche message + saut de ligne
  Serial.println ("**** Recherche terminee - fin de liste **** "); // affiche message + saut de ligne

} // fin else 


} // fin de la fonction setup()

void loop(){ // debut de la fonction loop()

  for (int index=1; index<=compt; index++) { // défile les 4 capteurs - 1er capteur = index 1 !

      // récupère adresse du capteur courant dans tableau 2D  
      for (int j=0; j<8; j++) {adresseCourante[j]=adresse[j][index];}
  
    //--------- lancer une mesure --------
      capteur.reset(); // initialise le bus 1-wire avant la communication avec un capteur donné
      capteur.select(adresseCourante); // sélectionne le capteur ayant l'adresse 64 bits contenue dans le tableau envoyé à la fonction
      capteur.write(lancerMesure,1); // lance la mesure et alimente le capteur par la broche de donnée
    
    //-------- pause d'une seconde ----- 
    delay(1000);     // au moins 750 ms
               // il faudrait mettre une instruction capteur.depower ici, mais le reset va le faire
    
    //---------- lancer une LECTURE des Registres ------------- 
      capteur.reset(); // initialise le bus 1-wire avant la communication avec un capteur donné
      capteur.select(adresseCourante); // sélectionne le capteur ayant l'adresse 64 bits contenue dans le tableau envoyé à la fonction
      capteur.write(modeLecture,1); // passe en mode lecture de la RAM du capteur
      
    // ----------- lire les 9 octets de la RAM (appelé Scratchpad) ----
    
    for ( int i = 0; i < 9; i++) {           // 9 octets de RAM stockés dans 9 octets
        data[i] = capteur.read();             // lecture de l'octet de rang i stocké dans tableau data
      }
      
    
    //----- test de validité des valeurs reçues par contrôle du code CRC  ----
    
    Serial.println("");
    Serial.println("---- Resultat de mesure de la temperature ---- ");
    
      // le dernier (9ème) octet de la RAM est un code de contrôle CRC 
      // à l'aide de la fonction crc8 on peut vérifier si ce code est valide
      if (capteur.crc8( data, 8) == data[8]) // vérification validité code CRC des valeurs reçues
      {
        Serial.println ("Verification du code CRC des Registres : VALIDE !"); 
      }
      else
      {
        Serial.println ("Verification du code CRC des Registres  : NON VALIDE !");     
      }

   //----- réalisation d'une mesure de température--- 
    data[1]=data[1] & B10000111; // met à 0 les bits de signes inutiles
    temperature=data[1]; // bits de poids fort
    temperature=temperature<<8; // décalage de 8 bits vers la gauche = vers les bits de poids fort
    temperature=temperature+data[0]; // bits de poids faible ici mis dans les 8 premiers bits
    
    Serial.print ("Mesure brute ="); 
    Serial.println (temperature); 
    
    // --- en mode 12 bits, la résolution est de 0.0625°C - cf datasheet DS18B20
    temperaturef=float(temperature)*6.25;
    temperaturef=temperaturef/100.0;
    
    Serial.print ("Mesure Finale ="); 
    Serial.print (temperaturef,2); 
    Serial.println (" Degres Celsius. "); 

  } // fin for index
  
  while(1); // stop loop

} // fin de la fonction loop()
