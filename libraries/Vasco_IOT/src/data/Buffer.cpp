#include "Buffer.h"

bool Buffer::push(float value, long timeStamp)
{
  _pushBuffer(value);

  if (millis() - _timer > delay) {
    _timer = millis();
    _pushHistory(_popBufferAverageValue(), timeStamp);

    return true;
  }

  return false;
}

void Buffer::_pushHistory(float value, long timeStamp)
{
  float historyTotal = 0;
  float lastValuesTotal = 0;
  float lastValuesAverage = 0;

  length++;
  if (length > _HISTORY_MAX_SIZE) {
    length = _HISTORY_MAX_SIZE;
  }

  minimum = 9999999;
  maximum = 0;
  for (int i = length - 1; i >= 0; i--) {
    // fifo = push value in buffer by sliding others from end to start index
    if (i == 0) {
      history[i] = value;
      timeline[i] = timeStamp;

    } else {
      history[i] = history[i-1];
      timeline[i] = timeline[i-1];
    }

    if (history[i] > maximum) {
      maximum = history[i];
    }
    if (history[i] < minimum) {
      minimum = history[i];
    }
    historyTotal += history[i];

    // get last 10 values to calculate average trending
    if (i < 10) {
      lastValuesTotal += history[i];
    }
  }

  // calc stats
  average = historyTotal / (float)length;
  delta = maximum - minimum;
  lastValuesAverage = lastValuesTotal / 10.0;
  trend = 0;
  if (lastValuesAverage > average) {
    trend = 1;
  }
  if (lastValuesAverage < average) {
    trend = 2;
  }
}

void Buffer::_pushBuffer(float value)
{
  // if there is 6 values: average on these 6 first, if 140 value: average on (buffer max size) values
  _bufferLength++;
  if (_bufferLength >= _TMP_BUFFER_MAX_SIZE) {
    _bufferLength = _TMP_BUFFER_MAX_SIZE;
  }

  _buffer[_bufferIndex] = value;

  _bufferIndex++;
  if (_bufferIndex >= _TMP_BUFFER_MAX_SIZE) {
    _bufferIndex = 0;
  }
}

float Buffer::_popBufferAverageValue()
{
  float bufferValuesTotal = 0;
  float bufferValuesAverage = 0;

  for (int i = 0; i < _bufferLength; i++) {
    bufferValuesTotal += _buffer[i];
  }

  bufferValuesAverage = bufferValuesTotal / (float)_bufferLength;

  _bufferIndex = 0;
  _bufferLength = 0;

  return bufferValuesAverage;
}
