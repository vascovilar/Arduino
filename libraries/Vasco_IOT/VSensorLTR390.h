/*
 * Read uv index and visible light with Adafruit LTR390
 * Ref: https://learn.adafruit.com/adafruit-ltr390-uv-sensor/overview-2
 * Doc: https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf
 * 
 * Implementation:
 *
 *   #include <VSensorLTR390.h>
 *
 *   VSensorLTR390 light(0x53); 
 *
 *   void setup() {
 *     light.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(light.getUvIndex().value);
 *   }
 */

#ifndef VSensorLTR390_h
#define VSensorLTR390_h

#include "Arduino.h"
#include "VUsePins.h"
#include "VDevice.h"
#include "VSensor.h"
#include "Adafruit_LTR390.h"
#include "Wire.h"

class VSensorLTR390 : public VDevice, public VSensor, public VUseI2cPins
{
  public:

    VSensorLTR390(byte addr) : VDevice(LIGHT_SENSOR), VSensor(true) {
      _i2cAddress = addr;
    };
 
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();

    // data updated
    vfield_data   getUvIndex() { return _data.uvIndex; }
    vfield_data   getVisible() { return _data.visible; }

  private:
  
    Adafruit_LTR390 _ltr = Adafruit_LTR390();
    byte     _i2cAddress;
    float    _maxValue = 0;

    struct fields {
      vfield_data   uvIndex = {"Index UV", "", 1.0};
      vfield_data   visible = {"Lumière visible", "lux", 10.0};
    };
    fields _data;

    vlegend_data _uvIndexes[5] = {
      {2, VERT, "risque faible"},
      {5, JAUNE, "risque modéré"},
      {7, ORANGE, "risque élevé"},
      {10, ROUGE, "risque très élevé"},
      {12, VIOLET, "risque extrême"},
    };

    vlegend_data _visibles[10] = {
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

    float _convertToLux(float visible);
};

#endif
