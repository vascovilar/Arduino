/*
 * This class overload Bitmap class to define specific page for 'esp32-wrover/Environment' project
 * it uses Adafruit TFT screen ADA4383 below Screen interface
 *
 * More:
 * Adafruit page: https://learn.adafruit.com/adafruit-1-14-240x135-color-tft-breakout/overview
 * ST7789VW chipset pdf: https://cdn-learn.adafruit.com/assets/assets/000/082/882/original/ST7789VW_SPEC_V1.0.pdf?1571860977
 * Arduino driver: https://github.com/Bodmer/TFT_eSPI
 *
*/

#ifndef EnvironmentBitmap_h
#define EnvironmentBitmap_h

#include "Arduino.h"
#include "../data/Bitmap.h"
#include "../data/Snapshot.h"
#include "../bin/earthImage.h"


class EnvironmentBitmap : public Bitmap
{
  public:

    EnvironmentBitmap(Screen &obj1, Mouse &obj2) : Bitmap(obj1, obj2) {};

    void    handleHomePage(String trigram, String ip, String ssid, int offset);
    void    handleHomeUpdate(String clock, vfield cycles, bool isConnected, int offset);
    void    handleEventUpdate(vsensor index, String dateTime, vfield field, Buffer buffer, vcolor bgColor, int offset);
    void    handleHistorySummary(vfield field, float delta, byte trend, vcolor bgColor, int offset);
    void    handleHistoryPage(String title, vfield field, int currentDelay, bool isRealTime, vcolor bgColor, int offset);
    void    handleHistoryUpdate(vsensor code, vfield field, Buffer buffer, vcolor bgColor, int offset);
    void    handleRealtimePage(String trigram, vfield field, int offset);
    void    handleRealtimeUpdate(Sensor &sensor, int length, vfield field, int processedChecks, int offset);
    void    handleSatellitePage(String trigram, int offset);
    void    handleSatelliteUpdate(String dateTime, vfield satellite, float latitude, float longitude, vfield speed, vfield altitude, float cap, vfield fixQuality, int offset);
};

#endif
