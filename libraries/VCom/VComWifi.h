/*
 * Connect to local network via WiFi and get real time
 * Implementation:
 *
 *   #include <VComWifi.h>
 *
 *   VComWifi net; 
 *
 *   void setup() {
 *     net.begin();
 *   }
 *   void loop() {
       String time = net.getTimeStamp();
 *   }
 */

#ifndef VComWifi_h
#define VComWifi_h

#include "Arduino.h"
#include "WiFi.h"
#include "time.h"

class VComWifi
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

    const char* ssid = "freebox_vasco";
    const char* password = "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d";
    //const char* ssid = "iPhone_vasco";
    //const char* password = "8743b52063cd";
    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;
};

#endif
