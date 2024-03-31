/*
 * Read uv index and visible light with LTR390
 * Implementation:
 *
 *   #include <VSensorLTR390.h>
 *
 *   VSensorLTR390 sun(0); 
 *
 *   void setup() {
 *     sun.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(sun.getUvIndex().value);
 *   }
 */

#ifndef VSensorLTR390_h
#define VSensorLTR390_h

#include "Arduino.h"
#include "VDevice.h"
#include "VSensor.h"
#include "Adafruit_LTR390.h"
#include "Wire.h"

class VSensorLTR390 : public VDevice, public VSensor, public VI2CPins
{
  public:

    VSensorLTR390(byte pin) : VDevice(ADA_LTR390), VSensor(pin) {};
 
    bool init();
    bool wake();
    bool sleep();
    bool sync();
    bool event();
    float check();

    field_data   getUvIndex() { return _data.uvIndex; }
    field_data   getVisible() { return _data.visible; }

  private:
  
    Adafruit_LTR390 _ltr = Adafruit_LTR390();

    struct fields {
      field_data   uvIndex = {"Index UV", "", 1.0};
      field_data   visible = {"Lumière visible", "lux", 10.0};
    };
    fields _data;

    legend_data _uvIndexes[5] = {
      {2, VERT, "risque faible"},
      {5, JAUNE, "risque modéré"},
      {7, ORANGE, "risque élevé"},
      {10, ROUGE, "risque très élevé"},
      {12, VIOLET, "risque extrême"},
    };

    legend_data _visibles[10] = {
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
