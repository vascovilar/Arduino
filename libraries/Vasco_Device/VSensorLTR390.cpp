#include "VSensorLTR390.h"

void VSensorLTR390::begin(int addr)
{
  if (addr == 0x53) {
    _ltr = Adafruit_LTR390();

    _ltr.begin();
    _ltr.setGain(LTR390_GAIN_3);
    _ltr.setResolution(LTR390_RESOLUTION_16BIT);
    _ltr.setThresholds(100, 1000);
    //_ltr.configInterrupt(true, LTR390_MODE_UVS);
  }
}

bool VSensorLTR390::update(int delay)
{
  unsigned int delta = millis() - _timer;

  if (delta > delay) {
    _timer = millis();

    sync();
    
    return true;
  }
  
  return false;
}

void VSensorLTR390::sync()
{
  unsigned int time = millis();
  
  _ltr.setMode(LTR390_MODE_UVS);
  _setUvIndex(_ltr.readUVS());
  _ltr.setMode(LTR390_MODE_ALS);
  _setVisible(_ltr.readALS());

  _processTime = millis() - time;
}