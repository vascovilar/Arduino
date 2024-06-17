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

  // reset led
  led(0);

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
  // count checks
  _processedChecks++;

  // blink blue led each 10 round // TODO vasco limit call to ledc api
  if (_processedChecks % 10) {
    led(_isLed);
    _isLed = !_isLed;
  }

  return false;
}

bool ESP32X::update()
{
  // sensor class values
  feed(_memoryAvailable, _convertToKiloByte(ESP.getFreeHeap()), _memories, 3);
  feed(_checkPerSecond, _processedChecks / ((millis() - _processedTime) / 1000.0), _checks, 3);

  // udpate other local variables
  _psRamAvailable = _convertToKiloByte(ESP.getFreePsram()); // readed on SPI channel 3, or use new esp_psram_get_size()
  _clockWatch = getDateTime().substring(11, 19);

  // reset counters
  _processedChecks = 0;
  _processedTime = millis(); // reset timer

  return true;
}

float ESP32X::read()
{
  return 0.0;
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
  setFlagInEeprom(ROM_BYTE_1, variable2);
  bool status = getFlagInEeprom(ROM_BYTE_1, 1);
  Serial.print("- value must be true, " + String(status ? "true": "false") + " found\n");
}

float ESP32X::_convertToUsedMemoryPercentage(int freeMemory)
{
  return (1 - (freeMemory / 327680.0)) * 100; // not 532480.0 free ! Only have 320Ko in runtime memory
}

float ESP32X::_convertToUsedPsRamPercentage(int freeMemory)
{
  return (1 - (freeMemory / 4194304.0)) * 100; // supposed to be 4Mo
}

float ESP32X::_convertToKiloByte(int freeMemory)
{
  return freeMemory / 1024.0;
}