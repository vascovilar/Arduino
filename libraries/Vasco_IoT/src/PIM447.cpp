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

  // reset values
  reset();
  led(0, 0);

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
  if (_trackBall.changed()) { // TODO vasco * do same thing for other classes update functions (if new data)
    int leftDelta = pow(_trackBall.left(), 2) / 2;
    int rightDelta = pow(_trackBall.right(), 2) / 2;
    int upDelta = pow(_trackBall.up(), 2) / 2;
    int downDelta = pow(_trackBall.down(), 2) / 2;
    // apply to screen referentialtrack.
    _x = _x + (rightDelta - leftDelta);
    if (_x < 0) _x = 0;
    if (_x >= _width - 1) _x = _width - 1;
    _y = _y + (downDelta - upDelta);
    if (_y < 0) _y = 0;
    if (_y > _height - 1) _y = _height - 1;
    // consider clicked when button released
    _focus = _trackBall.click();
    _click = _trackBall.release();
    // direction for menu navigation
    _left = leftDelta > rightDelta;
    _right = rightDelta > leftDelta;
    _up = upDelta > downDelta;
    _down = downDelta > upDelta;

    return true;
  }

  return false;
}

vpointer PIM447::get()
{
  return (vpointer){
    _x,
    _y,
    _focus,
    _click,
    _left,
    _right,
    _up,
    _down
  };
}

void PIM447::set(vpointer pointer)
{
  _x = pointer.x;
  _y = pointer.y;
  _focus = pointer.focus;
  _click = pointer.click;
  _left = pointer.left;
  _right = pointer.right;
  _up = pointer.up;
  _down = pointer.down;
}

void PIM447::reset()
{
  _x = _width / 2.0;
  _y = _height / 2.0;
  _focus = false;
  _click = false;
  _left = false;
  _right = false;
  _up = false;
  _down = false;
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
  vcolor color = _convert(code);
  led(color);
}
