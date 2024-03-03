#include "VSensorSEN0487.h"

void VSensorSEN0487::begin(int pin)
{
    _pin = pin;
    _enabled = true;
}

bool VSensorSEN0487::update(int delay)
{
  if (_enabled) {
    // always listen
    float value = read();
    if (value > _max) _max = value;
  }

  if (millis() - _timer > delay) {
    _timer = millis();

    snap();
    
    return true;
  }
  
  return false;
}

void VSensorSEN0487::snap()
{
  unsigned int time = millis();
  
  if (_enabled) {
    unsigned int timer = micros();
     
    _setMaxValue(_max);
    _max = 0;
  }
  
  _processTime = millis() - time;
}

void VSensorSEN0487::sleep(bool isSleeping)
{
  _enabled = !isSleeping;

  if (!_enabled) {
    _data.maxValue.status = GRIS;
  }
}

float VSensorSEN0487::read()
{
  int value = 0;

  if (_enabled) {
    value = max(0, analogRead(_pin) - 1660);
    if (value > 2350) {
      value = 2350;
    }
  }

  return ((float) value / (float) 2340) * 100;
}
