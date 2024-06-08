#ifndef Run_h
#define Run_h

#include "Arduino.h"

static const int  VRUN_LONG_DELAY = 60000;
static const int  VRUN_SHORT_DELAY = 3000;
static const int  VRUN_SECOND_DELAY = 1000;
static const int  VRUN_AWARE_DELAY = 100; // seems continuous with 10 measure per second
static const int  VRUN_CONTINUOUS_DELAY = 10; // really continuous, better is not use


enum vrun {
  INIT_ERROR = 0, // can't start due to communication error, bad wiring or faulty chipset
  SLEEPING = 1, // device consumption is reduced to minimum
  PAUSE = 2, // paused by container's pause function, remuse previous modes with resume
  LOW_REFRESH = 3,
  HIGH_REFRESH = 4,
  EACH_SECOND = 5,
  EVENT_TRIG = 6, // low refresh mode + trigs on event (event = check function returns true)
  AWARE = 7, // almost continuous + can't be paused by container
};


class Run
{
  public:

    virtual bool  begin(vrun mode) = 0; // init
    virtual bool  run() = 0; // return true if something new

    int           isEnabled() { return _enabled; }
    vrun          getProcessMode() { return _processMode; }
    int           getProcessedTime() { return _processedTime; }

  protected:

    bool    _enabled = false;
    vrun    _processMode = INIT_ERROR;
    int     _processedTime = 0;
};

#endif
