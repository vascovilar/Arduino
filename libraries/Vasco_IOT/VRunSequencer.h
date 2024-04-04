#ifndef VRunSequencer_h
#define VRunSequencer_h

#include "Arduino.h"
#include "VRun.h"
#include "VDevice.h"

class VRunSequencer : public VRun
{
  static const int LONG_UPDATE_TIME = 10000;
  static const int SHORT_UPDATE_TIME = 1000;

  public:

    VRunSequencer(VDevice &obj) : child(obj) {}; 
 
    // interface
    bool    begin(vmode_code mode);
    bool    run();
    
    // data updated
    int     getProcessedTime() { return _processedTime; }
    int     getProcessedChecks() { return _processedChecks; }
    int     getProcessedUpdates() { return _processedUpdates; }

  private:
  
    VDevice &child;
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
