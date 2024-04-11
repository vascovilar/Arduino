#ifndef Eeprom_h
#define Eeprom_h

#include "Arduino.h"
#include "EEPROM.h"


static const byte VEMORY_SIZE = 16;

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


class Eeprom
{
  static const int _EEPROM_SIZE = 512; // ESP32 all version

  protected:
    bool    _initEEPROM();
    bool    _getEepromFlag(vmemory_index index, int boolSlot); // 8 booleans in one octet, slots from 1 to 8
    bool    _setEepromFlag(vmemory_index index, bool* boolSlots); // send 8 values in array for one octet

    template<typename T> 
    void    _readEEPROM(vmemory_index index, T &value)
    {
      EEPROM.get((int)index, value);
    }

    template<typename T> 
    void    _writeEEPROM(vmemory_index index, T &value)
    {
      T oldValue = 0;
      EEPROM.get((int)index, oldValue);
      if (value != oldValue) {
        EEPROM.put((int)index, value);
        EEPROM.commit();
      }
    }

  private:
    
};

#endif
