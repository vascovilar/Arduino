#include "ST7789SD.h"

bool ST7789SD::init()
{
  // SPI tft device
  _tft.init(135, 240);
  _tft.setRotation(3); // set to 3 when device will be finally assembled
  _tft.fillScreen(convert(COLOR_BLACK));
  delay(1000);

  // TODO vasco: urgent fix SD card ? or device dead ?
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
  // led fadeout
  _updateLedPMW();

  return false;
}

bool ST7789SD::update()
{
  return true;
}

void ST7789SD::clear()
{
  _tft.fillScreen(convert(COLOR_BLACK));
}

void ST7789SD::led(int magnitude)
{
  _ledPWM(magnitude);
}

void ST7789SD::led(int from, int to, int duration)
{
  _ledPWM(from, to, duration);
}

void ST7789SD::title(String text, int x, int y, int color)
{
  _tft.setTextSize(2);
  _tft.setCursor(x, y);
  _tft.setTextColor(convert(color));
  _tft.print(text);
}

void ST7789SD::text(String text, int x, int y, int color)
{
  _tft.setTextSize(1);
  _tft.setCursor(x, y);
  _tft.setTextColor(convert(color));
  _tft.setTextWrap(true);
  _tft.print(text);
}

void ST7789SD::point(int x, int y, int color)
{
   _tft.drawPixel(x, y, convert(color));
}

int ST7789SD::convert(int hexadecimal)
{
  int red = hexadecimal >> 16;
  int green = hexadecimal >> 8 & 0xFF;
  int blue = hexadecimal & 0xFF;

  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

int ST7789SD::convert(String html)
{
  html = html.substring(1); // remove first # symbol
  byte len = html.length() + 1; // with one extra character for the null terminator

  char str[len];
  char *ptr;
  html.toCharArray(str, len);
  int value = strtoul(str, &ptr, 16);

  return convert(value);
}

void ST7789SD::listFiles()
{
  File32 dir;
  File32 file;
  char fileName[20];

  dir.open("/");
  dir.rewind();

  Serial.println(F("Files in SD root:"));
  while (file.openNext(&dir, O_READ)) {
    if (!file.isHidden()) {
      file.getName(fileName, sizeof(fileName));
      Serial.print("- " + String(fileName));

      if (file.isDir()) {
        Serial.println(F("/"));
      } else {
        Serial.println(" (" + String(file.fileSize()) + " bytes)");
      }
    }
    file.close();
  }
  dir.close();
}
