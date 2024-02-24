#include "VSensorESP32.h"

void VSensorESP32::begin()
{

}

bool VSensorESP32::update(int delay)
{
  unsigned int delta = millis() - _timer;

  if (delta > delay) {
    _timer = millis();

    sync();

    _setLoad(((float) (delta - delay) / (float) delay) * 100);
    
    return true;
  }
  
  return false;
}

void VSensorESP32::sync()
{
  unsigned int time = millis();

  if (_enabled) {
    
  }

  _data.processTime = millis() - _timer;
}

void VSensorESP32::sleep(bool isSleeping)
{
  _enabled = !isSleeping;

  if (!_enabled) {
    _data.load.status = GRIS;
  }
}
