/*
 * Display info on TFT ST7789
 * use (modify in Arduino/libraries/TFT_eSPI/User_Setups/Setup24_ST7789.h)
 *  - SDA  18
 *  - SCL  23
 *  - RES  19
 *  - DC    5
 * Implementation:
 *
 *   #include "VGearScreen.h"
 *
 *   VGearScreen screen;
 *
 *   void setup() {
 *     screen.begin();
 *   }
 *   void loop() {
 *   }
 */

#ifndef VGearScreen_h
#define VGearScreen_h

#include "Arduino.h"
#include "TFT_eSPI.h"

#define SCREEN_BACK_WORD "< back"

class VGearScreen
{
  public:

    void begin(String* menu, int size);
    void next(int i);
    void click(std::function<bool(String)> callback);

    void clear()                           { _tft.fillScreen(TFT_BLACK); }
    void title(String text, int x, int y)  { _tft.setTextColor(TFT_LIGHTGREY); _tft.drawString(text, x, y, 4); }
    void text(String text, int x, int y)   { _tft.setTextColor(TFT_LIGHTGREY); _tft.drawString(text, x, y, 2); }
  
    void menuBox(String text, int rank, bool isSelected) {
      int y = rank * 20;
      if (isSelected) {
        _tft.setTextColor(TFT_WHITE);
        _tft.fillRect(0, y, 240, 20, TFT_BLUE);  
      } else {
        _tft.setTextColor(TFT_LIGHTGREY);
      }
      _tft.drawCentreString(text, 120, y+2, 2);
    }

  private:

    TFT_eSPI _tft; 
    String*  _menu;
    int      _size;
    int      _position = 0;
    String   _word = "";

    String   _enumMenu();
    String   _matchMenu(String command);
    String   _addData(String data, String command);
    int      _countData(String data);
    String   _getData(String data, int position);
    void     _iterateData(String data, int position);
    
};

#endif
