#ifndef Html_h
#define Html_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../interface/Sensor.h"
#include "Buffer.h"


class Html : public Data
{
  public:

    String    handleHomePage(int delay);
    String    handleHistorySvgGraph(vfield data, Buffer buffer);
    String    handleDataTable(vfield* sensors, int length);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String text);

  private:

    String    _getPageWrapper(String content, vcolor backgroundColor, vcolor fontColor);
    String    _getHtmlBlocEnvironment(vcolor titleColor, int reloadDelay);
    String    _getHtmlBlocSvgCartouche(String text, String grid, String graph);
    String    _getHtmlColor(vcolor code);
    String    _getHtmlColor(vstatus code);
    String    _getHtmlSvgLine(float x1, float y1, float x2, float y2, vcolor color, float size = 1.0);
    String    _getHtmlSvgText(float x, float y, int size, vcolor color, String text);
    String    _getHtmlSvgBig(int offset, float value);
    String    _getHtmlSvgRect(float x, float y, int w, int h, vcolor color);
    String    _getHtmlSvgArrow(float x, float y, vcolor color);
    String    _getHtmlSvgCircle(float x, float y, vstatus color);
    String    _getHtmlLink(String href, String text);
    // needed for precision, map function use integers only
    float     _isometric(float value, float maximum, float minimum, int height, int offset);

};

#endif