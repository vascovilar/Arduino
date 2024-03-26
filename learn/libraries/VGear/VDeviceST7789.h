/*
 * Display info on TFT ST7789
 * use (modify in Arduino/libraries/TFT_eSPI/User_Setups/Setup24_ST7789.h)
 *  - SDA  18
 *  - SCL  23
 *  - RES  19
 *  - DC    5
 * 
 * Implementation:
 *
 *   #include "VDeviceST7789.h"
 *
 *   VGearScreen screen;
 *
 *   void setup() {
 *     screen.begin(0);
 *   }
 *   void loop() {
 *     screen.text("Hello world !", 0, 0);
 *   }
 */

#ifndef VDeviceST7789_h
#define VDeviceST7789_h

#include "Arduino.h"
#include "TFT_eSPI.h"

class VDeviceST7789
{
  public:

    void begin(int rotation); // 0
    void clear();
    void title(String text, int x, int y);
    void text(String text, int x, int y);

  private:

    TFT_eSPI _tft;
};

#endif
