#include "VDeviceST7789SD.h"

bool VDeviceST7789SD::init()
{
  _initPWM(_pwmPin, _PWM_CHANNEL);
  
  _tft.init(135, 240);
  _tft.setRotation(3);
  _tft.fillScreen(ST77XX_BLACK);

  return true;
}

bool VDeviceST7789SD::wake()
{
  return true;
}

bool VDeviceST7789SD::sleep()
{ 
  return true;
}

void VDeviceST7789SD::clear()
{
  _tft.fillScreen(ST77XX_BLACK);
}

void VDeviceST7789SD::title(String text, int x, int y)
{
  _tft.setTextColor(0xFFFF);
  _tft.setTextSize(2);
  _tft.setCursor(x, y);
  _tft.print(text);
}

void VDeviceST7789SD::text(String text, int x, int y)
{
  _tft.setTextColor(0xCCCC);
  _tft.setTextSize(1);
  _tft.setCursor(x, y);
  //_tft.setTextWrap(true);
  _tft.print(text);
}