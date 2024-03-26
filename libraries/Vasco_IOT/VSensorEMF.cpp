#include "VSensorEMF.h"

bool VSensorEMF::init()
{
  if (_initADC(analogPin, true, _ADC_MAX_VALUE, _ADC_ZERO_VALUE, _ADC_SMOOTH_FACTOR)) {

    return true;
  }

  Serial.println(F("Error initializing analog EMF device"));

  return false;
}

bool VSensorEMF::wake()
{
  return true;
}

bool VSensorEMF::sleep()
{ 
  return true;
}

bool VSensorEMF::sync()
{
  _feed(_data.maxValue, maxValue, _maxValues, 5);
  _feed(_data.frequency, _readADCFrequency(), _frequencies, 2);

  return true;
}

bool VSensorEMF::event()
{
  // detecting anything
  return maxValue > 0;
}

float VSensorEMF::check() 
{ 
  return _readADC(); 
}