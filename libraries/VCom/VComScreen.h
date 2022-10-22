/*
 * Display info on TFT ST7789
 * use (modify in Arduino/libraries/TFT_eSPI/User_Setups/Setup24_ST7789.h)
 *  - SCL  18
 *  - SDA  23
 *  - RES  19
 *  - DC    5
 * Implementation:
 *
 *   #include "VComScreen.h"
 *
 *   VComScreen screen;
 *
 *   void setup() {
 *     screen.begin();
 *   }
 *   void loop() {
 *   }
 */

#ifndef VComScreen_h
#define VComScreen_h

#include "Arduino.h"
#include "TFT_eSPI.h"

class VComScreen
{
  public:

    void begin();
    void menu();

    void clear()                           { _tft.fillScreen(TFT_BLACK); }
    void title(String text, int x, int y)  { _tft.setTextColor(TFT_LIGHTGREY); _tft.drawString(text, x, y, 4); }
    void text(String text, int x, int y)   { _tft.setTextColor(TFT_LIGHTGREY); _tft.drawString(text, x, y, 2); }

  
  private:

    TFT_eSPI _tft;  
};

#endif
