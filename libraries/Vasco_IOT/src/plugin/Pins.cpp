#include "Pins.h"

bool AdcPin::_initADC(byte attachedPin, bool isAmplified, float maxAnalogValue, float zeroAnalogValue) 
{
  if (!(attachedPin >= 32 && attachedPin <= 39)) {
    Serial.println(F("You must use ADC1 (pin 32 to 39) in order to work with ESP32. ADC2 is used by Wifi."));
    return false;
  }
  
  _attachedPin = attachedPin;
  _isAmplified = isAmplified;
  _maxAnalogValue = maxAnalogValue;
  _zeroAnalogValue = zeroAnalogValue;
  
  pinMode(_attachedPin, INPUT);
  analogReadResolution(12); // 4096 values
  analogSetPinAttenuation(_attachedPin, _isAmplified ? ADC_0db: ADC_11db);

  return true;
}

float AdcPin::_readADC() 
{
  int value = analogRead(_attachedPin);

  // float mapping
  if (value > _maxAnalogValue) value = _maxAnalogValue;
  value = value - _zeroAnalogValue;

  // signed % returned
  return ((float) value / (float) (_maxAnalogValue - _zeroAnalogValue)) * 100;
}

float AdcPin::_readADCFrequency()
{
  long time = millis();
  long timer = micros();
  float buffer[100];
  byte i = 0;
  int beat = 0;

  while (i < 100) { // total 100ms to measure
    if (micros() - timer > 1000) {
      timer = micros();
      buffer[i] = _readADC();
      if (buffer[i] == 0 && buffer[i-1] > 0) {
        beat++;
      }
      i++;
    }
  }

  return beat != 0 ? 1000.0 / ((float) (millis() - time) / (float) beat): 0;    
}

bool PwmPin::_initPWM(byte attachedPin, byte channel) 
{
  if ((attachedPin >= 0 && attachedPin <= 5) // for ESP32  // TODO vasco not in array 
    || (attachedPin >= 12 && attachedPin <= 15)
    || (attachedPin >= 18 && attachedPin <= 19)
    || (attachedPin >= 21 && attachedPin <= 23)
    || (attachedPin >= 25 && attachedPin <= 27)
    || (attachedPin >= 32 && attachedPin <= 33)
  ) {
    _attachedPin = attachedPin;
    _channel = channel;

    ledcSetup(_channel, 2300, 12); // 4096 values
    ledcAttachPin(_attachedPin, _channel);
    analogWriteResolution(12); // for analogWrite function argument

    return true;
  }

  Serial.println(F("You must use PWM pins in order to work with ESP32."));
  
  return false;
}

void PwmPin::_ledPWM(int magnitude) // 0~4096
{
  if (magnitude > 4096) {
    Serial.println(F("LED magnitude is too big, use values between 0 and 4095"));
  }
  analogWrite(_attachedPin, magnitude); 
  }

void PwmPin::_ledPWM(bool onOrOff)
{
  pinMode(_attachedPin, OUTPUT);
  digitalWrite(_attachedPin, (int) onOrOff);
}

void PwmPin::_tonePWM(int frequency) 
{
  ledcWriteTone(_channel, frequency);
}

