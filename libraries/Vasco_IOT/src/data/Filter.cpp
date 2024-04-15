#include "Filter.h"

int Filter::_isometric(float value, float maximum, float minimum, int height, int offset) 
{
  return int((1 - ((float) (value - minimum) / (float) (maximum - minimum))) * height) + offset;
}

float Filter::_smooth(float value, int factor) 
{
  if (factor >= _BUFFER_MAX_SIZE) {
    Serial.print(F("Can't smmoth, factor must be < "));
    Serial.println(_BUFFER_MAX_SIZE);
    
    return 0;
  }
  
  // moving average algorithm
  _smoothTotal -= _smoothBuffer[_smoothIndex];
  _smoothBuffer[_smoothIndex] = value;
  _smoothTotal += _smoothBuffer[_smoothIndex];
  
  _smoothIndex++;
  if (_smoothIndex >= factor) {
    _smoothIndex = 0;
  }

  return _smoothTotal / (float) factor;
}

float Filter::_maximum(float value, int factor) 
{
  if (factor >= _BUFFER_MAX_SIZE) {
    Serial.print(F("Can't smmoth, factor must be < "));
    Serial.println(_BUFFER_MAX_SIZE);
    
    return 0;
  }

  // get max on last records
  float maximum = 0;
  _maxBuffer[_maxIndex] = value;
  for (int i = 0; i < factor; i++) {
    if (_maxBuffer[i] > maximum) {
      maximum = _maxBuffer[i];
    }
  }

  _maxIndex++;
  if (_maxIndex >= factor) {
    _maxIndex = 0;
  }

  return maximum;
}

float Filter::_inertia(float value, int factor) 
{
  if (factor >= _BUFFER_MAX_SIZE) {
    Serial.print(F("Can't smmoth, factor must be < "));
    Serial.println(_BUFFER_MAX_SIZE);
    
    return 0;
  }

  // inertia algorithm      
  if (value > _inertiaValue) {
    _inertiaValue += (value - _inertiaValue) / (float) factor;
  } else if (value < _inertiaValue) {
    _inertiaValue -= (_inertiaValue - value) / (float) factor;
  }

  return _inertiaValue;
}