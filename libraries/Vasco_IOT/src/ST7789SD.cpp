#include "ST7789SD.h"

bool ST7789SD::init()
{
  // SPI tft device
  _tft.init(VSCREEN_HEIGHT, VSCREEN_WIDTH);
  _tft.setRotation(3); // set to 3 when device will be finally assembled
  _tft.fillScreen(0);

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
  led(false); // TODO vasco why do not works

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
  _updateLedPMW();

  return false;
}

bool ST7789SD::update()
{
  return false;
}

void ST7789SD::text(String text, int x, int y, vtextsize size, vcolor code, vcolor bg, bool isInBuffer)
{
  Adafruit_GFX *object = _getDisplayPointer(isInBuffer);
  object->setTextSize(size);
  object->setCursor(x, y);
  object->setTextColor(_convertToRGB565(code), _convertToRGB565(bg));
  object->setTextWrap(false);
  object->print(text);
}

void ST7789SD::point(int x, int y, vcolor code, bool isInBuffer)
{
  Adafruit_GFX *object = _getDisplayPointer(isInBuffer);
  object->drawPixel(x, y, _convertToRGB565(code));
}

void ST7789SD::line(int x1, int y1, int x2, int y2, vcolor code, bool isInBuffer)
{
  Adafruit_GFX *object = _getDisplayPointer(isInBuffer);
  object->drawLine(x1, y1, x2, y2, _convertToRGB565(code));
}
void ST7789SD::rect(int x, int y, int width, int height, vcolor code, int radius, bool isInBuffer)
{
  Adafruit_GFX *object = _getDisplayPointer(isInBuffer);
  object->fillRoundRect(x, y, width, height, radius, _convertToRGB565(code));
}

void ST7789SD::arrow(int x, int y, int width, int height, vcolor code, bool isInBuffer)
{
  Adafruit_GFX *object = _getDisplayPointer(isInBuffer);

}

void ST7789SD::circle(int x, int y, int radius, vcolor code, bool isInBuffer)
{
  Adafruit_GFX *object = _getDisplayPointer(isInBuffer);
  object->fillCircle(x, y, radius, _convertToRGB565(code));
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

void ST7789SD::led(bool status)
{
  _ledPWM(status ? 4095: 0);
}

void ST7789SD::led(int magnitude)
{
  _ledPWM(magnitude);
}

void ST7789SD::led(int from, int to, int duration)
{
  _ledPWM(from, to, duration);
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
  Adafruit_GFX *object;
  if (isInBuffer) {
    object = &_canvas;
  } else {
    object = &_tft;
  }

  return object;
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