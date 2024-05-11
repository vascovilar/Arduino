#ifndef Run_h
#define Run_h

#include "Arduino.h"

static const int  VDELAY_LONG_UPDATE = 10000;
static const int  VDELAY_SHORT_UPDATE = 1000;
static const int  VDELAY_CONTINUOUS_UPDATE = 100; // seems continuous with 10 measure per second

static const byte VMODE_COUNT = 5;

enum vrun {
  INIT_ERROR = 0,
  SLEEPING = 1,
  LOW_REFRESH = 2,
  HIGH_REFRESH = 3,
  EVENT_TRIG = 4, // low refresh mode + trigs on event (event = check function returns true)
  CONTINUOUS = 5,
};

class Run
{
  public:

    virtual bool  begin(vrun mode); // init
    virtual bool  run(); // return true if something new

    int           isEnabled() { return _enabled; }
    vrun          getProcessMode() { return _processMode; }
    int           getProcessedTime() { return _processedTime; }

  protected:

    bool    _enabled = false;
    vrun    _processMode = INIT_ERROR;
    int     _processedTime = 0;
};

#endif
