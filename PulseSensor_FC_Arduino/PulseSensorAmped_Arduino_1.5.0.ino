/*
 * -------------------- Code Arduino pour Pulse Sensor Amped 1.5 --------------------
 * 
 * Auteur et Source du code : Joel Murphy and Yury Gitman | http://www.pulsesensor.com
 * 
 * Code modifie dans le cadre du Projet Tuteuré de 4°année de l'INSA Toulouse
 *      
*/
#include <SoftwareSerial.h>

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2


//  Variables
int pulsePin = 0;       // Pulse Sensor purple wire connected to analog pin 0
int RX_Newton = 0;      // Port RX de l'Arduino sur lequel on conecte le port TX de la Newton (connexion filaire)
int TX_Newton = 1;      // Port TX de l'Arduino sur lequel on conecte le port RX de la Newton (connexion filaire)
int RX_BLE = 7;         // Port Numérique n°7 de l'Arduino sur lequel on conecte le port TX du module HC-05
int TX_BLE = 8;         // Port Numérique n°8 de l'Arduino sur lequel on conecte le port RX du module HC-05

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS 
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 400;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// SET THE SERIAL OUTPUT TYPE TO YOUR NEEDS
static boolean serialVisual = false;            // Re-set to 'false' to sendDataToSerial instead. : ) 

//static int outputType = SERIAL_PLOTTER;       // pour le moniteur série
static int outputType = PROCESSING_VISUALIZER;  // pour le moniteur sous Processing

//Initialisation des ports d'envoie TX et de reception RX du module BLE de l'Arduino
SoftwareSerial BTserial(RX_BLE,TX_BLE); 

//Initialisation des ports d'envoie TX et de reception RX pour la communication avec la Newton (connexion filaire)
SoftwareSerial Newton(RX_Newton,TX_Newton);

void setup(){
  
  Newton.begin(57600);             //Initialisation de la communication série entre Newton et Arduino (connexion filaire)

  BTserial.begin(9600);           //Initialisation de la communication série entre module BLE et Arduino
  Serial.println("Bluetooth Ready !");    
  
  interruptSetup();                 

  //Demarrage, sur la Newton, du script de sauvegarde des valeurs de frequences cardiaques 
  Newton.println("su root");
  Newton.println("su -c sh \"/data/scriptFiltre.sh\"");
  
}


void loop(){
  
  if (QS == true){                         // A Heartbeat Was Found
        serialOutputWhenBeatHappens();    // A Beat Happened, Output that to serial.
        QS = false;                       // reset the Quantified Self flag for next time
  }
  delay(20);                             //  take a break
}

