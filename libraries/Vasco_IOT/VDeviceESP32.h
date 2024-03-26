/*
 * Connect to local network via WiFi and get real time
 * Implementation:
 *
 *   #include <VDeviceESP32.h>
 *
 *   VDeviceESP32 net; 
 *
 *   void setup() {
 *     net.begin();
 *   }
 *   void loop() {
       String time = net.getTimeStamp();
 *   }
 */

#ifndef VDeviceESP32_h
#define VDeviceESP32_h

#include "Arduino.h"
#include "VDevice.h"
#include "WiFi.h"
#include "time.h"

class VDeviceESP32 : public VDevice, public VROMAccess
{
  const char* _NTP_SERVER = "pool.ntp.org";
  const long  _GMT_OFFSET = 3600;
  const int   _DAYLIGHT_OFFSET = 0; // 3600 in France winter

  public:
     
    VDeviceESP32(byte pin) : VDevice(ESP32_WROVER) { _ledPin = pin; }
    
    bool init();
    bool wake();
    bool sleep();
    
    bool    connectWifi();
    bool    disconnectWifi();
    int     getWifiAccessPoints();
    String  getWifiAccessPointInfo(int index);
    String  getDeviceIP();

    String  getUpTime();
    String  getDateTime();
    void    setDateTime(String dateTime);
    unsigned long getTimeStamp();
    
    float   getMemoryUsage();
    void    blueLed(bool status);

    // TODO vasco EEPROM
    void getROMTest();

    // TODO vasco add call http

  private:

    byte _ledPin;    

    struct credential {
      const char* user;
      const char* password;
    };

    const int _credentials_length = 2;
    credential _credentials[2] = {
      {"iPhone_vasco", "8743b52063cd"},
      {"freebox_vasco", "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d"},
    };
};

#endif
