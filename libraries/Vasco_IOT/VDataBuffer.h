#ifndef VDataBuffer_h
#define VDataBuffer_h

#include "Arduino.h"

// TODO vasco add dateTime
class VDataBuffer
{
  static const byte _HISTORY_MAX_SIZE = 240;
  static const byte _BUFFER_MAX_SIZE = 100;

  public:

    static const unsigned int _DELAY_TO_PUSH = 180000; // 180000 = 3 min means 240 mesures = 12h

    bool push(float value);

    float   getValue() { return _data.value; } // realtime value
    float*  getHistory() { return _data.history; } // 240 last values for last 24h
    float   getMinimum() { return _data.minimum; }
    float   getMaximum() { return _data.maximum; }
    float   getAverage() { return _data.average; }
    float   getDelta() { return _data.delta; }
    byte    getTrend() { return _data.trend; }
    byte    getLenght() { return _countHistory; } // real size of history
    
  private:

    float         _buffer[_BUFFER_MAX_SIZE]; // last 100 values stored
    byte          _countHistory = 0;
    byte          _countBuffer = 0;
    unsigned int  _timer = 0;
    
    struct fields {
      float    value;
      float    history[_HISTORY_MAX_SIZE];
      float    minimum;
      float    maximum;
      float    average;
      float    delta;
      byte     trend;
    };
    fields _data;
    
    void  _pushHistory(float value);
    void  _pushBuffer(float value);
    float _getBufferAverageValue();
};


#endif
