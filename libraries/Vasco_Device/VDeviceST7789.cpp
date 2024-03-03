#include "VDeviceST7789.h"

void VDeviceST7789::begin(int rotation) 
{
  _tft.init();
  _tft.setRotation(rotation);
}

void VDeviceST7789::clear()
{
  _tft.fillScreen(TFT_BLACK);
}

void VDeviceST7789::title(String text, int x, int y)
{
  _tft.setTextColor(TFT_LIGHTGREY);
  _tft.drawString(text, x, y, 4);
}

void VDeviceST7789::text(String text, int x, int y)
{
  _tft.setTextColor(TFT_LIGHTGREY);
  _tft.drawString(text, x, y, 2);
}