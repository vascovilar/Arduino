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
 *   ESP32X esp;modified
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
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "plugin/Pins.h"
#include "plugin/Psram.h"
#include "plugin/Eeprom.h"
#include "plugin/Rtc.h"
#include "plugin/Wifi.h"


class ESP32X : public Sensor, public PwmPin, public Psram, public Eeprom, public Rtc, public Wifi
{
  static const byte _PWM_CHANNEL = 3;

  public:

    ESP32X(byte pin) : Sensor(MICRO_CONTROLLER, false) { _ledPin = pin; }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    vfield  get(vsensor code)
    {
      switch(code) {
        case MEMORY_USED:
          return _memoryUsed;
      }

      return {};
    }

    // other data updated
    float   getPsRamUsed() { return _psRamUsed; }

    // api
    void    led(bool status); // build-in blue led. 0 or 1
    void    led(int magnitude); // magnitude: 0 to 4095
    void    led(int from, int to, int duration); // from magnitude to magnitude in milli-seconds duration
    bool    connectWifi() { return _connectWIFI(); }
    bool    disconnectWifi() { return _disconnectWIFI(); }
    int     getWifiAccessPoints() { return _getAccessPointsFromWIFI(); }
    String  getWifiAccessPointInfo(int index) { return _getAccessPointInfoFromWIFI(index); }
    String  getIP() { return _getIpWIFI(); }
    long    getTimeStamp() { return _getTimeStampRTC(); }
    String  getDateTime() { return _getDateTimeRTC(); }

    // TODO vasco tmp testing proxy
    void    getPsramTest();
    void    getEepromTest();

    // TODO vasco add http calls here and scraping tool

  private:

    byte     _ledPin;
    vfield   _memoryUsed = {"Mémoire utilisée", "%", 5.0};
    float    _psRamUsed = 0;

    float _convertToUsedMemoryPercentage(int freeMemory);
    float _convertToUsedPsRamPercentage(int freeMemory);


    vlegend _memories[2] = {
      {80, VERT, ""},
      {100, ORANGE, "chargé"},
    };

    vlegend _psrams[1] = {
      {10000000, VERT, ""},
    };
};

#endif
