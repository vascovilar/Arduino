#include "ST7789SD.h"

bool ST7789SD::init()
{
  // set custom font
  _gfxFont = (GFXfont *)&muMatrix8ptRegular;

  // SPI tft device
  _tft.init();
  _tft.setRotation(1); // set to 3 when device will be finally assembled
  _tft.fillScreen(0);
  _tft.setFreeFont(_gfxFont); // argument void = system font
  _tft.setTextWrap(false);

  // SD card // TODO vasco

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
  return false;
}

bool ST7789SD::update()
{
  return false;
}

void ST7789SD::text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor, bool isFixedWidthFont)
{
//bgColor = COLOR_RED; // debug

  // none font supports latin chars
  _removeLatinChar(content);

  // switch to selected size
  _tft.setTextSize(_convertToSize(size));

  // switch to fixed size fonts
  if (isFixedWidthFont) {
    _tft.setFreeFont();
    _tft.setCursor(x, y);
    if (bgColor != COLOR_TRANSPARENT) {
      _tft.setTextColor(_convertToRGB565(color), _convertToRGB565(bgColor));
    } else {
      _tft.setTextColor(_convertToRGB565(color));
    }
  } else {
    // Adafruit convention custom fonts draw up from baseline so move cursor
    _tft.setCursor(x, y + VSCREEN_CHAR_HEIGHT * _convertToSize(size));
    // custom font dont use background parameter in setTextColor function
    _tft.setTextColor(_convertToRGB565(color));
    // COLOR_WHITE is defined as transparent for backgrounds
    if (bgColor != COLOR_TRANSPARENT) {
      // draw missing background
      _tft.fillRect(x, y, width(content), height(content), _convertToRGB565(bgColor));
    }
  }

  // draw text
  _tft.print(content);

  // switch back to custom fonts right after printing
  if (isFixedWidthFont) {
    _tft.setFreeFont(_gfxFont);
  }
}

void ST7789SD::point(float x, float y, vcolor color)
{
  _tft.drawPixel(x, y, _convertToRGB565(color));
}

void ST7789SD::line(float x1, float y1, float x2, float y2, vcolor color)
{
  _tft.drawLine(x1, y1, x2, y2, _convertToRGB565(color));
}
void ST7789SD::rect(float x, float y, int width, int height, vcolor color, int radius, bool isFilled)
{
  if (isFilled) {
    _tft.fillRoundRect(x, y, width, height, radius, _convertToRGB565(color));
  } else {
    _tft.drawRoundRect(x, y, width, height, radius, _convertToRGB565(color));
  }
}

void ST7789SD::arrow(float x, float y, int width, int height, vcolor color, vdirection direction)
{
  // center x, y is arrowhead
  switch (direction) {
    case UP:
      _tft.fillTriangle(x, y, x - width / 2, y + height, x + width / 2, y + height, _convertToRGB565(color));
      break;
    case DOWN:
      _tft.fillTriangle(x, y, x - width / 2, y - height, x + width / 2, y - height, _convertToRGB565(color));
      break;
    case LEFT:
      _tft.fillTriangle(x, y, x - width, y + height / 2, x - width, y - height / 2, _convertToRGB565(color));
      break;
    case RIGHT:
      _tft.fillTriangle(x, y, x + width, y + height / 2, x + width, y - height / 2, _convertToRGB565(color));
      break;
  }
}

void ST7789SD::circle(float x, float y, int radius, vcolor color)
{
  _tft.fillCircle(x + radius, y + radius, radius, _convertToRGB565(color));
}

void ST7789SD::bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor)
{
  if (bgColor != COLOR_TRANSPARENT) {
    rect(x, y, width, height, bgColor);
  }

  _tft.drawBitmap(x, y, data, width, height, _convertToRGB565(color));
}

float ST7789SD::width(String content, vtextsize size, bool isFixedWidthFont)
{
  short unsigned int width = 0;

  if (content == "") {

    return width;
  }

  // no font supports latin chars
  _removeLatinChar(content);

  if (isFixedWidthFont) {
    // keep simple to be fastest
    width = content.length() * VSCREEN_CHAR_WIDTH * _convertToSize(size);
  } else {
    if (size != SIZE_NULL) {
      _tft.setTextSize(_convertToSize(size));
      _tft.setFreeFont(_gfxFont);
    }
    width = _tft.textWidth(content, 1);
  }

  return width;
}

float ST7789SD::height(String content, vtextsize size)
{
  if (content == "") {

    return 0;
  }

  int multiplier = (size != SIZE_NULL) ? _convertToSize(size): _tft.textsize;

  return (VSCREEN_CHAR_HEIGHT + 1) * multiplier; // ever 1 line height
}

void ST7789SD::vertical(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor)
{
  _spr.createSprite(60, VSCREEN_COL_PAGE);
  _spr.setPivot(y, x);      // Set pivot relative to top left corner of Sprite
  if (bgColor != COLOR_TRANSPARENT) {
    _spr.fillSprite(_convertToRGB565(bgColor));
  }
  _spr.setTextColor(_convertToRGB565(color));
  _spr.drawString(content, 0, 0, size);
  _spr.pushRotated(270);
  _spr.deleteSprite();
}

void ST7789SD::copy(float x, float y, int width, int height)
{
  _tft.readRect(x, y , width, height, _bitmapBuffer);
}

void ST7789SD::paste(float x, float y, int width, int height)
{
  _tft.pushRect(x, y , width, height, _bitmapBuffer);
}


void ST7789SD::clear()
{
  _tft.fillScreen(0);
}

void ST7789SD::show(String content)
{
  float paddingToCenter = VSCREEN_WIDTH / 2.0 - width(content, SIZE_TEXT) / 2.0;
  float offsetToCenter = VSCREEN_HEIGHT / 2.0;

  rect(0, offsetToCenter - 10, VSCREEN_WIDTH, offsetToCenter + 10, COLOR_BLACK);
  text(paddingToCenter, offsetToCenter, content, SIZE_TEXT, COLOR_WHITE, COLOR_TRANSPARENT, true);
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
  switch (size) {
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
