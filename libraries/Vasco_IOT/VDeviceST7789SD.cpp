#include "VDeviceST7789SD.h"

bool VDeviceST7789SD::init()
{  
  // SPI tft device
  _tft.init(135, 240);
  _tft.setRotation(1); // TODO vasco set to 3 when device will be finally assembled 
  _tft.fillScreen(convert(COLOR_BLACK));
  
  // TODO vasco: urgent fix SD card
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

bool VDeviceST7789SD::wake()
{
  return true;
}

bool VDeviceST7789SD::sleep()
{ 
  return true;
}

bool VDeviceST7789SD::check()
{ 
  return false;
}

bool VDeviceST7789SD::update()
{ 
  return false;
}

void VDeviceST7789SD::clear()
{
  _tft.fillScreen(convert(COLOR_BLACK));
}

void VDeviceST7789SD::light(int magnitude)
{
  _ledPWM(magnitude);
}

void VDeviceST7789SD::title(String text, int x, int y, int color)
{
  _tft.setTextSize(2);
  _tft.setCursor(x, y);
  _tft.setTextColor(convert(color));
  _tft.print(text);
}

void VDeviceST7789SD::text(String text, int x, int y, int color)
{
  _tft.setTextSize(1);
  _tft.setCursor(x, y);
  _tft.setTextColor(convert(color));
  _tft.setTextWrap(true);
  _tft.print(text);
}

int VDeviceST7789SD::convert(int hexadecimal)
{
  int red = hexadecimal >> 16;
  int green = hexadecimal >> 8 & 0xFF;
  int blue = hexadecimal & 0xFF;

  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

int VDeviceST7789SD::convert(String html)
{  
  html = html.substring(1); // remove first # symbol
  byte len = html.length() + 1; // with one extra character for the null terminator
  
  char str[len];
  char *ptr;
  html.toCharArray(str, len);
  int value = strtoul(str, &ptr, 16);

  return convert(value);
}

void VDeviceST7789SD::listFiles()
{
  File32 dir;
  File32 file;
  char fileName[20];

  dir.open("/");
  dir.rewind();

  Serial.println(F("Files in SD root directory:"));
  while (file.openNext(&dir, O_READ)) {
    if (!file.isHidden()) {
      file.getName(fileName, sizeof(fileName));
      Serial.print(" - " + String(fileName));

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
