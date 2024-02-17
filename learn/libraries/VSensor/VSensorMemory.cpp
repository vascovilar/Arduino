#include "VSensorMemory.h"

void VSensorMemory::begin()
{
}

bool VSensorMemory::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer

    _data.memoryUsage = _readMemoryUsage();
  
    return true;
  }

  return false;
}

float VSensorMemory::_readMemoryUsage()
{
  return (1 - (ESP.getFreeHeap() / 532480.0)) * 100;
}
