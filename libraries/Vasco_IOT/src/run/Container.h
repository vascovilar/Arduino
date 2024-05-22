#ifndef Container_h
#define Container_h

#include "Arduino.h"
#include "interface/Run.h"
#include "interface/Sensor.h"
#include "interface/Device.h"
#include "data/Buffer.h"
#include "run/Sequencer.h"
#include "inherit/Rtc.h"


class Container : public Run, public Rtc
{
  public:

    // interface
    bool      begin(vrun mode);
    bool      run();

    // api
    void      bind(Device &device, vrun mode = INIT_ERROR); // to call before begin function
    void      bind(Sensor &device, vrun mode = INIT_ERROR); // to call before begin function
    vfield    getField(vsensor code);
    void      setField(vsensor code, vfield field);
    Buffer    getBuffer(vsensor code);
    bool      isEnabled(vchipset code);
    //bool      isReady(vchipset code); // TODO vasco add to chipset interface. To not graph when chipset not functional

    // attributes
    vchipset  updatedChipsets[VCHIPSET_COUNT];
    int       updatedLength = 0;

  private:

    Chipset*    _instance[VCHIPSET_COUNT];
    Sequencer*  _sequencer[VCHIPSET_COUNT];
    vrun        _mode[VCHIPSET_COUNT];
    vfield      _field[VSENSOR_COUNT];
    Buffer      _buffer[VSENSOR_COUNT];

    void _updateChipsetSensors(vchipset code);

};

#endif
