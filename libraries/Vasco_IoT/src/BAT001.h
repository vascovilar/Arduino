/*
 * Read electro-magnetic field level with custom sensor (15cm antenna with 10Mohm pulldown resistance on VP pin)
 */

#ifndef BAT001_h
#define BAT001_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "component/Pins.h"


class BAT001 : public Sensor, public AdcPin
{
  static const int  _ADC_MAX_VALUE = 4095;
  static const int  _ADC_ZERO_VALUE = 0;

  public:

    BAT001(byte pin) : Sensor(BATTERY_SENSOR, true) { _analogPin = pin; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch (code) {
        case BAT_LEVEL:
          return _battery;
      }

      return (vfield){};
    }

    // api
    float getPercentage();

  private:

    byte    _analogPin;
    vfield  _battery = {"Batterie", "V", 0.5};

    vlegend _batteries[5] = {
      {3.0, ROUGE, "non fonctionnelle"},
      {3.60, ROUGE, "recharger"},
      {3.83, ORANGE, "faible"},
      {4.10, VERT, "ok"},
      {4.25, VERT, "maximum"},
    };

    struct voltage {
      float    percentage;
      float    volt;
    };

    // https://www.drones-technic.be/wp-content/uploads/2017/09/Conversion-LiPo.jpg
    voltage voltages[11] = {
      {0.0, 3.0},
      {10.0, 3.60},
      {20.0, 3.70},
      {30.0, 3.75},
      {40.0, 3.79},
      {50.0, 3.83},
      {60.0, 3.87},
      {70.0, 3.92},
      {80.0, 3.97},
      {90.0, 4.10},
      {100.0, 4.20},
    };

};

#endif
