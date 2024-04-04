#ifndef VUseEeprom_h
#define VUseEeprom_h

#include "Arduino.h"
#include "VUse.h"
#include "EEPROM.h"


class VUseEeprom
{
  static const int _EEPROM_SIZE = 512; // ESP32 all version

  protected:

    bool _initEEPROM();

    template<typename T> 
    void _readEEPROM(vmemory_index index, T &value)
    {
      // return by reference
      EEPROM.get((int)index, value);
    }

    template<typename T> 
    void _writeEEPROM(vmemory_index index, T &value)
    {
      T oldValue = 0;
      EEPROM.get((int)index, oldValue);
      if (value != oldValue) {
        EEPROM.put((int)index, value);
        EEPROM.commit();
      }
    }

    // 8 booleans in one octet
    bool _getEepromFlag(vmemory_index index, int boolSlot); // slots from 1 to 8
    bool _setEepromFlag(vmemory_index index, bool* boolSlots); // send 8 values in array
};

#endif
