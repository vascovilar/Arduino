#include "VSensorEMF.h"

bool VSensorEMF::init()
{
  if (!_initADC(_analogPin, true, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog EMF device"));
    return false;
  }

  return true;
}

bool VSensorEMF::wake()
{
  return true;
}

bool VSensorEMF::sleep()
{ 
  return true;
}

bool VSensorEMF::check()
{
  float value = read();
  if (value > _maxValue) {
    _maxValue = value;
  }
  // detecting anything
  return _maxValue > 0;
}

bool VSensorEMF::update()
{
  _feed(_data.maxValue, _maxValue, _maxValues, 5);
  _feed(_data.frequency, _readADCFrequency(), _frequencies, 2);
  _maxValue = 0;

  return true;
}

float VSensorEMF::read()
{
  return _readADC();
}