/*
 * Display on Adafruit TFT ST7789 and read and write onboarded SDCard
 * Ref: https://learn.adafruit.com/adafruit-1-14-240x135-color-tft-breakout/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/082/882/original/ST7789VW_SPEC_V1.0.pdf?1571860977
 *
 * Implementation:
 *
 *   #include "ST7789SD.h"
 *
 *   ST7789SD tft(5, 25, 33, 32);
 *
 *   void setup() {
 *     tft.begin();
 *     tft.led(true);
 *   }
 *   void loop() {
 *     tft.text("Hello world !", 0, 0, 0xFFFFFF);
 *   }
 */

#ifndef ST7789SD_h
#define ST7789SD_h

#define ARDUINO 100

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Device.h"
#include "plugin/Pins.h"
#include "SPI.h"             // works with SPI bus
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
//#include "SD.h"              // SD card filesystem library
#include "SdFat.h"           // SD card & FAT filesystem library


class ST7789SD : public Device, public SpiPins, public PwmPin
{
  static const byte _PWM_CHANNEL = 2;

  public:

    ST7789SD(byte tftcsPin, byte dcPin, byte sdcsPin, byte litPin) : Device(TFT_SD_SCREEN), _tft(Adafruit_ST7789(tftcsPin, dcPin, -1)) {
      _tftcsPin = tftcsPin;
      _dcPin = dcPin;
      _sdcdPin = sdcsPin;
      _litPin = litPin;
    }

    // interfaces
    bool    init();
    bool    sleep();
    bool    wake();
    bool    check();
    bool    update();

    // tft api
    void    clear(); // clear screen
    void    led(int magnitude); // tft backligth 0: off, 4095: intensity max
    void    led(int from, int to, int duration); // from magnitude to magnitude in milli-seconds duration
    void    title(String text, int x, int y, int colorCode);
    void    text(String text, int x, int y, int colorCode);
    void    point(int x, int y, int colorCode);
    int     convert(int hexadecimal); // color code like 0x00FF00
    int     convert(String html); // color code like "#00FF00"

    // sd card api
    void    listFiles();

  private:

    Adafruit_ST7789 _tft;
    SdFat   _sd;
    byte    _tftcsPin;
    byte    _dcPin;
    byte    _sdcdPin;
    byte    _litPin;
};

#endif
