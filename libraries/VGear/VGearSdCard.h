/*
 * Handle SD card on ESP32-cam
 * Implementation:
 *
 *   #include <VGearSdCard.h>
 *
 *   VGearSdCard card; 
 *
 *   void setup() {
 *     card.begin();
 *     Page list = card.list("/", 1);
 *     for (int i = 0; i < list.length; i++) {
 *       Serial.println(list.data[i]);
 *     }
 *     Serial.println("total: " + String(list.total));  
 *   }
 *   void loop() {
 *   }
 */

#ifndef VGearSdCard_h
#define VGearSdCard_h

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"

// Define CS pin for the SD card module
#define SD_MISO 2
#define SD_MOSI 15
#define SD_SCLK 14
#define SD_CS 13

// Define pager object
#define PAGE_LENGTH 10
struct Page {
  int total;
  int length;
  int page;
  String data[PAGE_LENGTH];
};

static SPIClass _spi(VSPI);

class VGearSdCard
{
  public:

    void begin();
    
    Page list(const String &path);
    File open(const String &path);
    void close(File file);
    void save(const String &path, uint8_t * data, size_t length);
    void remove(const String &path); 
    String date(const String &path);

  private:

};

#endif