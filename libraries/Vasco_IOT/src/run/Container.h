#ifndef Container_h
#define Container_h

#include "Arduino.h"
#include "interface/Run.h"
#include "interface/Sensor.h"
#include "interface/Device.h"
#include "data/Buffer.h"
#include "run/Sequencer.h"
#include "plugin/Rtc.h"


class Container : public Run, public Rtc
{
  public:

    // interface
    bool    begin(vrun mode);
    bool    run();

    // api
    void    bind(Device &device, vrun mode = INIT_ERROR); // to call before begin function
    void    bind(Sensor &device, vrun mode = INIT_ERROR); // to call before begin function
    vfield  getField(vsensor code);
    void    setField(vsensor code, vfield field);
    Buffer  getBuffer(vsensor code);
    bool    isEnabled(vchipset code);
    float   getProcessedChecksPerSecond() { return _processedChecksPerSecond; };

    // attributes
    vchipset updated[VCHIPSET_COUNT];
    int     length = 0;

  private:

    Chipset*    _instance[VCHIPSET_COUNT];
    Sequencer*  _sequencer[VCHIPSET_COUNT];
    vrun        _mode[VCHIPSET_COUNT];
    vfield      _field[VSENSOR_COUNT];
    Buffer      _buffer[VSENSOR_COUNT];
    int         _processedChecks = 0;
    int         _processedTimer = millis();
    float       _processedChecksPerSecond = 0;

    void _updateChipsetSensors(vchipset code);

};

#endif
