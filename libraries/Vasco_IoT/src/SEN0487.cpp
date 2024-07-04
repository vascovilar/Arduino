#include "SEN0487.h"

bool SEN0487::init()
{
  pinMode(_analogPin, INPUT);

  if (!_initADC(_analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog SEN0487 device"));

    return false;
  }

  if (readAnalogValue() == 0) {
    Serial.println(F("Error SEN0487 device not responding"));

    return false;
  }

  return true;
}

bool SEN0487::sleep()
{
  return true;
}

bool SEN0487::wake()
{
  return true;
}

bool SEN0487::check()
{
  // must read continuously to listen for the slightest change in value
  float value = abs(read());

  // store max
  if (value > _maxValueBuffer) {
    _maxValueBuffer = value;
  }

  // detecting anything bigger than thresold
  return value > _EVENT_THRESOLD_VALUE;
}

bool SEN0487::update()
{
  // sensor class values
  feed(_maxValue, _maxValueBuffer, _maxValues, 6);

  // reset max value buffer for another round
  _maxValueBuffer = 0;

  return true;
}

float SEN0487::read()
{
  return readAnalogPercentage();
}
