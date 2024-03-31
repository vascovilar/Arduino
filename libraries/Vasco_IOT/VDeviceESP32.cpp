#include "VDeviceESP32.h"

bool VDeviceESP32::init()
{ 
  pinMode(_ledPin, OUTPUT);

  if (!_initROM()) {
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

bool VDeviceESP32::connectWifi()
{
  int count = WiFi.scanNetworks();

  for (int i = 0; i < count; ++i) { 
    for (int j = 0; j < _credentials_length; j++) { 
      if (String(_credentials[j].user) == WiFi.SSID(i)) {
        // connect with first matching credentials
        WiFi.begin(_credentials[j].user, _credentials[j].password);
        // wait for 10 seconds max
        for (int k = 0; k < 20; k++) { 
          delay(500);
          if (WiFi.status() == WL_CONNECTED) {
            // get dateTime from web
            configTime(_GMT_OFFSET, _DAYLIGHT_OFFSET, _NTP_SERVER);
            getDateTime();

            return true;
          }
        }
      }
    }
    delay(10);
  }

  return false;
}

bool VDeviceESP32::disconnectWifi()
{
  return WiFi.disconnect();
}

int VDeviceESP32::getWifiAccessPoints() 
{ 
  return WiFi.scanNetworks(); 
}
    
String VDeviceESP32::getWifiAccessPointInfo(int index)
{
  return String(index) + "- " + String(WiFi.SSID(index)) + " (" + String(WiFi.RSSI(index)) + "db)";
}

String VDeviceESP32::getDeviceIP()
{
  return WiFi.localIP().toString();
}

String VDeviceESP32::getUpTime()
{
  unsigned int time = millis();
  int sec = time / 1000;
  int min = sec / 60;
  int hr = min / 60;
  
  char uptime[40];
  snprintf(uptime, 40, "%02d:%02d:%02d",
    hr, 
    min % 60, 
    sec % 60
  ); 

  return uptime;
}

String VDeviceESP32::getDateTime()
{
  struct tm timeinfo;
  timeinfo.tm_year = 0;
  
  if(!getLocalTime(&timeinfo, 5000)){
    Serial.println(F("Failed to get time from ntp server, return uptime"));
    
    return "0000-01-01 " + getUpTime();
  }

  char dateTime[20];
  snprintf(dateTime, 20, "%04d-%02d-%02d %02d:%02d:%02d", 
    timeinfo.tm_year + 1900, 
    timeinfo.tm_mon + 1, 
    timeinfo.tm_mday, 
    timeinfo.tm_hour, 
    timeinfo.tm_min, 
    timeinfo.tm_sec
  );

  return dateTime;
}

void VDeviceESP32::setDateTime(String dateTime)
{
  // TODO vasco howto from GPS
}

unsigned long VDeviceESP32::getTimeStamp()
{
  time_t now;
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo, 5000)) {
    Serial.println(F("Failed to get epoch time from ntp server, return uptime"));
    
    return (int) (millis() / 1000);
  }

  time(&now);
  
  return now;
}

float VDeviceESP32::getMemoryUsage()
{
  return (1 - (ESP.getFreeHeap() / 532480.0)) * 100;
}

void VDeviceESP32::blueLed(bool status)
{
  digitalWrite(_ledPin, status ? HIGH: LOW);
}

void VDeviceESP32::getROMTest()
{
  // TODO vasco : EEPROM to use !
  
  byte variable1 = 204;
  _writeROM(ROM_BYTE_1, variable1);
  byte value = 0;
  _readROM(ROM_BYTE_1, value);
  Serial.println(value);

  memory_flags variable2 = {
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
  };
  _setROMFlags(ROM_BYTE_1, variable2);
  memory_flags status = _getROMFlags(ROM_BYTE_1);
  Serial.println(status.flag1 ? "true": "false");
}
  