/*
 * Connect to local network via WiFi and get real time
 * Implementation:
 *
 *   #include <VToolWifi.h>
 *
 *   VToolWifi net; 
 *
 *   void setup() {
 *     net.begin();
 *   }
 *   void loop() {
       String time = net.getTimeStamp();
 *   }
 */

#ifndef VToolWifi_h
#define VToolWifi_h

#include "Arduino.h"
#include "WiFi.h"
#include "time.h"

class VToolWifi
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

    int _credentials_index = -1;
    const int _credentials_length = 2;
    credential _credentials[2] = {
      {"iPhone_vasco", "8743b52063cd"},
      {"freebox_vasco", "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d"},
    };
};

#endif
