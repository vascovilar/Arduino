#ifndef VSensor_h
#define VSensor_h

#include "Arduino.h"

#define VSENSOR_COUNT  9
enum sensor_code {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HUMIDITY = 2,
  AIR_QUALITY = 3,
  GAS_PERCENTAGE = 4,
  EMF_LEVEL = 5,
  UV_INDEX = 6,
  VISIBLE = 7,
  EAR_LEVEL = 8
};

#define VCOMFORT_COUNT  6
enum comfort_code {
  GRIS = 0,   // inactif
  VERT = 1,   // confort
  JAUNE = 2,  // perturbé
  ORANGE = 3, // gêne
  ROUGE = 4,  // mauvais
  VIOLET = 5  // danger
};

struct field_data {
  String        label;
  String        unit;
  float         tolerance;
  float         value;
  comfort_code  status;
  String        text;
};

struct legend_data {
  float         limit;
  comfort_code  status;
  String        text;
};


class VSensor
{
  public:

    byte          analogPin;
    float         maxValue;

    VSensor(byte pin) { analogPin = pin; } // pin=0 for i2c
    
    virtual bool  init() = 0; // init device
    virtual bool  wake() = 0; // make device wake up
    virtual bool  sleep() = 0; // make device sleep and consuming less possible
    virtual bool  sync() = 0; // played sometimes to get all data from device then store in local registry
    virtual bool  event() = 0; // played each turn in trigger mode, fire event by returning true
    virtual float check() = 0; // played each turn, for analog devices or to trig event else return zero

  protected:

    void _feed(field_data &field, float value, legend_data* data, int length)
    {
      field.value = value;
      for (int i = 0; i < length; i ++) {
        if (value <= data[i].limit || i == length - 1) {
          field.status = data[i].status;
          field.text = data[i].text;
          break;
        }
      }
    }

  private:
  
    struct    fields {}; // add here fields by inheritance override in child class
    fields    _data; // override too
};


class VI2CPins
{
  // Harware default. Speeder if not modified
  static const byte _I2C_SDA_PIN = 21; // Qwiic blue
  static const byte _I2C_SCL_PIN = 22; // Qwiic yellow

  // override with device value in child class
  static const byte _I2C_ADDRESS = 0x00;
};


class VAnalogPin
{
  // override with device value in child class
  static const int _ADC_MAX_VALUE = 4095; 
  static const int _ADC_ZERO_VALUE = 0;
  static const int _ADC_SMOOTH_FACTOR = 0;

  protected:

    bool _initADC(byte attachedPin, bool isAmplified, float maxAnalogValue, float zeroAnalogValue, byte smoothFactor) 
    {
      if (smoothFactor >= 100) {
        Serial.println(F("To init ADC smothFactor must be < 100"));
        return false;
      }
      if (!(attachedPin >= 32 && attachedPin <= 39)) {
        Serial.println(F("You must use ADC1 (pin 32 to 39) in order to work with ESP32. ADC2 is used by Wifi."));
        return false;
      }
      
      _attachedPin = attachedPin;
      _isAmplified = isAmplified;
      _maxAnalogValue = maxAnalogValue;
      _zeroAnalogValue = zeroAnalogValue;
      _smoothFactor = smoothFactor;
      
      pinMode(_attachedPin, INPUT);
      // analogSetWidth(12); already setted by default, but not settable without define CONFIG_IDF_TARGET_ESP32
      analogSetPinAttenuation(_attachedPin, _isAmplified ? ADC_0db: ADC_11db);

      return true;
    }
    
    float _readADC() 
    {
      int value = analogRead(_attachedPin);
  
      //smooth if needed
      if (_smoothFactor != 0) {
        value = _getSmoothedValue(value);
      } 
      // float mapping
      if (value > _maxAnalogValue) value = _maxAnalogValue;
      value = value - _zeroAnalogValue;

      // signed % returned
      return ((float) value / (float) (_maxAnalogValue - _zeroAnalogValue)) * 100;
    }

    float _readADCFrequency()
    {
      unsigned int time = millis();
      unsigned long timer = micros();
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

      return beat != 0 ? (float) 1000 / ((float) (millis() - time) / (float) beat): 0;    
    }
    
    float _getSmoothedValue(float value) 
    {
      float average;
      
      // smooth with "moving average" algorithm
      _smoothTotal = _smoothTotal - _smoothBuffer[_smoothIndex];
      _smoothBuffer[_smoothIndex] = value;
      _smoothTotal = _smoothTotal + _smoothBuffer[_smoothIndex];
      _smoothIndex++;
      if (_smoothIndex >= _smoothFactor) {
        _smoothIndex = 0;
      }
      average = _smoothTotal / _smoothFactor;

      return average; 
    }    

  private:
    byte  _attachedPin;
    bool  _isAmplified;
    float _maxAnalogValue;
    float _zeroAnalogValue;
    byte  _smoothFactor;
    float _smoothBuffer[100];
    byte  _smoothIndex = 0;
    float _smoothTotal = 0;
};

#endif
