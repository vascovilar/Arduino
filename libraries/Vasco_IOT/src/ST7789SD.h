/*
 * Display on Adafruit TFT ST7789 with GFX driver and read / write onboarded SDCard
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
#include "component/Pins.h"
#include "SPI.h"             // works with SPI bus
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include "SD.h"              // SD card filesystem library
//#include "SdFat.h"           // SD card & FAT filesystem library
#include "bin/muMatrix8ptRegular.h"


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

    void    text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isFixedWidthFont = false, bool isInBuffer = false);
    void    point(float x, float y, vcolor color, bool isInBuffer = false);
    void    line(float x1, float y1, float x2, float y2, vcolor color, bool isInBuffer = false);
    void    rect(float x, float y, int width, int height, vcolor color, int radius = 0, bool isFilled = true, bool isInBuffer = false);
    void    arrow(float x, float y, int width, int height, vcolor color, bool isInBuffer = false);
    void    circle(float x, float y, int radius, vcolor color, bool isInBuffer = false);
    void    bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isInBuffer = false);
    float   width(String content, vtextsize size, bool isFixedWidthFont = false);
    float   height(String content, vtextsize size);
    void    swap(); // swap double buffer: _canvas -> display
    void    clear(); // clear screen

    // tft api
    void    print(String content);

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
    int             _convertToSize(vtextsize size);

};

#endif
