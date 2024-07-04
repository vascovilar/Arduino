#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "../interface/Chipset.h"
#include "../interface/Data.h"

static const byte VSENSOR_COUNT = 17;

enum vsensor {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  GAS_PERCENTAGE = 4,
  EAR_LEVEL = 5,
  EMF_LEVEL = 6,
  GAUSS_LEVEL = 7,
  INFRARED = 8,
  VISIBLE = 9,
  UV_INDEX = 10,
  GAMMA_LEVEL = 11,
  ALTITUDE = 12,
  MOVEMENT = 13,
  MEMORY_USED = 14,
  RUN_CYCLES = 15,
  BAT_LEVEL = 16,
};


class Sensor : public Chipset
{
  public:

    Sensor(vchipset code, bool isRealtime) : Chipset(code, true) { _isRealtime = isRealtime; }

    virtual float   read() = 0; // read instant sensor raw value out of processtime incretementation, or return 0 if non applicable
    virtual vfield  get(vsensor code) = 0; // get buffered value, updated with "update" method, allows to know which sensor is in a chipset

    void            feed(vfield &field, float value, vlegend* data, int length);
    bool            isRealTime() { return _isRealtime; } // if so get access to read() realtime raw data

  private:

    bool    _isRealtime;
};

#endif
