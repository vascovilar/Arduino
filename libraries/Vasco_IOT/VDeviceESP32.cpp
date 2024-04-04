#include "VDeviceESP32.h"

bool VDeviceESP32::init()
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

bool VDeviceESP32::wake()
{
  return true;
}

bool VDeviceESP32::sleep()
{ 
  return true;
}

bool VDeviceESP32::check()
{ 
  float value = read();
  if (value > _maxValue) {
    _maxValue = value;
  }
  // detecting charge
  return _maxValue >= 100;
}

bool VDeviceESP32::update()
{ 
  _feed(_data.memoryUsed, _maxValue, _memories, 3);
  _maxValue = 0;

  _data.psRamUsed = (1 - ((float) ESP.getFreePsram() / 4194304.0)) * 100; // on SPI channel 3, or use new esp_psram_get_size()
  
  return true;
}

float VDeviceESP32::read()
{
  return (1 - ((float) ESP.getFreeHeap() / 327680.0)) * 100; // not 532480.0 ! Only have 320Ko in runtime
}

void VDeviceESP32::onboardedLed(bool status)
{
  _ledPWM(status);
}

void VDeviceESP32::onboardedLed(int magnitude)
{
  _ledPWM(magnitude);
}

void VDeviceESP32::getPsramTest()
{
  byte *tab;
  _bindPSRAM(tab, 500000); // instead of inline: tab = (byte *) ps_malloc(500000);

  tab[10000] = 21;
  Serial.println("PSRAM value must be 21: " + String(tab[10000]));

  _freePSRAM(tab);
  Serial.println("PSRAM: " + String(ESP.getFreePsram()) + " bytes free");
}

void VDeviceESP32::getEepromTest()
{
  byte variable1 = 204;
  _writeEEPROM(ROM_BYTE_1, variable1);
  byte value = 0;
  _readEEPROM(ROM_BYTE_1, value);
  Serial.println("EEPROM value must be 204: " + String(value));

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
  _setEepromFlag(ROM_BYTE_1, variable2);
  bool status = _getEepromFlag(ROM_BYTE_1, 1);
  Serial.println("EEPROM value must be true: " + String(status ? "true": "false"));
}
