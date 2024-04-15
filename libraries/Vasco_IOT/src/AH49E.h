/*
 * Read data on AH49E hall sensor
 * Ref: https://www.gotronic.fr/pj2-sen-ky035-manual-1934.pdf
 * Doc: https://www.diodes.com/assets/Datasheets/AH49E.pdf
 * 
 * Implementation:
 *
 *   #include <AH49E.h>
 *
 *   AH49E gauss(35); 
 *
 *   void setup() {
 *     gauss.init();
 *   }
 * 
 *   void loop() {
 *     Serial.println(gauss.getMaxValue().value);
 *   }
 */

#ifndef AH49E_h
#define AH49E_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"


class AH49E : public Device, public Sensor, public AdcPin
{
  static const int  _ADC_MAX_VALUE = 4095;
  static const int  _ADC_ZERO_VALUE = 1652;

  public:

    AH49E(byte pin) : Device(GAUSS_SENSOR), Sensor(true) { _analogPin = pin; }
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    float   read();
    // data updated
    vfield  getMaxValue() { return _data.maxValue; }

  private:
  
    byte    _analogPin;
    float   _maxValue = 0;

    // human readable
    struct fields {
      vfield   maxValue = {"Magnétisme", "%", 1.0};
    };
    fields _data;

    vlegend _maxValues[5] = {
      {0, VERT, "aucun champ"},
      {20, VERT, "champ faible"},
      {40, JAUNE, "champ moyen"},
      {80, ORANGE, "champ fort"},
      {100, ROUGE, "champ maximum"},
    };
};

#endif