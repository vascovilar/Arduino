#ifndef VUseWifi_h
#define VUseWifi_h

#include "Arduino.h"
#include "VUse.h"
#include "WiFi.h"

class VUseWifi
{
  const char* _NTP_SERVER = "pool.ntp.org";
  const long  _GMT_OFFSET = 3600;
  const int   _DAYLIGHT_OFFSET = 3600; // for France : winter 0, spring 3600
  
  // TODO vasco move real credentials data in secure area
  static const int _credentials_length = 2;
  vuser_credential _credentials[2] = {
    {"iPhone_vasco", "8743b52063cd"},
    {"freebox_vasco", "dbaZAxwC++tvYB8EpX1CxxkytD2FoH5d"}, // const char[32]
  };

  protected:

    bool      _connectWIFI();
    bool      _disconnectWIFI();
    int       _getAccessPointsWIFI();
    String    _getAccessPointInfoWIFI(int index);
    String    _getIpWIFI();
};

#endif
