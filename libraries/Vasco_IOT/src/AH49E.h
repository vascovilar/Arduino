/*
 * Read data on AH49E hall sensor
 * Ref: https://www.gotronic.fr/pj2-sen-ky035-manual-1934.pdf
 * Doc: https://www.diodes.com/assets/Datasheets/AH49E.pdf
 */

#ifndef AH49E_h
#define AH49E_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "inherit/Pins.h"


class AH49E : public Sensor, public AdcPin
{
  static const int  _ADC_MAX_VALUE = 4095;
  static const int  _ADC_ZERO_VALUE = 1652;
  const float       _ADC_ZERO_THRESOLD = 1.0;

  public:

    AH49E(byte pin) : Sensor(GAUSS_SENSOR, true) { _analogPin = pin; }

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
        case GAUSS_LEVEL:
          return _maxValue;
      }

      return (vfield){};
    }

    // api
    int     raw();

  private:

    byte    _analogPin;
    float   _maxValueBuffer = 0;
    vfield  _maxValue = {"Champ magnétique", "%", 1};

    vlegend _maxValues[5] = {
      {0, VERT, "aucun champ"},
      {20, VERT, "champ faible"},
      {40, JAUNE, "champ moyen"},
      {80, JAUNE, "champ fort"},
      {100, ORANGE, "champ maximum"},
    };
};

#endif
