#ifndef Pins_h
#define Pins_h

#include "Arduino.h"


class I2cPins
{
  // ESP32 Harware default 2 wire I2C. Speeder if not override
  static const byte _I2C_SDA_PIN = 21; // SDA Qwiic blue
  static const byte _I2C_SCL_PIN = 22; // SCL Qwiic yellow
};


class SpiPins
{
  // ESP32 Harware default 4 wire SPI. Speeder if not override
  static const byte _SPI_MOSI_PIN = 23;
  static const byte _SPI_MISO_PIN = 19;
  static const byte _SPI_CLK_PIN = 18;
  static const byte _SPI_CS_PIN = 5;
};


class AdcPin
{
  static const int  _ADC_MAX_VALUE = 4095; // override with device value in child class
  static const int  _ADC_ZEROledcSetup_VALUE = 0; // override with device value in child class

  public:

    int     readAnalogRawValue();
    float   readAnalogPercentage();
    float   readAnalogFrequency();

  protected:

    bool    _initADC(byte attachedPin, bool isAmplified, float maxAnalogValue, float zeroAnalogValue);

  private:

    byte    _attachedPin;
    bool    _isAmplified;
    float   _maxAnalogValue;
    float   _zeroAnalogValue;
};


class PwmPin
{
  static const byte _PWM_CHANNEL = 0; // override with device value in child class
  static const int  _LED_FADEOUT_DELAY = 10;

  public:

    void    led(bool onOrOff);
    void    led(int percentage); // 0~100%
    bool    led(int from, int to, int duration); // 0~100%, ms
    int     magnitude(); // 0~100%
    bool    note(int frequency); // in Hz

  protected:

    bool    _initPWM(byte attachedPin, byte channel);

  private:

    byte    _attachedPin;
    byte    _channel;
};

#endif
