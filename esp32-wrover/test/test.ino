#include "VDeviceSound.h"
#include "VSensorSEN0487.h"
#include "VSensorEMF.h"
#include "VDeviceJST2765.h"


VDeviceSound      sound;
VSensorSEN0487    audio;
VSensorEMF        emf;
VDeviceJST2765    joy;

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  //sound.begin(15);
  //audio.begin(39);
  //emf.begin(36);
  joy.begin(34, 39, 0);
}

void loop() 
{
  //sound.warning();
  //Serial.println(audio.read());
  //Serial.println(emf.read());
  //Serial.println("*");
  //delay(1000);

  if (joy.update(100)) {
    Serial.println(String(joy.getX()) + "    " + String(joy.getY()) + "     " + String(joy.getClick()));
  }
}