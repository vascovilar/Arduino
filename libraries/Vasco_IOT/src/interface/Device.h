#ifndef Device_h
#define Device_h

#include "Arduino.h"

static const byte VDEVICE_COUNT = 10;
    
enum vdevice {
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
};

struct vpointer {
  int   x;
  int   y;
  bool  focus;
  bool  click;
  bool  left;
  bool  right;
  bool  up;
  bool  down;
};


class Device
{
  public:

    Device(vdevice code) { _deviceCode = code; }
    virtual bool  init() = 0; // init device
    virtual bool  wake() = 0; // make device wake up
    virtual bool  sleep() = 0; // make device sleep and consuming less possible
    virtual bool  check() = 0; // played each turn, return true to force update in trigger mode
    virtual bool  update() = 0; // played sometimes to get all data from device then store in local registry
    vdevice       getDeviceCode() { return _deviceCode; }
    
  protected:
  
    vdevice   _deviceCode;
    
};

#endif
