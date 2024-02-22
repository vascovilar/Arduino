#include "VSensorEMF.h"

void VSensorEMF::begin(int pin)
{
  if (pin == 36) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);
  } 
}

bool VSensorEMF::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis();

    sync(1000);
    
    return true;
  }
  
  return false;
}

void VSensorEMF::sync(int delay)
{
  unsigned int timer = micros();
  unsigned int time = millis();
  int          i = 0;
  int          beat = 0;
  float        max = 0;
    
  while (i < 100) {
    if (micros() - timer > delay) {
      timer = micros();
      
      _data.buffer[i] = _read();
      if (_data.buffer[i] > max) {
        max = _data.buffer[i];
      } 
      if (_data.buffer[i] == 0 && _data.buffer[i-1] > 0) {
        beat++;
      }
      i++;
    }
  }

  _setMaxValue(max);
  _setFrequency(beat != 0 ? (float) 1000 / ((float) (millis() - time) / (float) beat): 0);

  _processTime = millis() - time;
}

float VSensorEMF::_read()
{
  return ((float) adc1_get_raw(ADC1_CHANNEL_0) / (float) 4096) * 100;
}
