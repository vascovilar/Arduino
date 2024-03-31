/*
 * Display on TFT ST7789 and SDCard read and write
 *  - MOSI  23
 *  - MISO  19
 *  - CLK   18
 *  - TFTCS  5
 *  - DC     4
 *  - SDCS   2
 *  - LIT   15
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

#define ARDUINO 100 // TODO vasco really need ?

#include "Arduino.h"
#include "VData.h"
#include "VDevice.h"
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include "SPI.h"
#include "SdFat.h"           // SD card & FAT filesystem library


class VDeviceST7789SD : public VDevice, public VSPIPins, public VPwmPin
{
  static const byte _PWM_CHANNEL = 2;

  public:

    VDeviceST7789SD(byte tftcsPin, byte dcPin, byte sdcsPin, byte litPin) : VDevice(ADA_ST7789SD), _tft(Adafruit_ST7789(tftcsPin, dcPin, -1)) {
      _tftcsPin = tftcsPin; 
      _dcPin = dcPin; 
      _sdcdPin = sdcsPin; 
      _litPin = litPin;
    }

    bool init();
    bool wake();
    bool sleep();

    void clear(); // clear screen
    void light(byte magnitude); // tft backligth 0: off, 255: intensity max
    
    void title(String text, int x, int y, int colorCode);
    void text(String text, int x, int y, int colorCode);

    int  convert(int hexadecimal); // color code like 0x00FF00
    int  convert(String html); // color code like "#00FF00"

    void listFiles();

  private:

    Adafruit_ST7789 _tft;
    SdFat           _sd;
    
    byte _tftcsPin;
    byte _dcPin;
    byte _sdcdPin;
    byte _litPin;
    
};

#endif
