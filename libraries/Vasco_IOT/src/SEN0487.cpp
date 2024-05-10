#include "SEN0487.h"

bool SEN0487::init()
{
  if (!_initADC(_analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog SEN0487 device"));

    return false;
  }

  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);

  delay(500);
  if (_rawADC() == 0) {
    Serial.println(F("Error SEN0487 device not responding"));

    return false;
  }

  return true;
}

bool SEN0487::sleep()
{
  digitalWrite(26, LOW);

  return true;
}

bool SEN0487::wake()
{
  digitalWrite(26, HIGH);

  return true;
}

bool SEN0487::check()
{
  // must read continuously to listen for the slightest change in value
  float value = _readADC();

  // noise reduction
  if (value < _ADC_ZERO_THRESOLD) {
    value = 0.0;
  }

  // store max
  if (value > _maxValueBuffer) {
    _maxValueBuffer = value;
  }

  // detecting anything over noise level
  return _maxValueBuffer > _EVENT_THRESOLD_VALUE;
}

bool SEN0487::update()
{
  // sensor class values
  _feed(_data.maxValue, _maxValueBuffer, _maxValues, 6);

  // reset max value buffer for another round
  _maxValueBuffer = 0;

  return true;
}

float SEN0487::read()
{
  return _rawADC();
}
