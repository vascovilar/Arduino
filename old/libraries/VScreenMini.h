/*
 * Display formated boxes on SSD1306
 * Implementation:
 *   
 *   #include <VScreenMini.h>
 *
 *   VScreenMini view;
 *
 *   void setup() {
 *     view.begin(0x3C);
 *   }
 *   void loop() {
 *     view.character(0,0,random(10,100));
 *     view.display();
 *     delay(1000);
 *     view.clear();
 *   }
 */

#ifndef VScreenMini_h
#define VScreenMini_h

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "VIcon.h"

#define MINI_SCREEN_WIDTH 128
#define MINI_SCREEN_HEIGHT 64

class VScreenMini
{ 
  public:
    void begin(int addr);
    void display();
    void clear();

    void pixelAt(int x, int y); 
    void charAt(int x, int y, int code);
    void printAt(int x, int y, String text);
    void numericBox(int x, int y, String key, float value, int unit);
    void graphBox(int x, int y, int height, float maxValue, float minValue, float moyValue, float* buffer);
    void iconBox(int x, int y, const unsigned char* data);
    void rectangleBox(int x, int y, int w, int h, bool fill);

  private:
    Adafruit_SSD1306 _display;

    void _grid(int x, int y);
    void _title(int x, int y, String text);
    void _text(int x, int y, String text);
    void _inv(int x, int y, String text);
    void _char(int x, int y, int code);
};

#endif
