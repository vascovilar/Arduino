#include "VSensorSEN0487.h"

bool VSensorSEN0487::init()
{
  if (!_initADC(_analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
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

bool VSensorSEN0487::check() 
{ 
  float value = read();
  if (value > _maxValue) {
    _maxValue = value;
  }

  // detecting anything over noise level
  return _maxValue > _NOISE_THRESOLD_VALUE; 
}

bool VSensorSEN0487::update()
{
  _feed(_data.maxValue, _maxValue, _maxValues, 6);
  _maxValue = 0;

  return true;
}

float VSensorSEN0487::read()
{
  return _readADC();
}
