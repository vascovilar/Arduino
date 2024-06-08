#ifndef Container_h
#define Container_h

#include "Arduino.h"
#include "../interface/Run.h"
#include "../component/Rtc.h"
#include "../component/Timer.h"
#include "../interface/Device.h"
#include "../interface/Sensor.h"
#include "../run/Sequencer.h"
#include "../data/Buffer.h"


class Container : public Run, public Rtc, public Timer
{
  public:

    Container() {
      /*if (_initPSRAM()) {
        _bindPSRAM(*_buffer, 30000);
      }*/
    }

    // interface
    bool        begin(vrun mode);
    bool        run(); // to run every cycle

    // api
    void        bind(Device &device, vrun mode = INIT_ERROR); // to call before begin function
    void        bind(Sensor &device, vrun mode = INIT_ERROR); // to call before begin function
    bool        changed(vchipset code);
    void        pause();
    void        resume();

    // get from instances
    bool        isEnabled(vchipset code);
    int         getCurrentDelay(vsensor code);

    // get from local vars
    vfield      getField(vsensor code);
    void        setField(vsensor code, vfield field);
    Buffer      getBuffer(vsensor code);
    int         getProcessedChecks();


    // public getter/setter for page management
    vpage       getCurrentPage();
    void        setCurrentPage(vpage page);
    vsensor     getCurrentEvent();
    void        setCurrentEvent(vsensor code);
    vsensor     getCurrentSensor();
    void        setCurrentSensor(vsensor code);

  private:

    Chipset*    _instance[VCHIPSET_COUNT];
    Sequencer*  _sequencer[VCHIPSET_COUNT];
    vrun        _initialMode[VCHIPSET_COUNT];
    vfield      _field[VSENSOR_COUNT]; // TODO vasco * put in PSRAM
    Buffer      _buffer[VSENSOR_COUNT];
    vpage       _currentPage = NONE;
    vsensor     _currentEvent = TEMPERATURE;
    vsensor     _currentSensor = TEMPERATURE;
    int         _processedChecks = 0;
    int         _checksBuffer = 0;

    void _updateLocalSensorsValues(vchipset code); // update local vars
};

#endif
