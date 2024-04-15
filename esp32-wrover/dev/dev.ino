#include "Arduino.h"

#include "ESP32X.h"
#include "BUZ001.h"
#include "ST7789SD.h"
#include "EMF001.h"
#include "LTR390.h"
#include "SEN0487.h"
#include "BME680.h"
#include "PA1010D.h"
#include "AH49E.h"
#include "PIM447.h"
#include "BMI160X.h"
//#include "Wire.h"

// put your include files here:




// instanciations come here




// arduino init

int timer = millis();
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  //Wire.begin(21, 22);
  
  // -----------------------------------------------------
  // TODO
  // -----------------------------------------------------

  

  // -----------------------------------------------------
}

// arduino run

void loop() 
{
  // put your main code here, to run repeatedly:

  // -----------------------------------------------------
  // TODO
  // -----------------------------------------------------



  // -----------------------------------------------------
  //Serial.print(String((millis() - timer)) + "ms");
  //Serial.println("");
  //timer = millis();
  //delay(1000);
}
