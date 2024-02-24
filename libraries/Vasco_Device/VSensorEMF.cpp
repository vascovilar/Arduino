#include "VSensorEMF.h"

void VSensorEMF::begin(int pin)
{
  if (pin == 36) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);
    _enabled = true;
  } 
}

bool VSensorEMF::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis();

    sync();
    
    return true;
  }
  
  return false;
}

void VSensorEMF::sync()
{
  unsigned int time = millis();
  
  if (_enabled) {
    unsigned int timer = micros();
    int          i = 0;
    int          beat = 0;
    float        max = 0;
      
    while (i < 100) {
      if (micros() - timer > 1000) {
        timer = micros();
        
        _data.buffer[i] = _readSensor();
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
  }
  
  _data.processTime = millis() - time;
}

void VSensorEMF::sleep(bool isSleeping)
{
  _enabled = !isSleeping;

  if (!_enabled) {
    _data.maxValue.status = GRIS;
    _data.frequency.status = GRIS;
  }
}

float VSensorEMF::_readSensor()
{
  return ((float) adc1_get_raw(ADC1_CHANNEL_0) / (float) 4096) * 100;
}
