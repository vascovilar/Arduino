#ifndef Pins_h
#define Pins_h

#include "Arduino.h"


class I2cPins
{
  // ESP32 Harware default 2 wire I2C. Speeder if not modified
  static const byte _I2C_SDA_PIN = 21; // SDA Qwiic blue
  static const byte _I2C_SCL_PIN = 22; // SCL Qwiic yellow
};


class SpiPins
{
  // ESP32 Harware default 4 wire SPI. Speeder if not modified
  static const byte _SPI_MOSI_PIN = 23; 
  static const byte _SPI_MISO_PIN = 19;  
  static const byte _SPI_CLK_PIN = 18;  
  static const byte _SPI_CS_PIN = 5;
};


class AdcPin
{
  static const int _ADC_MAX_VALUE = 4095; // override with device value in child class
  static const int _ADC_ZERO_VALUE = 0; // override with device value in child class

  protected:
    bool    _initADC(byte attachedPin, bool isAmplified, float maxAnalogValue, float zeroAnalogValue); 
    float   _readADC(); 
    float   _readADCFrequency(); 

  private:
    byte    _attachedPin;
    bool    _isAmplified;
    float   _maxAnalogValue;
    float   _zeroAnalogValue;
};


class PwmPin 
{
  static const byte _PWM_CHANNEL = 0; // override with device value in child class
  
  protected:
    bool    _initPWM(byte attachedPin, byte channel);
    void    _ledPWM(int magnitude); // 0~255
    void    _ledPWM(bool onOrOff);
    void    _tonePWM(int frequency); 
    
  private:
    byte    _attachedPin;
    byte    _channel;
};

#endif
