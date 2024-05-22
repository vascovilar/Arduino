#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../interface/Chipset.h"

static const byte VSENSOR_COUNT = 16;

enum vsensor {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  GAS_PERCENTAGE = 4,
  EAR_LEVEL = 5,
  EMF_LEVEL = 6,
  GAUSS_LEVEL = 7, // TODO vasco
  INFRARED = 8, // TODO vasco
  VISIBLE = 9,
  UV_INDEX = 10,
  GAMMA_LEVEL = 11, // TODO vasco
  ALTITUDE = 12, // TODO vasco
  MOVEMENT = 13,
  MEMORY_USED = 14,
  RUN_CYCLES = 15,
};

class Sensor : public Chipset
{
  public:

    Sensor(vchipset code, bool isRealtime) : Chipset(code, true) { _isRealtime = isRealtime; }

    virtual float   read(); // read instant sensor raw value out of processtime incretementation, or return 0 if non applicable
    virtual vfield  get(vsensor code); // get buffered value, updated with "update" method

    bool   isRealTime() { return _isRealtime; } // if so get access to read() realtime raw data
    void   feed(vfield &field, float value, vlegend* data, int length)
    {
      field.value = value;
      for (int i = 0; i < length; i ++) {
        if (value <= data[i].limit || i == length - 1) {
          field.status = data[i].status;
          field.text = data[i].text;
          break;
        }
      }
    }

  private:

    bool    _isRealtime;
};

#endif
