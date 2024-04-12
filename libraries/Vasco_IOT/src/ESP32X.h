/*
 * ESP32 onboarded features (wifi, bluetooth, RTC, ROM ...)
 * Ref: https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf
 * Doc: https://www.upesy.fr/blogs/tutorials/upesy-esp32-wrover-devkit-board-documentation-version-latest
 *      https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
 * 
 * Implementation:
 *
 *   #include <ESP32X.h>
 *
 *   ESP32X esp; 
 *
 *   void setup() {
 *     esp.init();
 *   }
 *   void loop() {
       Serial.println(esp.getTimeStamp());
 *   }
 */

#ifndef ESP32X_h
#define ESP32X_h

#include "Arduino.h"
#include "interface/Device.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"
#include "plugin/Psram.h"
#include "plugin/Eeprom.h"
#include "plugin/Rtc.h"
#include "plugin/Wifi.h"


class ESP32X : public Device, public Sensor, public PwmPin, public Psram, public Eeprom, public Rtc, public Wifi
{
  static const byte _PWM_CHANNEL = 3; 

  public:   
    ESP32X(byte pin) : Device(MICRO_CONTROLLER), Sensor(true) { _ledPin = pin; }    
    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();
    long    read();
    // data updated
    vfield  getMemoryUsed() { return _data.memoryUsed; }
    float   getPsRamUsed() { return _data.psRamUsed; } 
    // blue led
    void    onboardedLed(bool status); // build-in led
    void    onboardedLed(int magnitude);
    // testing proxy
    bool    connectWifi() { return _connectWIFI(); }
    bool    disconnectWifi() { return _disconnectWIFI(); }
    int     getWifiAccessPoints() { return _getAccessPointsWIFI(); }
    String  getWifiAccessPointInfo(int index) { return _getAccessPointInfoWIFI(index); }
    String  getIP() { return _getIpWIFI(); } 
    long    getTimeStamp() { return _getTimeRTC(); }
    String  getDateTime() { return _getDateTimeRTC(); }   
    void    getPsramTest();
    void    getEepromTest();

    // TODO vasco add http calls here and scraping tool in DataHtml

  private:
    byte     _ledPin;  
    float    _maxValue = 0;

    // human readable
    struct fields {
      vfield   memoryUsed = {"Mémoire utilisée", "%", 10.0};
      float    psRamUsed = 0;
    };
    fields _data;

    vlegend _memories[2] = {
      {80, VERT, ""},
      {100, ORANGE, "chargé"},
    };

    vlegend _psrams[1] = {
      {10000000, VERT, ""},
    };
};

#endif
