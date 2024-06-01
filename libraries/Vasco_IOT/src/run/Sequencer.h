#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "../interface/Run.h"
#include "../interface/Chipset.h"


class Sequencer : public Run
{
  public:

    Sequencer(Chipset &obj) : _child(obj) {};

    // interface
    bool    begin(vrun mode);
    bool    run();

    // api
    void    pause();
    void    resume();
    bool    isSomethingNew(); // as soon as true, fresh data is no more new

    // updated local data
    int     getProcessedChecks() { return _processedChecks; }
    int     getProcessedUpdates() { return _processedUpdates; }
    int     getCurrentDelay() { return _currentDelay; }

  private:

    Chipset  &_child;
    vrun    _processModeSave;
    int     _processedChecks = 0;
    int     _processedUpdates = 0;
    int     _currentDelay = 0;
    long    _timer = 0;
    long    _timeBuffer = 0;
    int     _checksBuffer = 0;
    int     _updatesBuffer = 0;
    bool    _isSomethingNew = false;

};

#endif
