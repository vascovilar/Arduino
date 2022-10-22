#include "VComScreen.h"

void VComScreen::begin() 
{
  _tft.init();
  _tft.setRotation(0);
  clear();
}

void VComScreen::menu() 
{
  _tft.setTextColor(TFT_WHITE);

  _tft.fillRect(0, 74, 240, 24, TFT_BLUE);
  _tft.drawCentreString("air", 120, 50, 4);
  _tft.drawCentreString("light", 120, 74, 4);
  _tft.drawCentreString("gps", 120, 98, 4);
}
