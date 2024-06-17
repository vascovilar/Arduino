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
  //analogSetWidth(12); // works ?
  analogReadResolution(12); // 4096 values
  analogSetPinAttenuation(_attachedPin, _isAmplified ? ADC_0db: ADC_11db);

  return true;
}

int AdcPin::readAnalogRawValue()
{
  return analogRead(_attachedPin);
}

float AdcPin::readAnalogPercentage()
{
  int rawValue = readAnalogRawValue();

  // float mapping
  if (rawValue > _maxAnalogValue) rawValue = _maxAnalogValue;
  rawValue = rawValue - _zeroAnalogValue;

  // signed % returned
  return (rawValue / (float)(_maxAnalogValue - _zeroAnalogValue)) * 100;
}

float AdcPin::readAnalogFrequency() // TODO vasco use FFT
{
  long time = millis();
  long timer = micros();
  float buffer[100];
  byte i = 0;
  int beat = 0;

  while (i < 100) { // total 100ms to measure
    if (micros() - timer > 1000) {
      timer = micros();
      buffer[i] = readAnalogRawValue();
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
  if (!((attachedPin >= 0 && attachedPin <= 19)
    || (attachedPin >= 21 && attachedPin <= 23)
    || (attachedPin >= 25 && attachedPin <= 27)
    || (attachedPin >= 32 && attachedPin <= 39)
  )) {
    Serial.println(F("You must use PWM pins"));

    return false;
  }

  _attachedPin = attachedPin;
  _channel = channel;

  if (!ledcAttachChannel(_attachedPin, 1000, 12, _channel)) { // 12 bits = 4096 values
    Serial.println(F("Can't attach pin to PWM"));

    return false;
  }

  analogWriteResolution(_attachedPin, 12); // for analogWrite function argument

  return true;
}

void PwmPin::led(bool onOrOff)
{
  ledcWrite(_attachedPin, onOrOff ? 4095: 0);
}

void PwmPin::led(int percentage)
{
  ledcWrite(_attachedPin, 4095 * (percentage / 100.0));
}

bool PwmPin::led(int from, int to, int duration)
{
  if (duration > 2000) duration = 2000; // TODO vasco why not more than 2 seconds ?

  return ledcFade(_attachedPin, 4095 * (from / 100.0), 4095 * (to / 100.0), duration);
}

int PwmPin::getMagnitude()
{
  return (ledcRead(_attachedPin) / 4095) * 100;
}

bool PwmPin::note(int frequency)
{
  // this send 50% duty wave, to use 100% call analogWriteFrequency(_attachedPin, frequency)
  return 0 != ledcWriteTone(_attachedPin, frequency);
}
