#include "VSensorEMF.h"
#include "VGearSound.h"

VSensorEMF emf;
VGearSound sound; 

void setup() {
  Serial.begin(115200);
  emf.begin(36); 
  
  //pinMode(33, INPUT_PULLUP);
  pinMode(39, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(15, OUTPUT);
  sound.begin(4);
  sound.open();
  delay(1000);
  digitalWrite(15, LOW);
}

void loop() 
{
/*
  int valH = digitalRead(39);
  int valM = digitalRead(34);
  int valD = digitalRead(35);

  Serial.println(String(valH)+" "+String(valM)+" "+String(valD));
*/
  
  if (emf.update(100)) {
    //sound.note(freq);
    Serial.println(String(emf.getGauss()));
  }
}
