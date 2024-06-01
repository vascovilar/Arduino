/*
 * Graphics data lib
 *
 * used to draw text & images on web via an HTTP server
 */

#ifndef Html_h
#define Html_h

#include "Arduino.h"
#include "../data/Buffer.h"
#include "../interface/Data.h"
#include "../interface/Sensor.h"
#include "../interface/Screen.h"
#include "../run/Webserver.h"


class Html : public Data, public Screen
{
  public:

    Html(Webserver &obj) : _server(obj) {};  // bind objects

    // interface
    void      text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isFixedWidthFont = false, bool isInBuffer = false);
    void      point(float x, float y, vcolor color, bool isInBuffer = false);
    void      line(float x1, float y1, float x2, float y2, vcolor color, bool isInBuffer = false);
    void      rect(float x, float y, int width, int height, vcolor color, int radius = 0, bool isFilled = true, bool isInBuffer = false);
    void      arrow(float x, float y, int width, int height, vcolor color, bool isInBuffer = false);
    void      circle(float x, float y, int radius, vcolor color, bool isInBuffer = false);
    void      bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isInBuffer = false);
    float     width(String content, vtextsize size, bool isFixedWidthFont = false); // not used here
    float     height(String content, vtextsize size);  // not used here
    void      swap(); // not used here
    void      clear(); // not used here

    // api
    String    makeHtmlScript();
    String    handleHomePage(int delay);
    String    handleHistorySvgGraph(vfield data, Buffer buffer);
    String    handleDataTable(vfield* sensors, int length);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String text);

  private:

    Webserver &_server;
    String    _htmlScript = ""; // TODO vasco temporaire, essayer en envoaynt directment dans _server

    String    _getHtmlSize(vtextsize size);
    String    _getHtmlColor(vcolor color);
    String    _getHtmlColor(vstatus status);

    String    _getPageWrapper(String content);
    String    _getHtmlBlocEnvironment(int reloadDelay);
    String    _getHtmlBlocSvgCartouche(String content);
    String    _getHtmlLink(String href, String text);

};

#endif