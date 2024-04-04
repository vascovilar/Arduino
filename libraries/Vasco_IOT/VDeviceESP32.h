/*
 * ESP32 onboarded features (wifi, bluetooth, RTC, ROM ...)
 * Ref: https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf
 * Doc: https://www.upesy.fr/blogs/tutorials/upesy-esp32-wrover-devkit-board-documentation-version-latest
 *      https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
 * 
 * Implementation:
 *
 *   #include <VDeviceESP32.h>
 *
 *   VDeviceESP32 esp; 
 *
 *   void setup() {
 *     esp.init();
 *   }
 *   void loop() {
       Serial.println(esp.getTimeStamp());
 *   }
 */

#ifndef VDeviceESP32_h
#define VDeviceESP32_h

#include "Arduino.h"
#include "VDevice.h"
#include "VSensor.h"
#include "VUseRtc.h"
#include "VUseWifi.h"
#include "VUsePins.h"
#include "VUsePsram.h"
#include "VUseEeprom.h"

class VDeviceESP32 : public VDevice, public VSensor, public VUsePwmPin, public VUsePsram, public VUseEeprom, public VUseRtc, public VUseWifi
{
  static const byte _PWM_CHANNEL = 3; 

  public:
     
    VDeviceESP32(byte pin) : VDevice(MICRO_CONTROLLER), VSensor(true) { 
      _ledPin = pin; 
    }
    
    // interfaces
    bool     init();
    bool     wake();
    bool     sleep();
    bool     check();
    bool     update();
    float    read();

    // data updated
    vfield_data getMemoryUsed() { return _data.memoryUsed; }
    float       getPsRamUsed() { return _data.psRamUsed; }
    
    // blue led
    void        onboardedLed(bool status); // build-in led
    void        onboardedLed(int magnitude);

    // testing proxy
    bool        connectWifi() { return _connectWIFI(); }
    bool        disconnectWifi() { return _disconnectWIFI(); }
    int         getWifiAccessPoints() { return _getAccessPointsWIFI(); }
    String      getWifiAccessPointInfo(int index) { return _getAccessPointInfoWIFI(index); }
    String      getIP() { return _getIpWIFI(); } 
    long        getTimeStamp() { return _getTimeRTC(); }
    String      getDateTime() { return _getDateTimeRTC(); }   
    void        getPsramTest();
    void        getEepromTest();

    // TODO vasco add http calls here and scraping tool in VDataHtml

  private:

    byte        _ledPin;  
    float       _maxValue = 0;

    struct fields {
      vfield_data memoryUsed = {"Mémoire utilisée", "%", 10.0};
      float       psRamUsed = 0;
    };
    fields  _data;  

    vlegend_data _memories[2] = {
      {80, VERT, ""},
      {100, ORANGE, "chargé"},
    };

    vlegend_data _psrams[1] = {
      {10000000, VERT, ""},
    };

    struct credential {
      const char* user;
      const char* password;
    };
};

#endif
