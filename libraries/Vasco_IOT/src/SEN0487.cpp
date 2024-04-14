#include "SEN0487.h"

bool SEN0487::init()
{
  if (!_initADC(_analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog SEN0487 device"));
    
    return false;
  }

  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);

  return true;
}

bool SEN0487::wake()
{
  digitalWrite(26, HIGH);

  return true;
}

bool SEN0487::sleep()
{ 
  digitalWrite(26, LOW);
  
  return true;
}

bool SEN0487::check() 
{ 
  float value = _readADC();
  if (value > _maxValue) {
    _maxValue = value;
  }

  // detecting anything over noise level
  return _maxValue > _NOISE_THRESOLD_VALUE; 
}

bool SEN0487::update()
{
  _feed(_data.maxValue, _maxValue, _maxValues, 6);
  _maxValue = 0;

  return true;
}

float SEN0487::read()
{
  return _rawADC();
}
