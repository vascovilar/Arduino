#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "interface/Run.h"
#include "interface/Device.h"


class Sequencer : public Run
{
  public:

    Sequencer(Device &obj) : child(obj) {}; 
    // interface
    bool    begin(vrun mode);
    bool    run();
    // data updated
    int     getProcessedTime() { return _processedTime; }
    int     getProcessedChecks() { return _processedChecks; }
    int     getProcessedUpdates() { return _processedUpdates; }

  private:
  
    Device  &child;
    bool    _enabled = false;
    int     _processedTime = 0;  
    int     _processedChecks = 0;  
    int     _processedUpdates = 0;  
    long    _timer = 0;
    long    _time = 0;
    int     _checks = 0;
    int     _updates = 0;
};

#endif
