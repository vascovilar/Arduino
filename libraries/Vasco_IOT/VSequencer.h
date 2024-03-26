#ifndef VSequencer_h
#define VSequencer_h

#include "Arduino.h"
#include "VDevice.h"
#include "VSensor.h"

#define VMODE_COUNT  6
enum mode_code {
  INIT_ERROR = 0,
  SLEEPING = 1,
  LOW_REFRESH = 2,
  HIGH_REFRESH = 3,
  EVENT_TRIG = 4,
  ON_DEMAND = 5
};


class VSequencer
{
  static const unsigned int LONG_UPDATE_TIME = 10000;
  static const unsigned int SHORT_UPDATE_TIME = 1000;

  public:

    VSequencer(VSensor &obj) : child(obj) {}; 
 
    void          begin(mode_code mode);
    bool          update();
    float         read();
    mode_code     getProcessMode() { return _processMode; }
    unsigned int  getProcessTime() { return _processTime; }

  private:
  
    VSensor       &child;
    bool          _enabled = false;
    mode_code     _processMode = INIT_ERROR;
    unsigned int  _processTime = 0;  
    unsigned int  _timer = 0;
};

#endif
