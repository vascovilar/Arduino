#include "VDeviceWifi.h"

void VDeviceWifi::begin()
{ 
  while (! scan()) {
    delay(10000);
  }

  Serial.println("Go: http://" + getIP());
  
  Serial.print("Requesting time server ...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("OK");

  Serial.println("Now: " + getTime() + "\n");
}

bool VDeviceWifi::scan()
{
  Serial.print("Scan wifi networks ...");
  int count = WiFi.scanNetworks();
  Serial.println("OK");
  
  Serial.println(String(count) + " networks found");

  for (int i = 0; i < count; ++i) { 
    Serial.println(" | " + String(WiFi.SSID(i)) + " (" + String(WiFi.RSSI(i)) + "db)");

    for (int j = 0; j < 2; ++j) { 
      if (String(_credentials[j].user) == WiFi.SSID(i)) {
        _ssid = j;
        if (connect()) {
          
          return true;
        } else {
          _ssid = -1;  
        }
      }
    }
    delay(10);
  }

  return false;   
}

bool VDeviceWifi::connect()
{
  if (_ssid >= 0) {
    Serial.print("Connecting to " + String(_credentials[_ssid].user));
    WiFi.begin(_credentials[_ssid].user, _credentials[_ssid].password);
    
    // wait for 10 seconds max
    for (int i = 0; i < 20; ++i) { 
      delay(500);
      Serial.print(".");

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("OK");

        return true;
      }
    }
  }

  Serial.println("Wrong credentials or network unreachable");
  
  return false;
}

bool VDeviceWifi::disconnect()
{
  WiFi.disconnect();
  Serial.println("Disconnecting from network");  

  return true;
}

String VDeviceWifi::getIP()
{
  return WiFi.localIP().toString();
}

String VDeviceWifi::getTime()
{
  struct tm timeinfo;
  timeinfo.tm_year = 0;
  
  if(!getLocalTime(&timeinfo, 5000)){
    Serial.println("Failed to get time from server, return uptime");
    
    return "0000-01-01 " + getUpTime();
  }

  char timeStamp[80];
  snprintf(timeStamp, 80, "%04d-%02d-%02d %02d:%02d:%02d", 
    timeinfo.tm_year + 1900, 
    timeinfo.tm_mon + 1, 
    timeinfo.tm_mday, 
    timeinfo.tm_hour, 
    timeinfo.tm_min, 
    timeinfo.tm_sec
  );

  return timeStamp;
}

String VDeviceWifi::getUpTime()
{
  int time = millis();
  int sec = time / 1000;
  int min = sec / 60;
  int hr = min / 60;
  
  char uptime[40];
  snprintf(uptime, 40, "%02d:%02d:%02d", hr, min % 60, sec % 60); 

  return uptime;
}

unsigned long VDeviceWifi::getTimeStamp()
{
  time_t now;
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo, 5000)) {
    Serial.println("Failed to get epoch time");
    
    return 0;
  }

  time(&now);
  
  return now;
}
