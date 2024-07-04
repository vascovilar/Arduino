#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "../interface/Run.h"
#include "../component/Timer.h"

#include "../interface/Chipset.h"


class Sequencer : public Run, public Timer
{
  public:

    Sequencer(Chipset &obj) : _child(obj) {};

    // interface
    bool    begin(vrun mode);
    bool    run();

    // api
    void    pause();
    void    resume();

    // updated local data
    int     getProcessedChecks() { return _processedChecks; }
    int     getProcessedUpdates() { return _processedUpdates; }
    vrun    getCurrentMode() { return _processMode; }
    int     getCurrentDelay() { return _currentDelay; }
    bool    popSomethingNew(); // as soon as true, flaged false on call

  private:

    Chipset  &_child; // by  reference
    vrun    _processMode = INIT_ERROR;
    vrun    _processModeSave = INIT_ERROR;
    int     _processedChecks = 0;
    int     _processedUpdates = 0;
    int     _currentDelay = 0;
    long    _timeBuffer = 0;
    int     _checksBuffer = 0;
    int     _updatesBuffer = 0;
    bool    _isSomethingNew = false;

};

#endif
