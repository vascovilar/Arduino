#include "VDataBuffer.h"

bool VDataBuffer::push(float value)
{ 
  _pushBuffer(value);

  if (_timer == 0 || millis() - _timer > 60000) {
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

  for(int i = _countHistory - 1; i >= 0 ; i--) 
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
  _data.average = total / (float) _countHistory;
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

  for(int i = _countBuffer - 1; i >= 0 ; i--) 
  {
    // fifo = push value in buffer by sliding others from end to start index
    if (i == 0) {
      _data.buffer[i] = value;
    } else {
      _data.buffer[i] = _data.buffer[i-1];
    }

    // last 10 total
    if (i < 10) {
      total += _data.buffer[i];
    }
  }

  // calc 
  average = total / (float) 10;
  if (average > value) { 
    _data.trend = -1; 
  }
  if (average < value) { 
    _data.trend = +1; 
  }
  if (average == value) { 
    _data.trend = 0; 
  }
}

float VDataBuffer::_getBufferAverageValue()
{
  float total = 0;
  
  for (int i = 0; i < _countBuffer; i++) { 
    total += _data.buffer[i];
  }

  return total / (float) _countBuffer;
}
