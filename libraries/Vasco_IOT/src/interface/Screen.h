#ifndef Screen_h
#define Screen_h

#include "Arduino.h"
#include "interface/Data.h"

static const int  VSCREEN_WIDTH = 240;
static const int  VSCREEN_HEIGHT = 135;

enum vtextsize {
  CHAR_6x8 = 1,
  CHAR_12x16 = 2,
  CHAR_24x32 = 3,
  CHAR_48x64 = 4,
  CHAR_96x128 = 5,
};


class Screen
{
  public:

    virtual void    text(String content, int x, int y, vtextsize size, vcolor code, vcolor bg = COLOR_BLACK, bool isInBuffer = false);
    virtual void    point(int x, int y, vcolor code, bool isInBuffer = false);
    virtual void    line(int x1, int y1, int x2, int y2, vcolor code, bool isInBuffer = false);
    virtual void    rect(int x, int y, int width, int height, vcolor code, int radius = 0, bool isInBuffer = false);
    virtual void    arrow(int x, int y, int width, int height, vcolor code, bool isInBuffer = false);
    virtual void    circle(int x, int y, int radius, vcolor code, bool isInBuffer = false);
    virtual void    swap(); // swap double buffer: _canvas -> display
    virtual void    clear(); // clear screen

    // api
    int             getWidth() { return VSCREEN_WIDTH; }
    int             getHeight() { return VSCREEN_HEIGHT; }


    //virtual void    image(content, int x, int y, int width, int height) // TODO vasco continue
};

#endif