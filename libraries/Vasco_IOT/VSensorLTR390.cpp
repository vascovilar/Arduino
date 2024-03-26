#include "VSensorLTR390.h"


bool VSensorLTR390::init()
{
  if (_ltr.begin()) {
    _ltr.setGain(LTR390_GAIN_3);
    _ltr.setResolution(LTR390_RESOLUTION_18BIT);
    _ltr.setThresholds(100, 1000);
    //_ltr.configInterrupt(true, LTR390_MODE_UVS);
    _ltr.setMode(LTR390_MODE_ALS);

    return true;
  }

  Serial.println(F("Error initializing I2C LTR390 device"));
  
  return true;
}

bool VSensorLTR390::wake()
{
  return true;
}

bool VSensorLTR390::sleep()
{ 
  return true;
}

bool VSensorLTR390::sync()
{
  _ltr.setMode(LTR390_MODE_UVS);
  _feed(_data.uvIndex, _ltr.readUVS(), _uvIndexes, 5); 
  _ltr.setMode(LTR390_MODE_ALS);
  _feed(_data.visible, _convertToLux(_ltr.readALS()), _visibles, 10);

  return true;
}

bool VSensorLTR390::event()
{
  // under sky
  return maxValue > 10000;
}

float VSensorLTR390::check() 
{ 
  return _convertToLux(_ltr.readALS());
}

float VSensorLTR390::_convertToLux(float visible)
{ 
  // return Lux value for 18bit resolution and x3 gain
  return (float) (0.6 * visible) / (float) 3;
}