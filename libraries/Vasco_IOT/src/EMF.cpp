#include "EMF.h"

bool EMF::init()
{
  if (!_initADC(_analogPin, true, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog EMF device"));
    return false;
  }

  return true;
}

bool EMF::wake()
{
  return true;
}

bool EMF::sleep()
{ 
  return true;
}

bool EMF::check()
{
  float value = read();
  if (value > _maxValue) {
    _maxValue = value;
  }
  // detecting anything
  return _maxValue > 0;
}

bool EMF::update()
{
  _feed(_data.maxValue, _maxValue, _maxValues, 5);
  _feed(_data.frequency, _readADCFrequency(), _frequencies, 2);
  _maxValue = 0;

  return true;
}

float EMF::read()
{
  return _readADC();
}