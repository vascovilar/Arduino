#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "../interface/Run.h"
#include "../interface/Device.h"


class Sequencer : public Run
{
  public:

    Sequencer(Device &obj) : child(obj) {};
    // interface
    bool    begin(vrun mode);
    bool    run();
    // additionnal updated data
    int     isEnabled() { return _enabled; }
    int     getProcessedTime() { return _processedTime; }
    int     getProcessedChecks() { return _processedChecks; }
    int     getProcessedUpdates() { return _processedUpdates; }
    int     getCurrentDelay() { return _currentDelay; }

  private:

    Device  &child;
    bool    _enabled = false;
    int     _processedTime = 0;
    int     _processedChecks = 0;
    int     _processedUpdates = 0;
    int     _currentDelay = 0;
    long    _timer = 0;
    long    _timeBuffer = 0;
    int     _checksBuffer = 0;
    int     _updatesBuffer = 0;
};

#endif
