#include "PIM447.h"

bool PIM447::init()
{
  if (_i2cAddress != 0x0A) {
    Serial.println(F("Error PIM447 device use I2C address 0x0A"));

    return false;
  }

  Wire.begin(); // need to initialise I2C
  _trackBall.begin(_i2cAddress, Wire);
  if(!_trackBall.isConnected()) {
    Serial.println(F("Error initializing I2C PIM447 device"));

    return false;
  }

  resetMouse();

  return true;
}

bool PIM447::sleep()
{
  return true;
}

bool PIM447::wake()
{
  return true;
}

bool PIM447::check()
{
  // no need to listen continuously because device have memory
  return false;
}

bool PIM447::update()
{
  // update local variables
  if (_trackBall.changed()) {
    int left = _trackBall.left();
    int right = _trackBall.right();
    int up = _trackBall.up();
    int down = _trackBall.down();
    // i.e. screen referential
    _x = _x + (right - left);
    if (_x < 0) _x = 0;
    if (_x > _width) _x = _width;
    _y = _y + (down - up);
    if (_y < 0) _y = 0;
    if (_y > _height) _y = _height;
    // consider clicked when button released
    _focus = _trackBall.click();
    _click = _trackBall.release();
    // direction for menu navigation
    _left = left > right;
    _right = right > left;
    _up = up > down;
    _down = down > up;

    return true;
  }

  return false;
}

void PIM447::led(int hexadecimal, byte brightness)
{
  // split RGB channels
  _trackBall.setRed(hexadecimal >> 16);
  _trackBall.setGreen(hexadecimal >> 8 & 0xFF);
  _trackBall.setBlue(hexadecimal & 0xFF);
  _trackBall.setWhite(brightness);
}

void PIM447::led(vstatus code)
{
  vcolor color = COLOR_BLACK;

  switch (code) {
    case GRIS:
      color = COLOR_GREY_DARK;
      break;
    case VERT:
      color = COLOR_GREEN;
      break;
    case JAUNE:
      color = COLOR_YELLOW;
      break;
    case ORANGE:
      color = COLOR_ORANGE;
      break;
    case ROUGE:
      color = COLOR_RED;
      break;
    case VIOLET:
      color = COLOR_VIOLET;
      break;
  }

  led(color, 0);
}


void PIM447::setBoundary(int width, int height)
{
  _width = width;
  _height = height;

  resetMouse();
}

void PIM447::setMouse(int x, int y)
{
  _x = x;
  _y = y;
}

void PIM447::resetMouse()
{
  _x = _width / 2;
  _y = _height / 2;
  _focus = false;
  _click = false;
  _left = false;
  _right = false;
  _up = false;
  _down = false;
}
