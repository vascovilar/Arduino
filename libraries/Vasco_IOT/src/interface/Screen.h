#ifndef Screen_h
#define Screen_h

#include "Arduino.h"
#include "interface/Data.h"
#include "interface/Sensor.h"
#include "data/Buffer.h"
#include "../component/Rtc.h"


static const int  VSCREEN_WIDTH = 240; // TODO vasco dispatch these in another classes
static const int  VSCREEN_HEIGHT = 135;
static const int  VSCREEN_WIDTH_WEB = 420;
static const int  VSCREEN_HEIGHT_WEB = 110;
static const int  VSCREEN_CHAR_WIDTH = 6;
static const int  VSCREEN_CHAR_HEIGHT = 7;
static const int  VSCREEN_PADDING_X = 8;
static const int  VSCREEN_PADDING_Y = 2;
static const int  VSCREEN_RADIUS = 7;
static const int  VSCREEN_LINESPACING = 9;
static const int  VSCREEN_COL_MARGIN = 5;
static const int  VSCREEN_COL_BAR = 68;
static const int  VSCREEN_COL_BACK = 95;
static const int  VSCREEN_OFFSET_HEADER = 2 * VSCREEN_RADIUS + 2 * VSCREEN_PADDING_Y + 5;
static const int  VSCREEN_OFFSET_FOOTER = VSCREEN_HEIGHT - VSCREEN_CHAR_HEIGHT - 5;
static const int  VSCREEN_OFFSET_EVENT = 74;


enum vtextsize {
  SIZE_SMALL = 1,
  SIZE_TEXT = 2,
  SIZE_TITLE = 3,
  SIZE_BIG = 4,
};


class Screen : public Rtc
{
  public:

    virtual void    text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isFixedWidthFont = false, bool isInBuffer = false) = 0;
    virtual void    point(float x, float y, vcolor color, bool isInBuffer = false) = 0;
    virtual void    line(float x1, float y1, float x2, float y2, vcolor color, bool isInBuffer = false) = 0;
    virtual void    rect(float x, float y, int width, int height, vcolor color, int radius = 0, bool isFilled = true, bool isInBuffer = false) = 0;
    virtual void    arrow(float x, float y, int width, int height, vcolor color, bool isInBuffer = false) = 0; // TODO vasco isFIlled or transparent
    virtual void    circle(float x, float y, int radius, vcolor color, bool isInBuffer = false) = 0; // TODO vasco isFIlled or transparent
    virtual void    bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isInBuffer = false) = 0;
    //virtual void    image(content, int x, int y, int width, int height) = 0 // TODO vasco continue with 16 bit color image
    virtual float   width(String content, vtextsize size, bool isFixedWidthFont = false) = 0;
    virtual float   height(String content, vtextsize size) = 0;
    virtual void    swap() = 0; // swap double buffer: _canvas -> display
    virtual void    clear() = 0; // clear screen

    // common math tools
    void            drawValueHistory(vfield field, Buffer buffer, int offset, int width, int height, vcolor bgColor = COLOR_TRANSPARENT, bool isInBuffer = false);
    float           drawSensorSnap(Sensor &sensor, int length, int offset, int width, int height, vcolor bgColor = COLOR_TRANSPARENT, bool isInBuffer = false);

  protected:

    void            _removeLatinChar(String &content); // tft screen does not support latin chars, return by reference
    float           _isometric(float value, float minimum, float maximum, int width, int offset); // needed for precision, map function use integers only

};

#endif
