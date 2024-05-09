#include "Pins.h"

bool AdcPin::_initADC(byte attachedPin, bool isAmplified, float maxAnalogValue, float zeroAnalogValue)
{
  if (!(attachedPin >= 32 && attachedPin <= 39)) {
    Serial.println(F("You must use ADC1 (pin 32 to 39) in order to work with ESP32. ADC2 is already used by Wifi"));

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

int AdcPin::_rawADC()
{
  return analogRead(_attachedPin);
}

float AdcPin::_readADC()
{
  int value = _rawADC();

  // float mapping
  if (value > _maxAnalogValue) value = _maxAnalogValue;
  value = value - _zeroAnalogValue;

  // signed % returned
  return (value / (float)(_maxAnalogValue - _zeroAnalogValue)) * 100;
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
      buffer[i] = _rawADC();
      if (buffer[i] == 0 && buffer[i-1] > 0) {
        beat++;
      }
      i++;
    }
  }

  return beat != 0 ? 1000.0 / ((millis() - time) / (float)beat): 0;
}

bool PwmPin::_initPWM(byte attachedPin, byte channel)
{
  // ESP 32
  if ((attachedPin >= 0 && attachedPin <= 19)
    || (attachedPin >= 21 && attachedPin <= 23)
    || (attachedPin >= 25 && attachedPin <= 27)
    || (attachedPin >= 32 && attachedPin <= 39)
  ) {
    _attachedPin = attachedPin;
    _channel = channel;

    ledcSetup(_channel, 2300, 12); // 4096 values
    ledcAttachPin(_attachedPin, _channel);
    analogWriteResolution(12); // for analogWrite function argument

    return true;
  }

  Serial.println(F("You must use PWM pins in order to work with ESP32"));

  return false;
}

void PwmPin::_ledPWM(bool onOrOff)
{
  pinMode(_attachedPin, OUTPUT);
  digitalWrite(_attachedPin, (int)onOrOff);
}

void PwmPin::_ledPWM(int magnitude) // 0~4096
{
  if (magnitude > 4096 || magnitude < 0) {
    Serial.println(F("Use LED magnitude values only between 0 and 4095"));
  }
  analogWrite(_attachedPin, magnitude);
}

void PwmPin::_ledPWM(int from, int to, int duration)
{
  // led fadeout start
  _ledPWM(from);
  _fadeOutMagnitude = from;
  _fadeOutIncrement = (from - to) / (float)(duration / _LED_FADEOUT_DELAY);
  _fadeOutTimer = millis();
}

void PwmPin::_updateLedPMW()
{
  // led fadeout update
  if (_fadeOutIncrement != 0) {
  if (millis() - _fadeOutTimer > _LED_FADEOUT_DELAY) {
      _fadeOutTimer = millis(); // reset timer
      _fadeOutMagnitude -= _fadeOutIncrement;
      if (_fadeOutMagnitude <= 0) {
        _fadeOutMagnitude = 0;
        _fadeOutIncrement = 0;
      }
      _ledPWM(_fadeOutMagnitude);
    }
  }
}

void PwmPin::_tonePWM(int frequency)
{
  ledcWriteTone(_channel, frequency);
}
