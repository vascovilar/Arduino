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
//SEN0487       ear(34);
BMI160X       imu(0x68);


// arduino init

int timer = millis();
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  Wire.begin(21, 22); // for IMU
  
  // -----------------------------------------------------
  // TODO
  // -----------------------------------------------------

  //ear.init();
  imu.init();

  // -----------------------------------------------------
}

// arduino run

void loop() 
{
  // put your main code here, to run repeatedly:

  // -----------------------------------------------------
  // TODO
  // -----------------------------------------------------
  
  /*
  ear.update();
  Serial.println(String("1700," + String(ear.read()) + ",1740"));
  */

  imu.update();
  vcoord coord = imu.getTest();
  Serial.println(String(coord.x) + "," + String(coord.y) + "," + String(coord.z));

  // -----------------------------------------------------
  //Serial.print(String((millis() - timer)) + "ms");
  //Serial.println("");
  //timer = millis();
  //delay(1000);
}
