/*
 * Smart FIFO _data buffer
 * Implementation:
 *   
 *   #include <VScreenMini.h>
 *   #include <VBuffer.h>
 *
 *   VScreenMini view;
 *   VBuffer graph;
 *
 *   void setup() {
 *     view.begin(0x3C);
 *   }
 *   void loop() {
 *     graph.push(random(10,100));
 *     view.clear();
 *     view.graphBox(0, 16, 15, graph._data);
 *     view.display();
 *     delay(1000);
 *   }
 */

#ifndef VBuffer_h
#define VBuffer_h

#include "Arduino.h"

#define BUFFER_ARRAY_LENGTH  120
#define BUFFER_SAMPLE_LENGTH 120
#define BUFFER_SAMPLE_DELAY  60000

struct buffer_data_stat {
  float    value;
  float    minimum;
  float    maximum; 
  float    average;
  float    delta;
};

class VBuffer
{
  public:

    bool push(float value)
    {
      _last = value;
      if (_index < BUFFER_SAMPLE_LENGTH) { // get firsts, forget others
        _sample[_index++] = value;  
      }
      
      if (millis() - _timer > BUFFER_SAMPLE_DELAY) {
        _timer = millis(); // reset the timer
        _pushFifo(_popSample());

        return true;
      }

      return false;
    }

    float* dump()
    {
      float * tmp = _data;
      tmp[0] = _last;

      return tmp;
    }

    void reset()
    {
      memset(_data, 0, sizeof(_data));
    }

    void slice(int from)
    {
      int length = (sizeof(_data) / sizeof(_data[0])) - 1;
      
      for(int i = from; i < length; i++) 
      {
        _data[i] = 0;
      }
    }

    buffer_data_stat stat(float values[BUFFER_ARRAY_LENGTH], int from, int to) 
    {
      buffer_data_stat stat;
      int count = 0;
      float total = 0;
      
      stat.value = values[0];
      stat.maximum = values[0];
      stat.minimum = values[0];

      for (int i = from; i < to; i++) {
        if (values[i] != 0) {
          if (values[i] > stat.maximum) { 
            stat.maximum = values[i]; 
          }
          if (values[i] < stat.minimum) { 
            stat.minimum = values[i]; 
          }
          count++;
          total += values[i];
        }
      }

      stat.average = total / count;
      stat.delta = stat.maximum - stat.minimum;

      return stat;   
    }

  private:

    int _index = 0;
    float _last;
    float _data[BUFFER_ARRAY_LENGTH];
    float _sample[BUFFER_SAMPLE_LENGTH];
    uint32_t _timer = millis();
    
    void _pushFifo(float value)
    {
      int length = (sizeof(_data) / sizeof(_data[0])) - 1;
      
      for(int i = length; i >= 0 ; i--) 
      {
        // push value in buffer by sliding others from end to start index
        if(i == 0) {
          _data[0] = value;
        } else {
          _data[i] = _data[i-1];  
        }
      }
    }

    float _popSample()
    {
      float total = 0;
      int count = 0;

      for(int i = 0; i < _index; i++) {
        total += _sample[i];
        count++;
        // reset
        _sample[i] = 0;
      }

      float average = total / _index;
      // reset
      _index = 0;

      return average;
    }
};


#endif
