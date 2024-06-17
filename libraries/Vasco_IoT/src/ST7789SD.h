/*
 * Display on Adafruit TFT ST7789 with GFX driver and read / write onboarded SDCard
 * Ref: https://learn.adafruit.com/adafruit-1-14-240x135-color-tft-breakout/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/082/882/original/ST7789VW_SPEC_V1.0.pdf?1571860977
 */

#ifndef ST7789SD_h
#define ST7789SD_h

//#define ARDUINO 100
//#define TFT_SDA_READ

#include "Arduino.h"
#include "interface/Screen.h"
#include "interface/Data.h"
#include "interface/Device.h"
#include "component/Pins.h"
#include "component/Psram.h"
#include "SPI.h"
#include "TFT_eSPI.h"     // Core graphics library // config TFT_eSPI/User_Setups/Setup24_ST7789.h & User_Setup_Select.h
#include "SD.h"           // std SDFS SD card filesystem library
//#include "SdFat.h"      // SD card & FAT filesystem library // TODO vasco
#include "bin/muMatrix8ptRegular.h"


static const int  VTFT_CHAR_WIDTH = 6;
static const int  VTFT_CHAR_HEIGHT = 7;


class ST7789SD : public Device, public Screen, public SpiPins, public PwmPin, public Psram
{
  static const byte _PWM_CHANNEL = 4;

  public:

    ST7789SD(byte tftcsPin, byte dcPin, byte sdcsPin, byte litPin) : Device(TFT_SD_SCREEN), _tft() {
      _tftcsPin = tftcsPin;
      _dcPin = dcPin;
      _sdcsPin = sdcsPin;
      _litPin = litPin;
      //if (_initPSRAM()) {
      //  _bindPSRAM(_bitmapBuffer, 400);
      //}
    }

    // interfaces
    bool      init();
    bool      sleep();
    bool      wake();
    bool      check();
    bool      update();
    void      text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isFixedWidthFont = false);
    void      point(float x, float y, vcolor color);
    void      line(float x1, float y1, float x2, float y2, vcolor color);
    void      rect(float x, float y, int width, int height, vcolor color, int radius = 0, bool isFilled = true);
    void      arrow(float x, float y, int width, int height, vcolor color, vdirection direction);
    void      circle(float x, float y, int radius, vcolor color);
    void      bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT);
    void      image(float x, float y, const uint16_t* data, int width, int height);
    float     width(String content, vtextsize size = SIZE_NULL, bool isFixedWidthFont = false);
    float     height(String content, vtextsize size = SIZE_NULL);
    void      vertical(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT);
    void      copy(float x, float y, int width, int height);
    void      paste(float x, float y, int width, int height);
    void      clear(); // clear screen

    // tft api
    void      show(String content);

    // sd card api
    void      listFiles();

  private:

    TFT_eSPI  _tft; // init by constructor
    TFT_eSprite _spr = TFT_eSprite(&_tft);
    SDFS      _sd = SD ; // init by constructor
    byte      _tftcsPin;
    byte      _dcPin;
    byte      _sdcsPin;
    byte      _litPin;
    GFXfont*  _gfxFont;
    uint16_t  _bitmapBuffer[192]; // TODO vasco do more generic !

    int       _convertToRGB565(int hexadecimal); // color code like 0x00FF00
    int       _convertToRGB565(String html); // color code like "#00FF00"
    int       _convertToSize(vtextsize size);
};

#endif
