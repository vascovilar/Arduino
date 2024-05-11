/*
 * Read uv index and visible light with Adafruit LTR390
 * Ref: https://learn.adafruit.com/adafruit-ltr390-uv-sensor/overview-2
 * Doc: https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf
 *
 * Implementation:
 *
 *   #include <LTR390.h>
 *
 *   LTR390 light(0x53);
 *
 *   void setup() {
 *     light.init();
 *   }
 *
 *   void loop() {
 *     Serial.println(light.getUvIndex().value);
 *   }
 */

#ifndef LTR390_h
#define LTR390_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"
#include "Adafruit_LTR390.h"
#include "Wire.h"


class LTR390 : public Sensor, public I2cPins
{
  public:

    LTR390(byte addr) : Sensor(LIGHT_SENSOR, false) { _i2cAddress = addr; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch(code) {
        case UV_INDEX:
          return _uvIndex;
        case VISIBLE:
          return _visible;
      }

      return {};
    }

  private:

    Adafruit_LTR390 _ltr = Adafruit_LTR390();
    byte    _i2cAddress;
    vfield  _uvIndex = {"Index UV", "", 1.0};
    vfield  _visible = {"Lumière ambiante", "lux", 10.0};

    float   _convertToLux(float visible);
    float   _convertToIndexUV(float(uvs));

    vlegend _uvIndexes[5] = {
      {2, VERT, "risque faible"},
      {5, JAUNE, "risque modéré"},
      {7, ORANGE, "risque élevé"},
      {10, ROUGE, "risque très élevé"},
      {12, VIOLET, "risque extrême"},
    };

    vlegend _visibles[10] = {
      {0, ORANGE, "obscurité"},
      {1, JAUNE, "minimum humain"},
      {20, JAUNE, "faible"},
      {100, VERT, "tamisée"},
      {500, VERT, "confortable"},
      {1000, VERT, "bureau"},
      {10000, VERT, "ciel couvert"},
      {50000, VERT, "ciel nuageux"},
      {100000, JAUNE, "ciel d'été"},
      {500000, ORANGE, "trop lumineux"},
    };
};

#endif
