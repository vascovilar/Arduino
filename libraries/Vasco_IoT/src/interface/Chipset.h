#ifndef Chipset_h
#define Chipset_h

#include "Arduino.h"

static const byte VCHIPSET_COUNT = 12;

enum vchipset {
  MICRO_CONTROLLER = 0,
  METEO_SENSOR = 1,
  LIGHT_SENSOR = 2,
  GPS_NAVIGATOR = 3,
  TFT_SD_SCREEN = 4,
  MICROPHONE_SENSOR = 5,
  GAUSS_SENSOR = 6,
  EMF_SENSOR = 7,
  GAMMA_SENSOR = 8,
  BUZZER = 9,
  POINTER = 10,
  INERTIAL_UNIT = 11,
};


class Chipset
{
  public:

    Chipset(vchipset code, bool isSensor) { _chipsetCode = code; _isSensor = isSensor; }

    virtual bool  init() = 0; // init device
    virtual bool  sleep() = 0; // make device sleep and consuming less possible
    virtual bool  wake() = 0; // make device wake up
    virtual bool  check() = 0; // played each turn, return true to force update in trigger mode
    virtual bool  update() = 0; // played sometimes to get all data from device then store in local registry

    vchipset      getChipsetCode() { return _chipsetCode; }
    bool          isSensor() { return _isSensor; }

  protected:

    vchipset   _chipsetCode;
    bool       _isSensor;

};

#endif
