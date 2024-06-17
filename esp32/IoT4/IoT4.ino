/*
 * Environment project
 *
 * Outdoor chipset
 * Including BME680 (meteo), SI1145 (light) and SEN0487 (sound)
 * esp32s3 (https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
 *
 * board: Select XIAO ESP32S3
 * port: /dev/ttyACM1
 * src: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 */


#include "Arduino.h"
#include "ESP32X.h"
#include "SI1145.h"


ESP32X          esp(BUILTIN_LED);
SI1145          light(0x60);


void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");

  esp.init();
  light.init();

  esp.led(false); // inverted
}

void loop()
{
  light.update();
  
  Serial.println("******************");
  Serial.println("infrared: " + String(light.get(INFRARED).value));
  Serial.println("visible: " + String(light.get(VISIBLE).value));
  Serial.println("ultraviolet: " + String(light.get(UV_INDEX).value));

  delay(1000);
}
