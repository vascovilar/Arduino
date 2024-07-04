#include "Wifi.h"

bool Wifi::connectWifi()
{
  WiFi.mode(WIFI_STA);

  int count = WiFi.scanNetworks();

  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < _CREDENTIAL_COUNT; j++) {
      if (String(_credentials[j].user) == WiFi.SSID(i)) {
        // connect with first matching credentials
        WiFi.begin(_credentials[j].user, _credentials[j].password);
        // wait for 15 seconds max
        for (int k = 0; k < 30; k++) {
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

bool Wifi::disconnectWifi()
{
  return WiFi.disconnect();
}

int Wifi::getAccessPointsCount()
{
  return WiFi.scanNetworks();
}

String Wifi::getAccessPointInfo(int index)
{
  return String(index) + "- " + String(WiFi.SSID(index)) + " (" + String(WiFi.RSSI(index)) + "db)";
}

String Wifi::getNetwork()
{
  if (WiFi.status() == WL_CONNECTED) {

    return WiFi.SSID();
  }

  return "";
}

String Wifi::getIp()
{
  return WiFi.localIP().toString();
}
