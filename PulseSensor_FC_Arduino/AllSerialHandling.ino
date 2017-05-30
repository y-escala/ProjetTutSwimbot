/*
  All Serial Handling Code,
  It's Changeable with the 'outputType' variable
  It's declared at start of code.
*/


//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens(){
  switch(outputType){
    case PROCESSING_VISUALIZER:      // find it here https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
      sendDataToSerial('B',BPM);     // send heart rate with a 'B' prefix
      break;

    default:
      break;
  }
}


//Fonction d'envoie des données
void sendDataToSerial(char symbol, int data ){
    //Envoie des données par port USB
    Newton.print(symbol);
    Newton.println(data); 
    //Envoie des données par BLE
    BTserial.println(data); 
 
}


