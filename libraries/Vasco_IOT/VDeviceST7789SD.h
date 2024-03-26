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
 *   VDeviceST7789SD screen(32);
 *
 *   void setup() {
 *     screen.begin();
 *   }
 *   void loop() {
 *     screen.text("Hello world !", 0, 0);
 *   }
 */

#ifndef VDeviceST7789SD_h
#define VDeviceST7789SD_h

#define ARDUINO 100

#include "Arduino.h"
#include "VDevice.h"
#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789
#include "SPI.h"

class VDeviceST7789SD : public VDevice, public VSPIPins, public VPwmPin
{
  static const byte  _PWM_CHANNEL = 1;

  static const byte  _SPI_CS_PIN = 5; // TFTCS
  static const byte  _SPI_RST_PIN = -1; // not connected
  static const byte  _SPI_DC_PIN = 4;
  static const byte  _SPI_SDCS_PIN = 2; // TODO vasco led ! 
  static const byte  _SPI_LIT_PIN = 15; 

  public:

    VDeviceST7789SD(byte pin) : VDevice(ADA_ST7789SD) { _pwmPin = pin; }

    bool init();
    bool wake();
    bool sleep();

    void clear();
    void title(String text, int x, int y);
    void text(String text, int x, int y);

  private:

    Adafruit_ST7789 _tft = Adafruit_ST7789(_SPI_CS_PIN, _SPI_DC_PIN, _SPI_RST_PIN);
    
    byte _pwmPin; // TODO vasco manage other pins in constructor if possible with member _tft initialization
};

#endif
