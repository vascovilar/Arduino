// SPDX-FileCopyrightText: 2020 Limor Fried for Adafruit Industries
// SPDX-FileCopyrightText: 2020 Kevin Townsend for Adafruit Industries
//
// SPDX-License-Identifier: MIT

/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Adafruit_SSD1306.h>
#include "bsec.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

Bsec iaqSensor;
String output;


void setup() {
  Serial.begin(115200);
  //while (!Serial);
  
  Serial.println(F("BME680 test"));
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("OLED begun");

  display.display();
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  
  iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();
  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
  // Print the header
  output = "Timestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
  Serial.println(output);
}

void loop() {
  display.setCursor(0,0);
  display.clearDisplay();
  Serial.println("*");

  if (! iaqSensor.run()) { // If no data is available
    checkIaqSensorStatus();
    return;
  }

  Serial.println("OK");

  display.print("Temp: "); display.print(iaqSensor.temperature); display.println(" C");
  display.print("Pres: "); display.print(iaqSensor.pressure / 100); display.println(" hPa");
  display.print("Humi: "); display.print(iaqSensor.humidity); display.println(" %");
  display.print("Resi: "); display.print(iaqSensor.gasResistance / 1000, 3); display.println(" kOhm");
  
  display.print("IAQ:  "); display.print(int(iaqSensor.staticIaq)); display.print("-"); 
  float iaq = iaqSensor.staticIaq;
  if (iaq < 50) display.print("GOOD");
  else if (iaq < 100) display.print("AVERAGE");
  else if (iaq < 150) display.print("NOT GOOD");
  else if (iaq < 200) display.print("BAD");
  else if (iaq < 300) display.print("WORSE");
  else display.print("VERY BAD");
  display.println("");
  
  display.print("CO2:  "); display.print(iaqSensor.co2Equivalent); display.println(" ppm");
  display.print("VOC:  "); display.print(iaqSensor.breathVocEquivalent); display.println(" ppm");

  display.display();
  delay(1000);
}


// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      display.setCursor(0,0);
      display.println(output);
      display.display();
      for (;;)  delay(10);
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      display.setCursor(0,0);
      display.println(output);
      display.display();
      for (;;)  delay(10);
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}
