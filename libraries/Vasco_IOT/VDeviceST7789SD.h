/*
 * Display on Adafruit TFT ST7789 and read and write onboarded SDCard
 * Ref: https://learn.adafruit.com/adafruit-1-14-240x135-color-tft-breakout/overview
 * Doc: https://cdn-learn.adafruit.com/assets/assets/000/082/882/original/ST7789VW_SPEC_V1.0.pdf?1571860977 
 * 
 * Implementation:
 *
 *   #include "VDeviceST7789SD.h"
 *
 *   VDeviceST7789SD tft(5, 25, 33, 32);
 *
 *   void setup() {
 *     tft.begin();
 *     tft.light(255);
 *   }
 *   void loop() {
 *     tft.text("Hello world !", 0, 0, 0xFFFFFF);
 *   }
 */

#ifndef VDeviceST7789SD_h
#define VDeviceST7789SD_h

#define ARDUINO 100

#include "Arduino.h"
#include "VData.h"
#include "VUsePins.h"
#include "VDevice.h"
#include "SPI.h"             // works with SPI bus
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include "SdFat.h"           // SD card & FAT filesystem library


class VDeviceST7789SD : public VDevice, public VUseSpiPins, public VUsePwmPin
{
  static const byte _PWM_CHANNEL = 2;

  public:

    VDeviceST7789SD(byte tftcsPin, byte dcPin, byte sdcsPin, byte litPin) : VDevice(TFT_SD_SCREEN), _tft(Adafruit_ST7789(tftcsPin, dcPin, -1)) {
      _tftcsPin = tftcsPin; 
      _dcPin = dcPin; 
      _sdcdPin = sdcsPin; 
      _litPin = litPin;
    }

    // interfaces
    bool    init();
    bool    wake();
    bool    sleep();
    bool    check();
    bool    update();

    // tft
    void    clear(); // clear screen
    void    light(int magnitude); // tft backligth 0: off, 255: intensity max
    void    title(String text, int x, int y, int colorCode);
    void    text(String text, int x, int y, int colorCode);
    int     convert(int hexadecimal); // color code like 0x00FF00
    int     convert(String html); // color code like "#00FF00"

    // sd card
    void    listFiles();

  private:

    Adafruit_ST7789 _tft;
    SdFat           _sd;
    
    byte    _tftcsPin;
    byte    _dcPin;
    byte    _sdcdPin;
    byte    _litPin;
    
};

#endif
