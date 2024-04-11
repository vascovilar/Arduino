#ifndef Buffer_h
#define Buffer_h

#include "Arduino.h"
#include "interface/Data.h"


class Buffer : public Data
{
  static const int  _DELAY_TO_PUSH_TO_HISTORY = 10000; // 180000 = 3 min means 240 mesures = 12h
  static const int  _HISTORY_BUFFER_MAX_SIZE = 240;
  static const byte _TMP_BUFFER_MAX_SIZE = 100;
  
  public:  
    float   history[_HISTORY_BUFFER_MAX_SIZE];   // last values stored
    long    timeline[_HISTORY_BUFFER_MAX_SIZE];  // last dates
    int     length;                              // real size of history
    int     delay = _DELAY_TO_PUSH_TO_HISTORY;
    float   minimum;
    float   maximum;
    float   average;
    float   delta;
    byte    trend;
    bool    push(float value, long timeStamp);
    
  private:
    float   _buffer[_TMP_BUFFER_MAX_SIZE]; // last 100 values stored
    int     _bufferIndex = 0;
    int     _bufferLength = 0;
    long    _timer = 0; 
    void    _pushHistory(float value, long timeStamp);
    void    _pushBuffer(float value);
    float   _popBufferAverageValue();
};


#endif
