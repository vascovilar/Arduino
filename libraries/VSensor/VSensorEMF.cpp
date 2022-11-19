#include "VSensorEMF.h"

void VSensorEMF::begin(int antenna)
{
  if (antenna == 36) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);
  } 
}

bool VSensorEMF::update(int delay)
{
  _max += _read();

  if (millis() - _timer > delay) {
    _timer = millis(); // reset the timer
    _data.gauss = _max / delay;
    _max = 0;

    return true;
  }

  return false;
}

float VSensorEMF::_read()
{
  return adc1_get_raw(ADC1_CHANNEL_0);
}

float VSensorEMF::_denoize()
{
  float total;

  for (int i = 0; i<1000; i++) {
    total += _read(); 
  }

  return total/1000;
}