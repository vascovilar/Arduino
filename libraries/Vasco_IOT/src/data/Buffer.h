#ifndef Buffer_h
#define Buffer_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../data/Filter.h" // TODO vasco temp to test 


class Buffer : public Data, Filter
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
    // TODO vasco temp to test
    float   smoothe(float value, int factor) { return _smooth(value, factor); }
    float   maximume(float value, int factor) { return _maximum(value, factor); }
    float   inertiae(float value, int factor) { return _inertia(value, factor); }

    
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
