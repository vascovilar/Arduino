#include "EMF001.h"

bool EMF001::init()
{
  if (!_initADC(_analogPin, true, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog EMF001 device"));
    
    return false;
  }

  return true;
}

bool EMF001::wake()
{
  return true;
}

bool EMF001::sleep()
{ 
  return true;
}

bool EMF001::check()
{
  float value = _readADC();
  if (value > _maxValue) {
    _maxValue = value;
  }
  // detecting anything
  return _maxValue > 0;
}

bool EMF001::update()
{
  _feed(_data.maxValue, _maxValue, _maxValues, 5);
  _feed(_data.frequency, _readADCFrequency(), _frequencies, 2);
  _maxValue = 0;

  return true;
}

float EMF001::read()
{
  return _rawADC();
}