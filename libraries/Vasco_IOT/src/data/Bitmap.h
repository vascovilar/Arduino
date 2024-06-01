/*
 * Graphics data lib
 *
 * used to draw text & images on a TFT screen
 */

#ifndef Bitmap_h
#define Bitmap_h

#include "Arduino.h"
#include "../data/Mouse.h"
#include "../interface/Screen.h"
#include "../data/Buffer.h"
#include "bin/iconMonoTransparent.h"


class Bitmap : public Data
{
  public:

    Bitmap(Mouse &obj1, Screen &obj2) : _mouse(obj1), _screen(obj2) {};  // bind objects

    // api
    void    handleHomePage(String trigram, String ip, String ssid, int offset);
    void    handleHomeUpdate(String clock, vfield cycles, bool isConnected, int offset);
    void    handleRealtimePage(String trigram, int offset);
    void    handleRealtimeUpdate(Sensor &sensor, int length, int offset);
    void    handleHistorySummary(vfield field, float delta, byte trend, vcolor bgColor, int offset);
    void    handleHistoryPage(String title, vfield field, Buffer buffer, vcolor bgColor, int offset);
    void    handleHistoryUpdate(vfield field, Buffer buffer, vcolor bgColor, int offset);
    void    handleSatellitePage(String trigram, int offset);
    void    handleSatelliteUpdate(String dateTime, vfield satellite, float latitude, float longitude, vfield speed, vfield altitude, float cap, vfield fixQuality, int offset);
    void    handleEventUpdate(String dateTime, vfield field, Buffer buffer, vcolor bgColor, int offset);

  private:

    Mouse   &_mouse;
    Screen  &_screen;

    vzone   _drawPage(String titleLeft, String titleRight, int forcedWidth = 0, vcolor color = COLOR_BLUE);
    vzone   _drawPill(float x, float y, String contentLeft, String contentRight, vcolor color, int forcedWidth = 0);
    vzone   _drawButtonMini(float x, float y, String content, vpage target);
    vzone   _drawButtonBack(float x, vpage target);
    vzone   _drawBullet(float x, float y, vcolor color);
    vzone   _drawAnalogBar(float x, float y, float value, float minimum, float maximum);
    vzone   _drawIcon(float x, float y, const unsigned char* icon, int size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT);
    vzone   _drawTableHeader(float x, float y, String* titles, int length);
    vzone   _drawTableData(float x, float y, String* values, int length);
};

#endif
