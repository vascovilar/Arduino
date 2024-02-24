#ifndef VDataBuffer_h
#define VDataBuffer_h

#include "Arduino.h"

class VDataBuffer
{
  public:

    bool push(float value);

    float   getValue() { return _data.value; } // realtime value
    float*  getBuffer() { return _data.buffer; } // 100 last values pushed
    float*  getHistory() { return _data.history; } // 240 last 24h values
    float   getMinimum() { return _data.minimum; }
    float   getMaximum() { return _data.maximum; }
    float   getAverage() { return _data.average; }
    float   getDelta() { return _data.delta; }
    short   getTrend() { return _data.trend; }
    int     getLength() { return _countHistory; } // size of history
    
  private:

    unsigned int  _countHistory = 0;
    unsigned int  _countBuffer = 0;
    unsigned int _timer = 0;
    
    struct fields {
      float    value;
      float    buffer[100];
      float    history[240];
      float    minimum;
      float    maximum;
      float    average;
      float    delta;
      short    trend;
    };
    fields _data;
    
    void _pushHistory(float value);
    void _pushBuffer(float value);
    float _getBufferAverageValue();
};


#endif
