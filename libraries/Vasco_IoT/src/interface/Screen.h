#ifndef Screen_h
#define Screen_h

#include "Arduino.h"
#include "../component/Rtc.h"
#include "../interface/Data.h"
#include "../interface/Sensor.h"
#include "../data/Buffer.h"
#include "../data/Snapshot.h"


static const int  VSCREEN_WIDTH = 240;
static const int  VSCREEN_HEIGHT = 135;
static const int  VSCREEN_WIDTH_WEB = 420;
static const int  VSCREEN_HEIGHT_WEB = 110;

enum vtextsize {
  SIZE_NULL = 0,
  SIZE_SMALL = 1,
  SIZE_TEXT = 2,
  SIZE_TITLE = 3,
  SIZE_BIG = 4,
};


class Screen : public Rtc
{
  public:

    virtual void    text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isFixedWidthFont = false) = 0;
    virtual void    point(float x, float y, vcolor color) = 0;
    virtual void    line(float x1, float y1, float x2, float y2, vcolor color) = 0;
    virtual void    rect(float x, float y, int width, int height, vcolor color, int radius = 0, bool isFilled = true) = 0;
    virtual void    arrow(float x, float y, int width, int height, vcolor color, vdirection direction) = 0; // TODO vasco *** isFIlled or transparent
    virtual void    circle(float x, float y, int radius, vcolor color) = 0; // TODO vasco *** isFIlled or transparent
    virtual void    bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT) = 0;
    virtual void    image(float x, float y, const uint16_t* data, int width, int height) = 0;
    virtual float   width(String content, vtextsize size = SIZE_NULL, bool isFixedWidthFont = false) = 0;
    virtual float   height(String content, vtextsize size= SIZE_NULL) = 0;
    virtual void    vertical(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT) = 0;
    virtual void    copy(float x, float y, int width, int height) = 0;
    virtual void    paste(float x, float y, int width, int height) = 0;
    virtual void    clear() = 0; // clear screen

    // common math tools
    void            drawValueHistory(float x, float y, vfield field, Buffer buffer, int width, int height, vcolor bgColor = COLOR_TRANSPARENT);
    void            drawSensorSnap(float x, float y, Snapshot snap, int tolerance, String unit, int width, int height, vcolor bgColor = COLOR_TRANSPARENT);

  protected:

    void            _removeLatinChar(String &content); // tft screen does not support latin chars, return by reference
    float           _isometric(float value, float minimum, float maximum, int width, int offset); // needed for precision, map function use integers only

};

#endif
