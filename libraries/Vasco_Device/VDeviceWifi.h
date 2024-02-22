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
   bool connect();
   bool disconnect();
   String getIP();
   String getTime();
   String getUpTime();
   unsigned long getTimeStamp();

   // TODO add call http

  private:
    
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 3600;
    const int daylightOffset_sec = 3600;

    struct credential {
      const char* user;
      const char* password;
    };

    int _ssid = -1;
    credential _credentials[2] = {
      {"iPhone_vasco", "8743b52063cd"},
      {"freebox_vasco", "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d"},
    };
};

#endif
