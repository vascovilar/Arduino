#include "VDeviceJST2765.h"

void VDeviceJST2765::begin(int xpin, int ypin, int clickpin)
{
  _xpin = xpin;
  _ypin = ypin;
  _clickpin = clickpin;

  pinMode(_clickpin, INPUT_PULLUP);
}

bool VDeviceJST2765::update(int delay)
{
  if (millis() - _timer > delay) {
    _timer = millis();

    _x = _readX();
    _y = _readY();
    _click = _readClick();

    return true;
  }
  
  return false;
}

float VDeviceJST2765::_readX()
{
  return _linearize(analogRead(_xpin), true);
}

float VDeviceJST2765::_readY()
{
  return _linearize(analogRead(_ypin), false);
}

bool VDeviceJST2765::_readClick()
{
  return digitalRead(_clickpin) == 0;
}

float VDeviceJST2765::_linearize(int value, bool axis)
{
  float pos;
  int   center = axis ? _xo: _yo;
  int   max = axis ? _xmax: _ymax;

  // autocenter first
  if (center == 0) {
    center = value - 10;
    max = center * 2;
    if (axis) {
      _xo = center;
      _xmax = max;
    } else {
      _yo = center;
      _ymax = max;
    }    
  }

  // learn max value in using
  if (value > max) {
    max = value - 10;
    if (axis) {
      _xmax = max;
    } else {
      _ymax = max;
    }
  } 

  // linearize in %
  if (value < center) {
    pos = ((float) value / (float) center) * 50;
  } else {
    pos = ((float) (value - center) / (float) (max - center)) * 50 + 50;
  }
    
  return pos;  
}