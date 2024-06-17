#include "Timer.h"

bool Timer::isTime(int delay)
{
  if (millis() - _timer >= delay) {
    _timer = millis(); // reset timer

    return true;
  }

  return false;
}