#ifndef Eeprom_h
#define Eeprom_h

#include "Arduino.h"
#include "EEPROM.h"


enum vmemory_index {
  ROM_BYTE_1 = 0, // 1 byte long
  ROM_BYTE_2 = 1,
  ROM_BYTE_3 = 2,
  ROM_BYTE_4 = 3,
  ROM_BYTE_5 = 4,
  ROM_BYTE_6 = 5,
  ROM_BYTE_7 = 6,
  ROM_BYTE_8 = 7,
  ROM_LONG_1 = 8, // 4 byte long
  ROM_LAST_EVENT_TIME = 12,
  ROM_PASSWORD_COUNT = 16, // 1 byte long
  ROM_PASSWORDS_RAW = 17, // 32 bytes long per password
};


class Eeprom
{
  static const int _EEPROM_SIZE = 512; // ESP32 all version

  protected:

    bool    _initEEPROM();
    bool    _getFlagInEEPROM(vmemory_index index, int boolSlot); // 8 booleans in one octet, slots from 1 to 8
    void    _setFlagInEEPROM(vmemory_index index, bool* boolSlots); // send 8 values in array for one octet
    char*   _getPasswordInEEPROM(byte index);
    void    _setPasswordInEEPROM(byte index, char* password);

    template<typename T>
    void    _readEEPROM(int index, T &value)
    {
      EEPROM.get(index, value);
    }

    template<typename T>
    void    _writeEEPROM(int index, T &value)
    {
      if ((sizeof(T) + index) > _EEPROM_SIZE) {
        Serial.println(F("Error: EEPROM memory is full"));

        return;
      }
      T oldValue = 0;
      EEPROM.get(index, oldValue);
      if (value != oldValue) {
        EEPROM.put(index, value);
        EEPROM.commit();
      }
    }
};

#endif
