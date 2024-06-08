#ifndef Buffer_h
#define Buffer_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../component/Psram.h"
#include "../component/Timer.h"


static const int    VBUFFER_PUSH_DELAY = 60000; // 180000 = 3 min means 240 mesures = 12h
static const int    VBUFFER_MAX_SIZE = 240; // 240


class Buffer : public Data, public Psram, public Timer
{
  static const byte _TMP_BUFFER_MAX_SIZE = 60;

  public:

    // use PSRAM for large amount of data
    Buffer() {
      if (_initPSRAM()) {
        _bindPSRAM(history, 1000); // 240 x 4 bytes (32 bit values)
        _bindPSRAM(timeline, 1000); // 240 x 4 bytes (32 bit values)
        _bindPSRAM(_buffer, 1000); // 60 x 2 bytes (16 bit values)
      }
    };

    ~Buffer() {
      //_freePSRAM(history); // TODO vasco bug here
      //_freePSRAM(timeline);
      //_freePSRAM(_buffer);
    };

    //float       history[VBUFFER_MAX_SIZE];   // last values stored
    //long        timeline[VBUFFER_MAX_SIZE];  // last dates
    float*      history; // = (float*)ps_malloc(1000);
    long*       timeline; // = (long*)ps_malloc(1000);
    int         length = 0;                    // real size of history
    float       minimum = 0;
    float       maximum = 0;
    float       average = 0;
    float       delta = 0;
    byte        trend = 0;

    bool        push(float value, long timeStamp);

  private:

    //int    _buffer[_TMP_BUFFER_MAX_SIZE]; // last values stored
    int*        _buffer;
    int         _bufferIndex = 0;
    int         _bufferLength = 0;

    void        _pushHistory(float value, long timeStamp);
    void        _pushBuffer(float value);
    float       _popBufferAverageValue();
};


#endif
