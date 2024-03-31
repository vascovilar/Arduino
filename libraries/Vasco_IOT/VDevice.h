#ifndef VDevice_h
#define VDevice_h

#include "Arduino.h"
#include "EEPROM.h"

#define VDEVICE_COUNT  11
enum vdevice_code {
  ESP32_WROVER = 0,
  ADA_BME680 = 1,
  ADA_LTR390 = 2,
  ADA_PA1010D = 3,
  ADA_ST7789SD = 4,
  FERMION_SEN0487 = 5,
  JOYIT_AH49E = 6,
  CUSTOM_EMF = 7,
  CUSTOM_GAMMA = 8,
  CUSTOM_BUZZER = 9,
  CUSTOM_JOYSTICK = 10,
};

#define VEMORY_SIZE  16
enum vmemory_index {
  ROM_BYTE_1 = 0,
  ROM_BYTE_2 = 1,
  ROM_BYTE_3 = 2,
  ROM_BYTE_4 = 3,
  ROM_BYTE_5 = 4,
  ROM_BYTE_6 = 5,
  ROM_BYTE_7 = 6,
  ROM_BYTE_8 = 7,
  ROM_FLOAT_1 = 8,
  ROM_FLOAT_2 = 12,
};

struct memory_flags {
  bool flag1;
  bool flag2;
  bool flag3;
  bool flag4;
  bool flag5;
  bool flag6;
  bool flag7;
  bool flag8;
};

class VDevice
{
  public:

    vdevice_code   deviceCode;

    VDevice(vdevice_code code) { code = code; }     
    
    virtual bool  init() = 0; // init device
    virtual bool  wake() = 0; // make device wake up
    virtual bool  sleep() = 0; // make device sleep and consuming less possible
};

class VSPIPins
{
  // Harware default 4 wire SPI. Speeder if not modified
  static const byte _SPI_MOSI_PIN = 23; 
  static const byte _SPI_MISO_PIN = 19;  
  static const byte _SPI_CLK_PIN = 18;  
  static const byte _SPI_CS_PIN = 5;
};

class VPwmPin 
{
  // override with device value in child class
  static const byte _PWM_CHANNEL = 0;
  
  protected:

    bool _initPWM(byte attachedPin, byte channel) 
    {
      if ((attachedPin >= 0 && attachedPin <= 5) 
        || (attachedPin >= 12 && attachedPin <= 15)
        || (attachedPin >= 18 && attachedPin <= 19)
        || (attachedPin >= 21 && attachedPin <= 23)
        || (attachedPin >= 25 && attachedPin <= 27)
        || (attachedPin >= 32 && attachedPin <= 33)
      ) {
        _attachedPin = attachedPin;
        _channel = channel;

        pinMode(_attachedPin, OUTPUT);
        ledcSetup(_channel, 2300, 14);
        ledcAttachPin(_attachedPin, _channel);

        return true;
      }

      Serial.println(F("You must use pins in order to work with ESP32."));
      
      return false;
    }

    void _ledPWM(byte magnitude)
    {
      analogWrite(_attachedPin, magnitude);
    }
    
    void _tonePWM(int frequency) 
    {
        ledcWriteTone(_channel, frequency);
    }

  private:
    byte _attachedPin;
    byte _channel;

};

class VROMAccess
{
  protected:

    bool _initROM() 
    {
      return EEPROM.begin(VEMORY_SIZE);
    }

    // generic multi type getter and setter

    template<typename T> 
    void _readROM(vmemory_index index, T &value) 
    {
      // return by reference
      EEPROM.get((int)index, value);
    }

    template<typename T> 
    void _writeROM(vmemory_index index, T &value)
    {
      T oldValue = 0;
      EEPROM.get((int)index, oldValue);
      if (value != oldValue) {
        EEPROM.put((int)index, value);
        EEPROM.commit();
      }
    }

    // 8 structured booleans in one octet

    memory_flags _getROMFlags(vmemory_index index)
    {
      byte value = EEPROM.read((int)index);

      memory_flags data;
      data.flag1 = bitRead(value, 0) ? true: false;
      data.flag2 = bitRead(value, 1) ? true: false;
      data.flag3 = bitRead(value, 2) ? true: false;
      data.flag4 = bitRead(value, 3) ? true: false;
      data.flag5 = bitRead(value, 4) ? true: false;
      data.flag6 = bitRead(value, 5) ? true: false;
      data.flag7 = bitRead(value, 6) ? true: false;
      data.flag8 = bitRead(value, 7) ? true: false;

      return data; 
    }

    bool _setROMFlags(vmemory_index index, memory_flags value) 
    {
      byte data;
      bitWrite(data, 0, value.flag1 ? 1: 0);
      bitWrite(data, 1, value.flag2 ? 1: 0);
      bitWrite(data, 2, value.flag3 ? 1: 0);
      bitWrite(data, 3, value.flag4 ? 1: 0);
      bitWrite(data, 4, value.flag5 ? 1: 0);
      bitWrite(data, 5, value.flag6 ? 1: 0);
      bitWrite(data, 6, value.flag7 ? 1: 0);
      bitWrite(data, 7, value.flag8 ? 1: 0);

      EEPROM.write((int)index, data);
      
      return EEPROM.commit();
    }
};

#endif
