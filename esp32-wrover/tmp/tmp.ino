//#define ARDUINO 100

#include "Arduino.h"
#include "Adafruit_GFX.h"         // Core graphics library
#include "Adafruit_ST7789.h"      // Hardware-specific library for ST7789
#include "SdFat.h"                // SD card & FAT filesystem library
#include "Adafruit_ImageReader.h" // Image-reading functions

#define SD_CS    33 // SD card select pin
#define TFT_CS    5 // TFT select pin
#define TFT_DC   25 // TFT display/command pin
#define TFT_RST  -1 // Or set to -1 and connect to Arduino RESET pin

Adafruit_ST7789      tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
SdFat                SD;         // SD card filesystem
Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
Adafruit_Image       img;        // An image loaded into RAM
int32_t              width  = 0, // BMP image dimensions
                     height = 0;

void displayDirectoryContent(File& aDirectory, byte tabulation) {
  File file;
  char fileName[20];

  if (!aDirectory.isDir()) return;
  aDirectory.rewind();

  while (file.openNext(&aDirectory, O_READ)) {
    if (!file.isHidden()) {
      file.getName(fileName, sizeof(fileName));
      for (uint8_t i = 0; i < tabulation; i++) Serial.write('\t');
      Serial.print(fileName);

      if (file.isDir()) {
        Serial.println(F("/"));
        displayDirectoryContent(file, tabulation + 1);
      } else {
        Serial.write('\t'); Serial.print(file.fileSize()); Serial.println(F(" bytes"));
      }
    }
    file.close();
  }
}

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("***********************************************************************************");  

  ImageReturnCode stat; // Status from image-reading functions
  File dir;

  tft.init(135, 240);           // Init ST7789 172x320
  tft.fillScreen(ST77XX_BLUE);
  tft.setRotation(2);
  tft.fillScreen(0);

  if(!SD.begin(SD_CS, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
    Serial.println(F("SD begin() failed"));
    for(;;); // Fatal error, do not continue
  }

  dir.open("/");
  displayDirectoryContent(dir, 0);

  Serial.print(F("Loading to canvas..."));
  stat = reader.loadBMP("/minerva.bmp", img);
  reader.printStatus(stat);

  delay(2000);
}

void loop() 
{
  // shift
  for(int i=0; i<135; i++) {
    img.draw(tft, 135 - i, 0);
  }

  delay(2000);
}
