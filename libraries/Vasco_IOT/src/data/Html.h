#ifndef Html_h
#define Html_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "Filter.h"
#include "../interface/Sensor.h"
#include "Buffer.h"
 

class Html : public Data, Filter
{
  public:

    String    handleHomePage(int delay);
    String    handleHistorySvgGraph(vfield data, Buffer buffer);
    String    handleDataTable(vfield* sensors, int length);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String text);

  private:
  
    String    _getPageWrapper(String content, vcolor backgroundColor, vcolor fontColor, vcolor titleColor);
    String    _getHtmlBlocEnvironment(vcolor titleColor, int reloadDelay);
    String    _getHtmlBlocSvgCartouche(String text, vcolor gridColor, String grid, vcolor graphColor, String graph);
    String    _getHtmlColor(vcolor code);
    String    _getHtmlColor(vstatus code);
    String    _getHtmlSvgLine(int x1, int y1, int x2, int y2, float size = 1.0);
    String    _getHtmlSvgText(int x, int y, int size, String color, String text);
    String    _getHtmlSvgBig(int offset, float value);
    String    _getHtmlSvgRect(int x, int y, int w, int h, String color);
    String    _getHtmlSvgArrow(int x, int y, String color);
    String    _getHtmlSvgCircle(int x, int y, String color); 
    String    _getHtmlLink(String href, String text);
};

#endif