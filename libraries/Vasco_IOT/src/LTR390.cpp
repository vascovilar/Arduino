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

  _ltr.setMode(LTR390_MODE_ALS); // by default for check facilities

  return true;
}

bool LTR390::wake()
{
  return true;
}

bool LTR390::sleep()
{ 
  return true;
}

bool LTR390::check()
{
  float value = read();
  if (value > _maxValue) {
    _maxValue = value;
  }

  // under sky
  return _maxValue > 10000;
}

bool LTR390::update()
{
  //_ltr.setMode(LTR390_MODE_UVS); // TODO vasco fix LTR390 get UV
  //_feed(_data.uvIndex, _ltr.readUVS(), _uvIndexes, 5); 
  _ltr.setMode(LTR390_MODE_ALS); 
  _feed(_data.visible, _convertToLux(_ltr.readALS()), _visibles, 10);
  _maxValue = 0;

  return true;
}

float LTR390::read() { 
  return _convertToLux(_ltr.readALS()); 
}

float LTR390::_convertToLux(float visible)
{ 
  // return Lux value for 18bit resolution and x3 gain
  return (float) (0.6 * visible) / 3.0;
}
