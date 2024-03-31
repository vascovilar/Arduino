
#include "Arduino.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  // put your setup code here, to run once:

  /*
  byte red = 0;
  byte green = 0;
  byte blue = 255;
  //int rgb = (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
  //Serial.println(rgb, HEX);

  int number = 0x0000FF;
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  //int rgb = (((r & 0b11111000)<<8) + ((g & 0b11111100)<<3)+(b>>3));
  //Serial.println(rgb, HEX);
  
  String html = "FF00FF";
  byte len = html.length() + 1; // with one extra character for the null terminator
  char str[len];
  char *ptr;
  html.toCharArray(str, html.length() + 1);
  //int rgb = strtoul(str, &ptr, 16);
  //Serial.println(rgb, HEX);
  */

  /*
   from html string to rgb gfx code
  */

  String html = "#00FF00";
  
  html = html.substring(1); // remove first # symbol
  byte len = html.length() + 1; // with one extra character for the null terminator
  char str[len];
  char *ptr;
  html.toCharArray(str, html.length() + 1);
  int number = strtoul(str, &ptr, 16);
  
  int red = number >> 16;
  int green = number >> 8 & 0xFF;
  int blue = number & 0xFF;

  int rgb = (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
  
  




  
  Serial.println(rgb, HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
