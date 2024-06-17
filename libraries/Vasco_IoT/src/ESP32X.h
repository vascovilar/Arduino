/*
 * ESP32 onboarded features (wifi, bluetooth, RTC, ROM ...)
 * Ref: https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf
 * Doc: https://www.upesy.fr/blogs/tutorials/upesy-esp32-wrover-devkit-board-documentation-version-latest
 *      https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
 */

#ifndef ESP32X_h
#define ESP32X_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "interface/Run.h"
#include "component/Pins.h"
#include "component/Psram.h"
#include "component/Eeprom.h"
#include "component/Rtc.h"
#include "component/Wifi.h"


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
      switch (code) {
        case MEMORY_USED:
          return _memoryAvailable;
        case RUN_CYCLES:
          return _checkPerSecond;
      }

      return (vfield){};
    }

    // other data updated
    float   getPsRamUsed() { return _psRamAvailable; } // 0 -> 100 %
    String  getClockWatch() { return _clockWatch; }  // string format "19:34:55"

    // tmp testing
    void    getPsramTest();
    void    getEepromTest();

    // TODO vasco add http calls here and scraping tool,  here ?

  private:

    byte     _ledPin;
    vfield   _memoryAvailable = {"Mémoire disponible", "Ko", 10};
    vfield   _checkPerSecond = {"Programme", "fps", 100};
    float    _psRamAvailable = 0;
    String   _clockWatch = "";
    int      _processedChecks = 0;
    int      _processedTime = millis();
    bool     _isLed = false;

    float _convertToUsedMemoryPercentage(int freeMemory);
    float _convertToUsedPsRamPercentage(int freeMemory);
    float _convertToKiloByte(int freeMemory);


    vlegend _memories[3] = {
      {130, ROUGE, "insuffisante"},
      {160, ORANGE, "faible"},
      {10000, VERT, "ok"},
    };

    vlegend _psrams[1] = {
      {0, VERT, ""},
    };

    vlegend _checks[3] = {
      {10, ROUGE, "mesures faussées"},
      {100, ORANGE, "interface ralentie"},
      {10000, VERT, "ok"},
    };

};

#endif
