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
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();
    float   read();
    // data updated
    vfield  getMemoryUsed() { return _data.memoryUsed; }
    float   getPsRamUsed() { return _data.psRamUsed; }

    // api
    void    led(bool status); // build-in blue led. 0 or 1
    void    led(int magnitude); // magnitude: 0 to 4095
    void    led(int from, int to, int duration); // from magnitude to magnitude in milli-seconds duration
    bool    connectWifi() { return _connectWIFI(); }
    bool    disconnectWifi() { return _disconnectWIFI(); }
    int     getWifiAccessPoints() { return _getAccessPointsWIFI(); }
    String  getWifiAccessPointInfo(int index) { return _getAccessPointInfoWIFI(index); }
    String  getIP() { return _getIpWIFI(); }
    long    getTimeStamp() { return _getTimeStampRTC(); }
    String  getDateTime() { return _getDateTimeRTC(); }
    // testing proxy
    void    getPsramTest();
    void    getEepromTest();

    // TODO vasco add http calls here and scraping tool

  private:

    byte     _ledPin;
    float    _maxValueBuffer = 0;

    // human readable buffer. Updated by udpate function
    struct fields {
      vfield   memoryUsed = {"Mémoire utilisée", "%", 5.0};
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
