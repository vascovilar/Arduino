/*
 * Display on Adafruit TFT ST7789 and read and write onboarded SDCard
 * Ref: https://learn.adafruit.com/adafruit-1-14-240x135-color-tft-breakout/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/082/882/original/ST7789VW_SPEC_V1.0.pdf?1571860977
 */

#ifndef ST7789SD_h
#define ST7789SD_h

#define ARDUINO 100

#include "Arduino.h"
#include "interface/Screen.h"
#include "interface/Data.h"
#include "interface/Device.h"
#include "inherit/Pins.h"
#include "SPI.h"             // works with SPI bus
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include "SD.h"              // SD card filesystem library
//#include "SdFat.h"           // SD card & FAT filesystem library


class ST7789SD : public Device, public Screen, public SpiPins, public PwmPin
{
  static const byte _PWM_CHANNEL = 2;

  public:

    ST7789SD(byte tftcsPin, byte dcPin, byte sdcsPin, byte litPin) : Device(TFT_SD_SCREEN), _tft(tftcsPin, dcPin, -1) {
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
    void    text(String content, int x, int y, vtextsize size, vcolor code, vcolor bg = COLOR_BLACK, bool isInBuffer = false);
    void    point(int x, int y, vcolor code, bool isInBuffer = false);
    void    line(int x1, int y1, int x2, int y2, vcolor code, bool isInBuffer = false);
    void    rect(int x, int y, int width, int height, vcolor code, int radius = 0, bool isInBuffer = false);
    void    arrow(int x, int y, int width, int height, vcolor code, bool isInBuffer = false);
    void    circle(int x, int y, int radius, vcolor code, bool isInBuffer = false);
    void    swap(); // swap double buffer: _canvas -> display
    void    clear(); // clear screen

    // tft api
    void    led(bool status);
    void    led(int magnitude); // tft backligth 0: off, 4095: intensity max
    void    led(int from, int to, int duration); // from magnitude to magnitude in milli-seconds duration

    // sd card api
    void    listFiles();

  private:

    Adafruit_ST7789 _tft; // init by constructor
    GFXcanvas16     _canvas = GFXcanvas16(VSCREEN_WIDTH, VSCREEN_HEIGHT);
    //SD            _sd;
    byte            _tftcsPin;
    byte            _dcPin;
    byte            _sdcdPin;
    byte            _litPin;

    Adafruit_GFX*   _getDisplayPointer(bool isInBuffer);
    int             _convertToRGB565(int hexadecimal); // color code like 0x00FF00
    int             _convertToRGB565(String html); // color code like "#00FF00"

};

#endif
