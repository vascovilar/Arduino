/*
 * Read ambiant sound level with DFRobot SEN0487
 * Ref: https://wiki.dfrobot.com/Fermion_MEMS_Microphone_Sensor_SKU_SEN0487
 * Doc: https://dfimg.dfrobot.com/nobody/wiki/5160bfe4d49deff484e1bd66a44d743a.pdf
 */

#ifndef SEN0487_h
#define SEN0487_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "component/Pins.h"

class SEN0487 : public Sensor, public AdcPin
{
  static const int  _ADC_MAX_VALUE = 4000;
  static const int  _ADC_ZERO_VALUE = 1710;
  const float       _EVENT_THRESOLD_VALUE = 10.0;

  public:

    SEN0487(byte pin) : Sensor(MICROPHONE_SENSOR, true) { _analogPin = pin; }

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
        case EAR_LEVEL:
          return _maxValue;
      }

      return (vfield){};
    }

  private:

    byte     _analogPin;
    float    _maxValueBuffer = 0;
    vfield   _maxValue = {"Vibration air", "%", 5};

    vlegend _maxValues[6] = {
      {0, VERT, "silence"},
      {3, VERT, "calme"},
      {10, VERT, "moyenne"},
      {50, JAUNE, "bruit"},
      {70, ORANGE, "fort"},
      {100, ROUGE, "très fort"},
    };
};

#endif
