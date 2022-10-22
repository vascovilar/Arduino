#include "VSensorBattery.h"

void VSensorBattery::begin(int bridgePin)
{
  _bridgePin = bridgePin;
   
  pinMode(_bridgePin, INPUT);
}

bool VSensorBattery::update(int delay)
{
  // get data every 100 ms
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer

    _data.voltage = _read();

    return true;
  }

  return false;
}

float VSensorBattery::_read()
{
  return 4.2;
}
