#include "AH49E.h"

bool AH49E::init()
{
  if (!_initADC(_analogPin, false, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog AH49E device"));

    return false;
  }

  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);

  delay(500);
  if (readAnalogRawValue() == 0) {
    Serial.println(F("Error AH49E device not responding"));

    return false;
  }

  return true;
}

bool AH49E::sleep()
{
  digitalWrite(26, LOW);

  return true;
}

bool AH49E::wake()
{
  digitalWrite(26, HIGH);

  return true;
}

bool AH49E::check()
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

bool AH49E::update()
{
  // sensor class values
  feed(_maxValue, _maxValueBuffer, _maxValues, 5);

  // reset max value buffer for another round
  _maxValueBuffer = 0;

  return true;
}

float AH49E::read()
{
  return readAnalogPercentage() * 10; // 1000Gauss / 100%
}

int AH49E::raw()
{
  return readAnalogRawValue();
}