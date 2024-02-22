#include "VSensorESP32.h"

void VSensorESP32::begin()
{

}

bool VSensorESP32::update(int delay)
{
  unsigned int delta = millis() - _timer;

  if (delta > delay) {
    _timer = millis();

    _setLoad(((float) _processTime / (float) delta) * 100);
    _processTime = 0;
    
    return true;
  }
  
  return false;
}

void VSensorESP32::addLoad(int time)
{
  _processTime += time;
}