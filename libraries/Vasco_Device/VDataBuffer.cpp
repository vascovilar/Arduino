#include "VDataBuffer.h"

bool VDataBuffer::push(float value)
{ 
  _pushBuffer(value);

  if (millis() - _timer > 180000) { // 180000 = 3 min, 240 mesures = 12h 
    _timer = millis(); // reset the timer
    
    _pushHistory(_getBufferAverageValue());

    return true;
  }

  return false;
}

void VDataBuffer::_pushHistory(float value)
{
  float total = 0;

  _countHistory++;
  if (_countHistory > 240) {
    _countHistory = 240;
  }

  _data.minimum = 9999999;
  _data.maximum = 0;
  for (int i = _countHistory - 1; i >= 0 ; i--) 
  {
    // fifo = push value in buffer by sliding others from end to start index
    if (i == 0) {
      _data.history[i] = value;
    } else {
      _data.history[i] = _data.history[i-1];
    }

    if (_data.history[i] > _data.maximum) { 
      _data.maximum = _data.history[i]; 
    }
    if (_data.history[i] < _data.minimum) { 
      _data.minimum = _data.history[i]; 
    }
    total += _data.history[i];
  }

  // calc
  _data.average = (float) total / (float) _countHistory;
  _data.delta = _data.maximum - _data.minimum;
}

void VDataBuffer::_pushBuffer(float value)
{
  float total = 0;
  float average = 0;

  _data.value = value;

  _countBuffer++;
  if (_countBuffer > 100) {
    _countBuffer = 100;
  }

  for (int i = _countBuffer - 1; i >= 0 ; i--) 
  {
    // fifo = push value in buffer by sliding others from end to start index
    if (i == 0) {
      _data.buffer[i] = value;
    } else {
      _data.buffer[i] = _data.buffer[i-1];
    }

    total += _data.buffer[i];
  }

  // calc 
  average = (float) total / (float) _countBuffer;
  _data.trend = 0;
  if (average > _data.average) { 
    _data.trend = 1; 
  }
  if (average < _data.average) { 
    _data.trend = -1; 
  }
}

float VDataBuffer::_getBufferAverageValue()
{
  float total = 0;
  int   count = 0;
  
  for (int i = 0; i < 100; i++) { 
    if (i < _countBuffer) {
      total += _data.buffer[i];
      count++;
    } else {
      // clean other values
      _data.buffer[i] = 0;
    }
  }

  _countBuffer = 0;

  return (float) total / (float) count;
}
