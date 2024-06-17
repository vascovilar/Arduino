#include "SI1145.h"

bool SI1145::init()
{
  if (_i2cAddress != 0x60) {
    Serial.println(F("Error SI1145 device use I2C address 0x60"));

    return false;
  }

  if (!_uv.begin(_i2cAddress, &Wire)) {
    Serial.println(F("Error initializing I2C SI1145 device"));

    return false;
  }

  return true;
}

bool SI1145::sleep()
{
  return true;
}

bool SI1145::wake()
{
  return true;
}

bool SI1145::check()
{
  return false;
}

bool SI1145::update()
{
  // sensor class values
  feed(_uvIndex, _convertToUVIndex(_uv.readUV()), _uvIndexes, 5);
  feed(_visible, _uv.readVisible() - 260.0, _visibles, 10);
  feed(_infrared, _uv.readIR() - 252.0, _infrareds, 1);

  return true;
}

float SI1145::read()
{
  return 0;
}

float SI1145::_convertToUVIndex(float uv)
{
  return uv / 100.0;
}
