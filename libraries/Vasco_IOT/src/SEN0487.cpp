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
  float value = read();

  // noise reduction
  if (abs(value) < _ADC_ZERO_THRESOLD) {
    value = 0.0;
  }

  // store volume
  _volumeValueBuffer += abs(value);
  _volumeCount++;

  // detecting anything over noise level
  return _volumeValueBuffer > _EVENT_THRESOLD_VALUE;
}

bool SEN0487::update()
{
  // sensor class values
  feed(_volumeValue, _volumeValueBuffer / (float)_volumeCount, _volumeValues, 6);

  // reset volume value buffer for another round
  _volumeValueBuffer = 0;
  _volumeCount = 0;

  return true;
}

float SEN0487::read()
{
  return _readADC();
}

int SEN0487::raw()
{
  return _rawADC();
}
