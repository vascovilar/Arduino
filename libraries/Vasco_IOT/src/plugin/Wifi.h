#ifndef Wifi_h
#define Wifi_h

#include "Arduino.h"
#include "WiFi.h"


class Wifi
{
  const char*       _NTP_SERVER = "pool.ntp.org"; // do not supports static
  static const long _GMT_OFFSET = 3600;
  static const int  _DAYLIGHT_OFFSET = 3600; // for France : winter 0, spring 3600
  static const int  _CREDENTIAL_COUNT = 2;
    
  protected:

    bool    _connectWIFI();
    bool    _disconnectWIFI();
    int     _getAccessPointsWIFI();
    String  _getAccessPointInfoWIFI(int index);
    String  _getIpWIFI();

  private:
  
    // TODO vasco move real credentials data in secure area
    struct vcredential {
      const char*   user;
      const char*   password;
    };
    vcredential _credentials[2] = {
      {"iPhone_vasco", "8743b52063cd"},
      {"freebox_vasco", "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d"}, // const char[32]
    };
};

#endif
