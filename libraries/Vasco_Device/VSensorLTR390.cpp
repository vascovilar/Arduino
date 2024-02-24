#include "VSensorLTR390.h"

void VSensorLTR390::begin(int addr)
{
  if (addr == 0x53) {
    _ltr = Adafruit_LTR390();
    _ltr.begin();
    _ltr.setGain(LTR390_GAIN_3);
    _ltr.setResolution(LTR390_RESOLUTION_18BIT);
    _ltr.setThresholds(100, 1000);
    //_ltr.configInterrupt(true, LTR390_MODE_UVS);
    _enabled = true;
  }
}

bool VSensorLTR390::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis();

    sync();
    
    return true;
  }
  
  return false;
}

void VSensorLTR390::sync()
{
  unsigned int time = millis();
  
  if (_enabled) {
    _setUvIndex(_readUVS());
    _setVisible(_readALS());
  }

  _data.processTime = millis() - time;
}

void VSensorLTR390::sleep(bool isSleeping)
{
  _ltr.enable(!isSleeping);
  _enabled = _ltr.enabled();

  if (!_enabled) {
    _data.uvIndex.status = GRIS;
    _data.visible.status = GRIS;
  }
}

float VSensorLTR390::_readUVS()
{
  // for 18bit resolution and x3 gain : no doc found about formula
  _ltr.setMode(LTR390_MODE_UVS);

  return _ltr.readUVS();
}

float VSensorLTR390::_readALS()
{
  // for 18bit resolution and x3 gain
  _ltr.setMode(LTR390_MODE_ALS);
  
  return (float) (0.6 * _ltr.readALS()) / (float) 3;
}