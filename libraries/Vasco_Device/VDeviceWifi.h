/*
 * Connect to local network via WiFi and get real time
 * Implementation:
 *
 *   #include <VDeviceWifi.h>
 *
 *   VDeviceWifi net; 
 *
 *   void setup() {
 *     net.begin();
 *   }
 *   void loop() {
       String time = net.getTimeStamp();
 *   }
 */

#ifndef VDeviceWifi_h
#define VDeviceWifi_h

#include "Arduino.h"
#include "WiFi.h"
#include "time.h"

class VDeviceWifi
{
  public:
     
   void begin();
   bool scan();
   bool connect(const char* ssid, const char* password);
   bool disconnect();
   String getIP();
   String getTime();
   String getUpTime();
   unsigned long getTimeStamp();

  private:

    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;
};

#endif
