#include "BAT001.h"

bool BAT001::init()
{
  if (!_initADC(_analogPin, true, _ADC_MAX_VALUE, _ADC_ZERO_VALUE)) {
    Serial.println(F("Error initializing analog BAT001 device"));

    return false;
  }

  return true;
}

bool BAT001::sleep()
{
  return true;
}

bool BAT001::wake()
{
  return true;
}

bool BAT001::check()
{
  return false;
}

bool BAT001::update()
{
  feed(_battery, read(), _batteries, 5);

  return true;
}

float BAT001::read()
{
  float Vbat = 0;
  for (int i = 0; i < 16; i++) {
    Vbat += readAnalogVoltage();
  }

  return 2 * Vbat / 16.0 / 1000.0; // attenuation 2 (divisor bridge 2x 220kOhm)
}

float BAT001::getPercentage()
{
  float input = _battery.value;
  float output = 0;

  for (int i = 0; i < 11; i ++) {
    if (input <= voltages[i].volt) {
      float ratio = (voltages[i].volt - input) / (voltages[i].volt - voltages[i - 1].volt);
      output = voltages[i].percentage - ratio * (voltages[i].percentage - voltages[i - 1].percentage);
      break;
    }
  }

  return output;
}
