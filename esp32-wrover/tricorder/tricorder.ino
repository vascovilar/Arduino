#include "VCommon.h"
#include "VSensorEMF.h"

VSensorEMF emf;

void setup() {
  Serial.begin(115200);
  emf.begin(36); 
  delay(1000);
}

void loop() 
{
  /*if (emf.update(1000)) {
    float* buffer = emf.getBuffer();
    for (int i = 0; i<100; i++) {
      Serial.println(buffer[i]);
    } 
  }*/

  delay(1000);
  emf.snap(1000); 
  Serial.print(emf.getFrequency().value);
  Serial.print(emf.getFrequency().unit);
  Serial.print(" in ");
  Serial.print(emf.getMeasureTime());
  Serial.println("ms");
}
