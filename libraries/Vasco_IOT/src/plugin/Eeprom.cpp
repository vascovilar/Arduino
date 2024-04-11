#include "Eeprom.h"

bool Eeprom::_initEEPROM() 
{
  return EEPROM.begin(_EEPROM_SIZE);
}

bool Eeprom::_getEepromFlag(vmemory_index index, int boolSlot)
{
  byte value = EEPROM.read((int)index);

  return bitRead(value, boolSlot - 1) ? true: false;
}

bool Eeprom::_setEepromFlag(vmemory_index index, bool* boolSlots)
{
  byte data;
  for (int i = 0; i < 8; i++) {
    bitWrite(data, i, boolSlots[i] ? 1: 0);
  }
  
  EEPROM.write((int)index, data);
  
  return EEPROM.commit();
}