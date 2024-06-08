/*
 * Graphics data lib
 *
 * used to draw text & images on a TFT screen
 */

#ifndef Bitmap_h
#define Bitmap_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../interface/Screen.h"
#include "../data/Mouse.h"
#include "../data/Buffer.h"
#include "../bin/iconMonoTransparent.h"


class Bitmap : public Data
{
  public:

    Bitmap(Screen &obj1, Mouse &obj2) : _screen(obj1), _mouse(obj2) {};  // bind objects

  protected:

    Mouse   &_mouse;
    Screen  &_screen;

    vzone   _drawPage(String titleLeft, String titleRight, int forcedWidth = 0);
    vzone   _drawPill(float x, float y, String contentLeft, String contentRight, int forcedWidth = 0);
    vzone   _drawButtonMini(float x, float y, String content, vpage target);
    vzone   _drawButtonArrow(float x, float y, vdirection direction, vpage target);
    vzone   _drawButtonBack(float x, vpage target);
    vzone   _drawBullet(float x, float y, vcolor color);
    vzone   _drawAnalogBar(float x, float y, float value, float minimum, float maximum);
    vzone   _drawIcon(float x, float y, const unsigned char* icon, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT);
    vzone   _drawTableHeader(float x, float y, String* titles, int length);
    vzone   _drawTableData(float x, float y, String* values, int length);
    const unsigned char* _getIcon(vsensor code);
};

#endif
