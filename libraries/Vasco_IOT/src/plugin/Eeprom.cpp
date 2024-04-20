#include "Eeprom.h"

bool Eeprom::_initEEPROM()
{
  return EEPROM.begin(_EEPROM_SIZE);
}

bool Eeprom::_getEepromFlag(vmemory_index index, int boolSlot)
{
  byte buffer = EEPROM.read((int)index);

  return bitRead(buffer, boolSlot - 1) ? true: false;
}

void Eeprom::_setEepromFlag(vmemory_index index, bool* boolSlots)
{
  byte buffer;
  for (int i = 0; i < 8; i++) {
    bitWrite(buffer, i, boolSlots[i] ? 1: 0);
  }
  _writeEEPROM((int)index, buffer);
}

char* Eeprom::_getPassword(byte index)
{
  char buffer[32];
  int addr = (int)index * 32 + (int)ROM_PASSWORDS_RAW;
  EEPROM.readBytes(addr, buffer, 32);

  return buffer;
}

void Eeprom::_setPassword(byte index, char* password)
{
  if ((32 + index) > _EEPROM_SIZE) {
    Serial.println(F("Error: EEPROM memory is full"));

    return;
  }
  int addr = (int)index * 32 + (int)ROM_PASSWORDS_RAW;
  EEPROM.writeBytes(addr, password, 32);
  EEPROM.commit();
}