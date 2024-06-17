#include "VSensorDht.h"

void VSensorDht::begin(int pin)
{
  _dht.setup(pin, DHTesp::DHT22);
}

float VSensorDht::getTemperature()
{
  return _dht.getTemperature();
}

float VSensorDht::getHumidity()
{
  return _dht.getHumidity();
}

ComfortState VSensorDht::getComfort()
{
  ComfortState cf;

  float cr = _dht.getComfortRatio(cf, getTemperature(), getHumidity());

  return cf;
}
