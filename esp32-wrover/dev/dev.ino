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
#include "SPI.h"
#include "data/Bitmap.h"


// instanciations come here

//BMI160X       imu(0x68);
SEN0487         ear(34);
EMF001          emf(36);
ST7789SD        tft(5, 25, 33, 32);
Bitmap          bitmap;

// arduino init

int timer = millis();
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  //Wire.begin(21, 22); // for IMU
  
  // -----------------------------------------------------
  // INIT
  // -----------------------------------------------------

  //imu.init();
  ear.init();
  emf.init();
  tft.init();

  tft.led(true);
  tft.clear();

  //bitmap.handleStaticGraphPage(tft, 0, "EMF", COLOR_BLUE);
  bitmap.handleStaticGraphPage(tft, 0, "EAR", COLOR_BLUE);
  
  // -----------------------------------------------------
}

// arduino run

void loop() 
{
  // put your main code here, to run repeatedly:

  // -----------------------------------------------------
  // LOOP
  // -----------------------------------------------------
  
  /*
  ear.update();
  Serial.println(String("1700," + String(ear.read()) + ",1740"));
  */

  /*
  imu.update();
  vcoord coord = imu.getTest();
  Serial.println(String(coord.x) + "," + String(coord.y) + "," + String(coord.z));
  */

  /*
  emf.update();
  Serial.println(String("0," + String(emf.getMaxValue().value) + ",100"));
  */

  //bitmap.handleRealtimeGraph(tft, emf, 240);
  bitmap.handleRealtimeGraphUpdate(tft, 25, ear, 240);

  // -----------------------------------------------------
  //Serial.print(String((millis() - timer)) + "ms");
  //Serial.println("");
  //timer = millis();
  //delay(1000);
}
