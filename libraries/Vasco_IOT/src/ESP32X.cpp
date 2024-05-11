#include "ESP32X.h"

bool ESP32X::init()
{
  if (!_initPWM(_ledPin, _PWM_CHANNEL)) {
    Serial.println(F("Onboard led init failed"));

    return false;
  }

  if (!_initPSRAM()) {
    Serial.println(F("Error initializing PSRAM"));

    return false;
  }

  if (!_initEEPROM()) {
    Serial.println(F("Error initializing EEPROM"));

    return false;
  }

  return true;
}

bool ESP32X::sleep()
{
  return true;
}

bool ESP32X::wake()
{
  return true;
}

bool ESP32X::check()
{
  // led fadeout if needed
  _updateLedPMW();

  return false;
}

bool ESP32X::update()
{
  // sensor class values
  _feed(_memoryUsed, _convertToUsedMemoryPercentage(ESP.getFreeHeap()), _memories, 3);

  // udpate local variables
  _psRamUsed = _convertToUsedPsRamPercentage(ESP.getFreePsram()); // readed on SPI channel 3, or use new esp_psram_get_size()

  return true;
}

float ESP32X::read()
{
  return 0.0;
}

void ESP32X::led(bool status)
{
  _ledPWM(status);
}

void ESP32X::led(int magnitude)
{
  _ledPWM(magnitude);
}

void ESP32X::led(int from, int to, int duration)
{
  _ledPWM(from, to, duration);
}

void ESP32X::getPsramTest()
{
  byte *tab;
  _bindPSRAM(tab, 500000); // instead of inline: tab = (byte *) ps_malloc(500000);

  tab[10000] = 21;
  Serial.println("PSRAM test:");
  Serial.print("- value must be 21, " + String(tab[10000]) + " found\n");

  _freePSRAM(tab);
  Serial.print("- " + String(ESP.getFreePsram()) + " bytes free\n");
}

void ESP32X::getEepromTest()
{
  byte variable1 = 204;
  _writeEEPROM(ROM_BYTE_1, variable1);
  byte value = 0;
  _readEEPROM(ROM_BYTE_1, value);
  Serial.println("EEPROM test:");
  Serial.print("- value must be 204, " + String(value) + " found\n");

  bool variable2[8] = {
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
  };
  _setFlagInEEPROM(ROM_BYTE_1, variable2);
  bool status = _getFlagInEEPROM(ROM_BYTE_1, 1);
  Serial.print("- value must be true, " + String(status ? "true": "false") + " found\n");
}

float ESP32X::_convertToUsedMemoryPercentage(int freeMemory)
{
  return (1 - (freeMemory / 327680.0)) * 100; // not 532480.0 free ! Only have 320Ko in runtime memory
}

float ESP32X::_convertToUsedPsRamPercentage(int freeMemory)
{
  return (1 - (ESP.getFreePsram() / 4194304.0)) * 100; // supposed to be 4Mo
}
