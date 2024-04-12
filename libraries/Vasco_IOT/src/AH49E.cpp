#include "AH49E.h"

bool AH49E::init()
{
  if (!_initADC(_analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog EMF device"));
    return false;
  }

  return true;
}

bool AH49E::wake()
{
  return true;
}

bool AH49E::sleep()
{ 
  return true;
}

bool AH49E::check()
{
  float value = _readADC();
  if (value > _maxValue) {
    _maxValue = value;
  }
  // detecting anything
  return _maxValue > 0;
}

bool AH49E::update()
{
  _feed(_data.maxValue, _maxValue, _maxValues, 5);
  _maxValue = 0;

  return true;
}

long AH49E::read()
{
  return _rawADC();
}