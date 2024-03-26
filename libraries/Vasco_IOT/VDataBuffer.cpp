#include "VDataBuffer.h"

bool VDataBuffer::push(float value)
{ 
  _pushBuffer(value);
  
  if (millis() - _timer > _DELAY_TO_PUSH) { 
    _timer = millis();
    _pushHistory(_getBufferAverageValue());

    return true;
  }

  return false;
}

void VDataBuffer::_pushHistory(float value)
{
  float total = 0;
  float last = 0;
  float average = 0;

  _countHistory++;
  if (_countHistory > _HISTORY_MAX_SIZE) {
    _countHistory = _HISTORY_MAX_SIZE;
  }

  _data.minimum = 9999999;
  _data.maximum = 0;
  for (int i = _countHistory - 1; i >= 0; i--) {
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

    // get last 10 sum to calculate average
    if (i < 10) {
      last += _data.history[i];
    }
  }

  // calc
  _data.average = (float) total / (float) _countHistory;
  _data.delta = _data.maximum - _data.minimum;
  average = (float) last / (float) 10;
  _data.trend = 0;
  if (average > _data.average) { 
    _data.trend = 1; 
  }
  if (average < _data.average) { 
    _data.trend = 2; 
  }
}

void VDataBuffer::_pushBuffer(float value)
{
  _data.value = value;

  _countBuffer++;
  if (_countBuffer > _BUFFER_MAX_SIZE) {
    _countBuffer = _BUFFER_MAX_SIZE;
  }

  for (int i = _countBuffer - 1; i >= 0; i--) {
    // fifo = push value in buffer by sliding others from end to start index
    if (i == 0) {
      _buffer[i] = value;
    } else {
      _buffer[i] = _buffer[i-1];
    }
  }
}

float VDataBuffer::_getBufferAverageValue()
{
  float total = 0;
  byte  count = 0;
  
  for (int i = 0; i < _BUFFER_MAX_SIZE; i++) { 
    if (i < _countBuffer) {
      total += _buffer[i];
      count++;
    } else {
      // clean other values
      _buffer[i] = 0;
    }
  }

  _countBuffer = 0;

  return (float) total / (float) count;
}
