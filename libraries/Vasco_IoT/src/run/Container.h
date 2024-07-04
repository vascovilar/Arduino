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

struct vcodes {
  vchipset  chipsetCodes[VCHIPSET_COUNT];
  int       chipsetLength;
  vsensor   sensorCodes[VSENSOR_COUNT];
  int       sensorLength;
};


class Container : public Run, public Rtc, public Timer
{
  public:

    Container() {
      /*if (_initPSRAM()) {
        _bindPSRAM(*_buffer, 30000);
      }*/
    }

    ~Container() {
      // freePSRAM(_buffer);
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
    bool        isSensor(vchipset code);
    bool        isRealTime(vsensor code);
    int         getCurrentDelay(vchipset code);
    int         getCurrentDelay(vsensor code);

    // get from updated local vars
    vcodes      getBindedCodes();
    vfield      getField(vsensor code) { return _field[code]; }
    void        setField(vsensor code, vfield field);
    Buffer      getBuffer(vsensor code) { return _buffer[code]; }
    int         getProcessedChecks() { return _processedChecks; }
    vdatatables getChipsetDataTable(vchipset* chipsetCodes, int chipsetLength);
    vdatatables getSensorDataTable(vsensor* sensorCodes, int sensorLength);
    vfield      getWorstField();

    // public getter/setter for page management
    vpage       getCurrentPage() { return _currentPage; }
    void        setCurrentPage(vpage page) { _currentPage = page; }
    vsensor     getCurrentEvent() { return _currentEvent; }
    void        setCurrentEvent(vsensor code) { _currentEvent = code; }
    vsensor     getCurrentSensor() { return _currentSensor; }
    void        setCurrentSensor(vsensor code) { _currentSensor = code; }

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

    void        _updateLocalSensorsValues(vchipset code); // update local vars
};

#endif
