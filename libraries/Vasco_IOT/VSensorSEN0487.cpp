#include "VSensorSEN0487.h"

bool VSensorSEN0487::init()
{
  if (!_initADC(analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE, _ADC_SMOOTH_FACTOR)) {
    Serial.println(F("Error initializing analog SEN0487 device"));
    return false;
  }

  return true;
}

bool VSensorSEN0487::wake()
{
  return true;
}

bool VSensorSEN0487::sleep()
{ 
  return true;
}

bool VSensorSEN0487::sync()
{
  _feed(_data.maxValue, maxValue, _maxValues, 6);

  return true;
}

bool VSensorSEN0487::event()
{
  // detecting anything but noise
  return maxValue > _NOISE_THRESOLD_VALUE;
}

float VSensorSEN0487::check() 
{ 
  return _readADC(); 
}
