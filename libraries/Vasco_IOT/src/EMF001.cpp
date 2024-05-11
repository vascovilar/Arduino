#include "EMF001.h"

bool EMF001::init()
{
  if (!_initADC(_analogPin, true, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog EMF001 device"));

    return false;
  }

  return true;
}

bool EMF001::sleep()
{
  return true;
}

bool EMF001::wake()
{
  return true;
}

bool EMF001::check()
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

  // detecting anything
  return _maxValueBuffer > 0;
}

bool EMF001::update()
{
  // sensor class values
  _feed(_maxValue, _maxValueBuffer, _maxValues, 5);
  _feed(_frequency, _readADCFrequency(), _frequencies, 2);

  // reset max value buffer for another round
  _maxValueBuffer = 0;

  return true;
}

float EMF001::read()
{
  return _rawADC();
}