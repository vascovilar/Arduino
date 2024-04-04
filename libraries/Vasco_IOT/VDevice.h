#ifndef VDevice_h
#define VDevice_h

#include "Arduino.h"

#define VDEVICE_COUNT  11
enum vdevice_code {
  MICRO_CONTROLLER = 0,
  METEO_SENSOR = 1,
  LIGHT_SENSOR = 2,
  GPS_NAViGATION = 3,
  TFT_SD_SCREEN = 4,
  MICROPHONE_SENSOR = 5,
  GAUSS_SENSOR = 6,
  TRACKBALL = 7,
  EMF_SENSOR = 8,
  GAMMA_SENSOR = 9,
  BUZZER = 10,
};


class VDevice
{
  public:

    VDevice(vdevice_code deviceCode) { 
      _deviceCode = deviceCode; 
    }

    virtual bool  init() = 0; // init device
    virtual bool  wake() = 0; // make device wake up
    virtual bool  sleep() = 0; // make device sleep and consuming less possible
    virtual bool  check() = 0; // played each turn, return true to force update in trigger mode
    virtual bool  update() = 0; // played sometimes to get all data from device then store in local registry

    vdevice_code  getDeviceCode() { return _deviceCode; }
    
  protected:

    vdevice_code  _deviceCode;
    
};

#endif
