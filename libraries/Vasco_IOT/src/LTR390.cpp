#include "LTR390.h"


bool LTR390::init()
{
  if (_i2cAddress != 0x53) {
    Serial.println(F("Error LTR390 device use I2C address 0x53"));

    return false;
  }

  if (!_ltr.begin()) {
    Serial.println(F("Error initializing I2C LTR390 device"));

    return false;
  }

  _ltr.setGain(LTR390_GAIN_3);
  _ltr.setResolution(LTR390_RESOLUTION_18BIT);
  _ltr.setThresholds(100, 1000);
  //_ltr.configInterrupt(true, LTR390_MODE_UVS);

  _ltr.setMode(LTR390_MODE_ALS); // by default for check function facilities
  delay(50);

  return true;
}

bool LTR390::sleep()
{
  return true;
}

bool LTR390::wake()
{
  return true;
}

bool LTR390::check()
{
  float value = read();
  if (value > _maxValueBuffer) {
    _maxValueBuffer = value;
  }
  // under sky
  return _maxValueBuffer > 10000;
}

bool LTR390::update()
{

  _ltr.setMode(LTR390_MODE_UVS);
  delay(200); // need time delay to works
  while (!_ltr.newDataAvailable());
  _feed(_data.uvIndex, _convertToIndexUV(_ltr.readUVS()), _uvIndexes, 5);

  _ltr.setMode(LTR390_MODE_ALS); // end with ALS to be ready in case of check function call
  delay(50);
  while (!_ltr.newDataAvailable());
  _feed(_data.visible, _maxValueBuffer, _visibles, 10);
  _maxValueBuffer = 0;

  return true;
}

float LTR390::read() {
  return _convertToLux(_ltr.readALS());
}

float LTR390::_convertToLux(float visible)
{
  // return Lux value for 18bit resolution and x3 gain
  return (0.6 * visible) / 3.0;
}

float LTR390::_convertToIndexUV(float(uvs))
{
  //fix formula for 18bit/x3 https://github.com/esphome/issues/issues/4380
  float sensitivity = 2300 * (3.0/18.0) * (100.0/400.0); // default 100ms for INT_TIME, sensitivity = 95.8333333
  return uvs / sensitivity;
}
