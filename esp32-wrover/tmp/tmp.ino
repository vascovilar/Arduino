#include "Arduino.h"
#include "VDeviceESP32.h"

VDeviceESP32 esp(2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  esp.init();
  //esp.getROMTest();

}

void loop() {
  // put your main code here, to run repeatedly:

}
