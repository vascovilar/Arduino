#ifndef VRun_h
#define VRun_h

#include "Arduino.h"

#define VMODE_COUNT  5
enum vmode_code {
  INIT_ERROR = 0,
  SLEEPING = 1,
  LOW_REFRESH = 2,
  HIGH_REFRESH = 3,
  EVENT_TRIG = 4,
};

class VRun
{
  public:

    virtual bool  begin(vmode_code mode); // init 
    virtual bool  run(); // return true if something new

    vmode_code    getProcessMode() { return _processMode; }

  protected: 

    vmode_code    _processMode = INIT_ERROR;
};

#endif
