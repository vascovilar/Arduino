#ifndef Container_h
#define Container_h

#include "Arduino.h"
#include "interface/Run.h"
#include "interface/Device.h"
#include "run/Sequencer.h"


class Container : public Run
{
  public:

    // interface
    bool    begin(vrun mode);
    bool    run();
    // api
    void    add(Device &device);
    // test
    int     go1();
    bool    go2();
    void    go3();

  private:

    Device*     _instance[VDEVICE_COUNT];
    Sequencer*  _sequencer[VDEVICE_COUNT];
    int         _index = 0;

};

#endif
