#include "VDeviceWifi.h"

void VDeviceWifi::begin()
{ 
  while (WiFi.status() != WL_CONNECTED) {
    scan();
  }
  Serial.println("Go: http://" + getIP());
  
  Serial.print("Requesting time server ...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("OK");
  Serial.println("Now: " + getTime());
  
  Serial.println("");
}

bool VDeviceWifi::scan()
{
  Serial.println("");
  Serial.print("Scan wifi networks ...");
  int n = WiFi.scanNetworks();
  Serial.println("OK");
  
  Serial.print(n);
  Serial.println(" networks found");

  for (int i = 0; i < n; ++i) { 
    // Print SSID and RSSI for each network found
    Serial.print(" | ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println("db)");
    // Connect to known wifis
    if (WiFi.SSID(i) == "iPhone_vasco") {
      if (connect("iPhone_vasco", "8743b52063cd")) {
        
        return true;
      }
    }
    if (WiFi.SSID(i) == "freebox_vasco") {
      if (connect("freebox_vasco", "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d")) {
        
        return true;
      }
    }
    delay(10);
  }
  delay(10000);

  return false;   
}

bool VDeviceWifi::connect(const char* ssid, const char* password)
{
  int i = 0;
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && i++ < 20) { // wait for 10 seconds max
    delay(500);
    Serial.print(".");
  }
  if (i > 20) {
    Serial.println("\nBad credentials or network unreachable");
    
    return false;
  }
  Serial.println("OK");

  return true;  
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
  
  if(!getLocalTime(&timeinfo, 1000)){
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
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get epoch time");
    
    return 0;
  }

  time(&now);
  
  return now;
}
