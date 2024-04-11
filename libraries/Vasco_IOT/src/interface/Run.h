#ifndef Run_h
#define Run_h

#include "Arduino.h"

static const byte VMODE_COUNT = 5;

enum vrun {
  INIT_ERROR = 0,
  SLEEPING = 1,
  LOW_REFRESH = 2,
  HIGH_REFRESH = 3,
  EVENT_TRIG = 4,
};


class Run
{
  public:
    virtual bool  begin(vrun mode); // init 
    virtual bool  run(); // return true if something new
    vrun          getProcessMode() { return _processMode; }

  protected: 
    vrun    _processMode = INIT_ERROR;
};

#endif
