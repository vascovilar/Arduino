#ifndef Html_h
#define Html_h

#include "Arduino.h"
#include "../data/Buffer.h"
#include "../interface/Data.h"
#include "../interface/Sensor.h"
#include "../inherit/Rtc.h"


class Html : public Data, public Rtc
{
  public:

    String    handleHomePage(int delay);
    String    handleHistorySvgGraph(vfield data, Buffer buffer);
    String    handleDataTable(vfield* sensors, int length);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String text);

  private:

    String    _getPageWrapper(String content);
    String    _getHtmlBlocEnvironment(int reloadDelay);
    String    _getHtmlBlocSvgCartouche(String text, String grid, String graph);
    String    _getHtmlColor(vcolor code);
    String    _getHtmlColor(vstatus code);
    String    _getHtmlSvgTitle(int offset, float value);
    String    _getHtmlSvgText(float x, float y, int size, vcolor color, String text);
    String    _getHtmlSvgLine(float x1, float y1, float x2, float y2, vcolor color, float size = 1.0);
    String    _getHtmlSvgRect(float x, float y, int width, int height, vcolor color);
    String    _getHtmlSvgArrow(float x, float y, vcolor color);
    String    _getHtmlSvgCircle(float x, float y, vstatus color);
    String    _getHtmlLink(String href, String text);
};

#endif