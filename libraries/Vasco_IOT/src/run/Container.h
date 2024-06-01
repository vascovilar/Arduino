#ifndef Container_h
#define Container_h

#include "Arduino.h"
#include "interface/Run.h"
#include "interface/Sensor.h"
#include "interface/Device.h" TODO vasco clean des dépendances
#include "data/Buffer.h"
#include "run/Sequencer.h"
#include "component/Rtc.h"
#include "component/Psram.h"


class Container : public Run, public Rtc, public Psram
{
  public:

    // interface
    bool        begin(vrun mode);
    bool        run(); // to run every cycle is better

    // api
    void        bind(Device &device, vrun mode = INIT_ERROR); // to call before begin function
    void        bind(Sensor &device, vrun mode = INIT_ERROR); // to call before begin function
    bool        changed(vchipset chipset);
    void        pause();
    void        resume();

    // get from instances
    bool        isEnabled(vchipset code);

    // get from local vars
    vfield      getField(vsensor code);
    void        setField(vsensor code, vfield field);
    Buffer      getBuffer(vsensor code);

    // public getter/setter for page management
    vpage       getCurrentPage();
    void        setCurrentPage(vpage page);
    vsensor     getCurrentArgument();
    void        setCurrentArgument(vsensor argument);

  private:

    Chipset*    _instance[VCHIPSET_COUNT];
    Sequencer*  _sequencer[VCHIPSET_COUNT];
    vrun        _initialMode[VCHIPSET_COUNT];
    vfield      _field[VSENSOR_COUNT];
    Buffer      _buffer[VSENSOR_COUNT];
    vpage       _currentPage = NONE;
    vsensor     _currentArgument = TEMPERATURE;

    void _updateLocalSensorsValues(vchipset code); // update local vars
};

#endif
