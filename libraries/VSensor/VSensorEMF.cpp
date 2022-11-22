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
    _data.gauss = _max / delay / 100;
    _max = 0;

    return true;
  }

  return false;
}

float* VSensorEMF::snap()
{
  uint32_t time = micros();
  int i = 0;
  int max = 0;

  while (i<100) {
    max += _read();
    if (micros() - time > 500) {
      time = micros();
      _buffer[i++] = max + 0.001; 
      max = 0;
    }
  }

  return _buffer;
}

float VSensorEMF::_read()
{
  return adc1_get_raw(ADC1_CHANNEL_0);
}
