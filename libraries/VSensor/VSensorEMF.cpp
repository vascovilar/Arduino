#include "VSensorEMF.h"

void VSensorEMF::begin(int antennaPin)
{
  _antennaPin = antennaPin;
   
  pinMode(_antennaPin, INPUT);
}

bool VSensorEMF::update(int delay)
{
  // get data every 100 ms
  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer

    _data.gauss = _read();

    return true;
  }

  return false;
}

float VSensorEMF::_read()
{
  float val = analogRead(_antennaPin);

  return val;
}
