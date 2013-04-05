// Supports PDF d'Ateliers Arduino par www.mon-club-elec.fr
// http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ATELIERS
// par X. HINAULT - tous droits réservés - 2013 - GPLv3

// code : 18. Annexe : 1-wire : DS18B20 : Utilisation d'une librairie Arduino dédiée

#include <OneWire.h>
#include <DallasTemperature.h>

// le capteur est connecté sur la broche 2 - précision 9 bits
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

// Initialise le bus OneWire pour utiliser un capteur 1-wire quelconque
OneWire oneWire(ONE_WIRE_BUS);

//Transmet le pointeur de l'objet OneWire à la librairie Dallas Temperature.
DallasTemperature sensors(&oneWire);

int numberOfDevices; //variable pour nombre de capteurs trouvés

DeviceAddress tempDeviceAddress; // variable de stockage d'une adresse de capteur trouvé

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Démarre la librairie DallasTempérature
  sensors.begin();
  
  // Comptage des capteurs 
  numberOfDevices = sensors.getDeviceCount();
  
  // localise les capteurs sur le bus
  Serial.print("Recherche des capteurs...");
  
  Serial.print("Trouve ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" capteurs.");

  // message sur l'etat de l'alimentation
  Serial.print("Parasite power est: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  // Défile les capteurs trouvés et affiche adresse
  for(int i=0;i<numberOfDevices; i++)
  {
    // cherche le capteur pour l'adresse 
    if(sensors.getAddress(tempDeviceAddress, i))
{
Serial.print("Trouve capteur ");
Serial.print(i, DEC);
Serial.print(" avec adresse: ");
printAddress(tempDeviceAddress);
Serial.println();

Serial.print("Fixe la resolution a : ");
Serial.println(TEMPERATURE_PRECISION, DEC);

// fixe la résolution à utiliser - jusqu'à 12 bits
sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

Serial.print("Resolution actuellement a : ");
Serial.print(sensors.getResolution(tempDeviceAddress), DEC);
Serial.println();
}else{
Serial.print("Trouve capteur fantome a ");
Serial.print(i, DEC);
Serial.print(" mais ne peut pas obtenir l'adresse. Verifier cablage et connexion capteur.");
}
  }

}
