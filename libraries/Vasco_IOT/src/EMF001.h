/*
 * Read electro-magnetic field level with custom sensor (15cm antenna with 10Mohm pulldown resistance on VP pin)
 */

#ifndef EMF001_h
#define EMF001_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "inherit/Pins.h"


class EMF001 : public Sensor, public AdcPin
{
  static const int  _ADC_MAX_VALUE = 4095;
  static const int  _ADC_ZERO_VALUE = 0;
  const float       _ADC_ZERO_THRESOLD = 2.0;

  public:

    EMF001(byte pin) : Sensor(EMF_SENSOR, true) { _analogPin = pin; }

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
        case EMF_LEVEL:
          return _maxValue;
      }

      return (vfield){};
    }

    // api
    int     raw();
    float   frequency();


  private:

    byte    _analogPin;
    float   _maxValueBuffer = 0;
    vfield  _maxValue = {"Intensité EMF", "%", 2};
    vfield  _frequency = {"Fréquence EMF", "Hz", 10};

    vlegend _maxValues[5] = {
      {0, VERT, "aucun champ"},
      {20, VERT, "champ faible"},
      {40, JAUNE, "champ moyen"},
      {80, ORANGE, "champ fort"},
      {100, ORANGE, "champ maximum"},
    };

    vlegend _frequencies[2] = {
        {0, VERT, "non detectée"},
        {40000, VERT, ""},
      };

};

#endif
