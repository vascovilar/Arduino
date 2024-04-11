#include "Wifi.h"

bool Wifi::_connectWIFI()
{
  int count = WiFi.scanNetworks();

  for (int i = 0; i < count; ++i) { 
    for (int j = 0; j < _CREDENTIAL_COUNT; j++) { 
      if (String(_credentials[j].user) == WiFi.SSID(i)) {
        // connect with first matching credentials
        WiFi.begin(_credentials[j].user, _credentials[j].password);
        // wait for 10 seconds max
        for (int k = 0; k < 20; k++) { 
          delay(500);
          if (WiFi.status() == WL_CONNECTED) {
            // synchronize dateTime from web
            struct tm timeinfo;
            timeinfo.tm_year = 0;
            configTime(_GMT_OFFSET, _DAYLIGHT_OFFSET, _NTP_SERVER);
            // wait for 10 seconds max
            if (!getLocalTime(&timeinfo, 10000)) {
              Serial.println(F("Failed to get time from ntp server"));
            }

            return true;
          }
        }
      }
    }
    delay(10);
  }

  return false;
}

bool Wifi::_disconnectWIFI()
{
  return WiFi.disconnect();
}

int Wifi::_getAccessPointsWIFI()
{ 
  return WiFi.scanNetworks(); 
}

String Wifi::_getAccessPointInfoWIFI(int index)
{
  return String(index) + "- " + String(WiFi.SSID(index)) + " (" + String(WiFi.RSSI(index)) + "db)";
}

String Wifi::_getIpWIFI()
{
  return WiFi.localIP().toString();
}
