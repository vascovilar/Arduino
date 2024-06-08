#ifndef Timer_h
#define Timer_h

#include "Arduino.h"


class Timer
{
  public:

    bool    isTime(int delay); // in ms

  protected:

    long    _timer = 0;

};

#endif
