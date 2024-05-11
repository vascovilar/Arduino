#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "Data.h"
#include "Chipset.h"

static const byte VSENSOR_COUNT = 14;

enum vsensor {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  GAS_PERCENTAGE = 4,
  EMF_LEVEL = 5,
  UV_INDEX = 6,
  VISIBLE = 7,
  EAR_LEVEL = 8,
  ALTITUDE = 9,
  MOVEMENT = 10,
  GAUSS_LEVEL = 11,
  MEMORY_USED = 12,
  RUN_CYCLES = 13,
};

class Sensor : public Chipset
{
  public:

    Sensor(vchipset code, bool isRealtime) : Chipset(code, true) { _isRealtime = isRealtime; }

    virtual float   read(); // read instant sensor raw value out of processtime incretementation, or return 0 if non applicable
    virtual vfield  get(vsensor code); // get buffered value, update with update method

    bool            isRealTime() { return _isRealtime; } // if so get access to read() realtime raw data

  protected:

    bool    _isRealtime;
    void    _feed(vfield &field, float value, vlegend* data, int length)
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
};

#endif
