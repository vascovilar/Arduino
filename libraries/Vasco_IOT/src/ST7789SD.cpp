#include "ST7789SD.h"

bool ST7789SD::init()
{
  // SPI tft device
  _tft.init(VSCREEN_HEIGHT, VSCREEN_WIDTH);
  _tft.setRotation(3); // set to 3 when device will be finally assembled
  _tft.fillScreen(0);
  //_tft.setFont(); // system font
  _tft.setFont(&muMatrix8ptRegular); // custom font included
  _tft.setTextWrap(false);


  // TODO vasco: fix SD card ? or device dead ?
  /*
  // SPI SD card device
  if(!_sd.begin(_sdcdPin, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
    Serial.println(F("SD Card init failed"));
    return false;
  }*/

  // backlight
  if (!_initPWM(_litPin, _PWM_CHANNEL)) {
    Serial.println(F("Backlight init failed"));

    return false;
  }

  // avoid screen backlight always on
  clear();
  led(false);

  return true;
}

bool ST7789SD::sleep()
{
  return true;
}

bool ST7789SD::wake()
{
  return true;
}

bool ST7789SD::check()
{
  // led fadeout if needed
  runLedFader();

  return false;
}

bool ST7789SD::update()
{
  return false;
}

void ST7789SD::text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor, bool isFixedWidthFont, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);

//bgColor = COLOR_RED; // debug

  // none font supports latin chars
  _removeLatinChar(content);

  // switch to fixed size fonts
  if (isFixedWidthFont) {
    display->setFont();
  }

  // With custom font setted in init()
  display->setTextSize(_convertToSize(size));
  if (isFixedWidthFont) {
    display->setCursor(x, y);
    if (bgColor != COLOR_TRANSPARENT) {
      display->setTextColor(_convertToRGB565(color), _convertToRGB565(bgColor));
    } else {
      display->setTextColor(_convertToRGB565(color));
    }
  } else {
    // Adafruit convention custom fonts draw up from baseline so move cursor
    display->setCursor(x, y + VSCREEN_CHAR_HEIGHT * _convertToSize(size));
    // custom font dont use background parameter in setTextColor function
    display->setTextColor(_convertToRGB565(color));
    // COLOR_WHITE is defined as transparent for backgrounds in custom font mode
    if (bgColor != COLOR_TRANSPARENT) {
      short int xo, yo;
      short unsigned int width, height;
      display->getTextBounds(content, x, y, &xo, &yo, &width, &height);
      // draw missing backgorund
      display->fillRect(xo, yo + VSCREEN_CHAR_HEIGHT * _convertToSize(size), width, height, _convertToRGB565(bgColor));
    }
  }

  // draw text
  display->print(content);

  // switch to custom fonts right after printing
  if (isFixedWidthFont) {
    display->setFont(&muMatrix8ptRegular);
  }
}

void ST7789SD::point(float x, float y, vcolor color, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);
  display->drawPixel(x, y, _convertToRGB565(color));
}

void ST7789SD::line(float x1, float y1, float x2, float y2, vcolor color, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);
  display->drawLine(x1, y1, x2, y2, _convertToRGB565(color));
}
void ST7789SD::rect(float x, float y, int width, int height, vcolor color, int radius, bool isFilled, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);
  if (isFilled) {
    display->fillRoundRect(x, y, width, height, radius, _convertToRGB565(color));
  } else {
    display->drawRoundRect(x, y, width, height, radius, _convertToRGB565(color));
  }
}

void ST7789SD::arrow(float x, float y, int width, int height, vcolor color, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);
  display->fillTriangle(x, y, x + width, y + height / 2, x + width, y - height / 2, _convertToRGB565(color));
}

void ST7789SD::circle(float x, float y, int radius, vcolor color, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);
  display->fillCircle(x + radius, y + radius, radius, _convertToRGB565(color));
}

void ST7789SD::bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor, bool isInBuffer)
{
  Adafruit_GFX *display = _getDisplayPointer(isInBuffer);

  if (bgColor != COLOR_TRANSPARENT) {
    rect(x, y, width, height, bgColor, isInBuffer);
  }

  display->drawBitmap(x, y, data, width, height, _convertToRGB565(color));
}

float ST7789SD::width(String content, vtextsize size, bool isFixedWidthFont)
{
  short unsigned int width = 0;

  if (content == "") {

    return 0;
  }

  // none font supports latin chars
  _removeLatinChar(content);

  if (isFixedWidthFont) {
    width = content.length() * VSCREEN_CHAR_WIDTH * _convertToSize(size);
  } else {
    short int xo, yo;
    short unsigned int height;
    _tft.setTextSize(_convertToSize(size));
    _tft.getTextBounds(content, 0, 0, &xo, &yo, &width, &height); // get text zone by reference
  }

  return width;
}

float ST7789SD::height(String content, vtextsize size)
{
  if (content == "") {

    return 0;
  }

  return VSCREEN_CHAR_HEIGHT * _convertToSize(size); // ever 1 line height
}

void ST7789SD::swap()
{
  // swap double-buffer (canvas -> tft)
  _tft.drawRGBBitmap(0, 0, _canvas.getBuffer(), _canvas.width(), _canvas.height());  // TODO vasco use too much memory
  // reset buffer
  _canvas.fillScreen(0);
}

void ST7789SD::clear()
{
  _tft.fillScreen(0);
}

void ST7789SD::print(String content)
{
  float paddingToCenter = VSCREEN_WIDTH / 2.0 - width(content, SIZE_TEXT) / 2.0;
  float offset = VSCREEN_HEIGHT / 2.0;

  text(paddingToCenter, offset, content, SIZE_TEXT, COLOR_WHITE, COLOR_BLACK);
}

void ST7789SD::listFiles()
{
  File root = SD.open("/");
  int numTabs = 0;

  Serial.println(F("Files in SD root:"));
  while (true)
  {
    File entry =  root.openNextFile();
    if (!entry) {
      break;
    }
    for (int i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.println(String(entry.name()) + " (" + String(entry.size()) + " bytes)");
    entry.close();
  }
}

Adafruit_GFX* ST7789SD::_getDisplayPointer(bool isInBuffer)
{
  Adafruit_GFX *display;

  if (isInBuffer) {
    display = &_canvas;
  } else {
    display = &_tft;
  }

  return display;
}

int ST7789SD::_convertToRGB565(int hexadecimal)
{
  int red = hexadecimal >> 16;
  int green = hexadecimal >> 8 & 0xFF;
  int blue = hexadecimal & 0xFF;

  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

int ST7789SD::_convertToRGB565(String html)
{
  html = html.substring(1); // remove first # symbol
  byte len = html.length() + 1; // with one extra character for the null terminator

  char str[len];
  char *ptr;
  html.toCharArray(str, len);
  int value = strtoul(str, &ptr, 16);

  return _convertToRGB565(value);
}

int ST7789SD::_convertToSize(vtextsize size)
{
  switch(size) {
    case SIZE_SMALL:
      return 1;       // 6x8 px
    case SIZE_TEXT:
      return 1;       // 6x8 px
    case SIZE_TITLE:
      return 2;       // 12x16 px
    case SIZE_BIG:
      return 2;       // 12x16 px
  }

  return 0;
}
