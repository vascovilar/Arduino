/*
 * Graphics data lib
 *
 * used to draw text & images on web via an HTTP server
 */

#ifndef Html_h
#define Html_h

#include "Arduino.h"
#include "../interface/Data.h"
#include "../interface/Screen.h"
#include "../run/Webserver.h"
#include "../interface/Sensor.h"
#include "../data/Buffer.h"


class Html : public Data, public Screen
{
  public:

    Html(Webserver &obj) : _server(obj) {};  // bind objects

    // interface
    void      text(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT, bool isFixedWidthFont = false);
    void      point(float x, float y, vcolor color);
    void      line(float x1, float y1, float x2, float y2, vcolor color);
    void      rect(float x, float y, int width, int height, vcolor color, int radius = 0, bool isFilled = true);
    void      arrow(float x, float y, int width, int height, vcolor color, vdirection direction);
    void      circle(float x, float y, int radius, vcolor color);
    void      bitmap(float x, float y, const unsigned char* data, int width, int height, vcolor color, vcolor bgColor = COLOR_TRANSPARENT);
    float     width(String content, vtextsize size = SIZE_NULL, bool isFixedWidthFont = false); // not used here
    float     height(String content, vtextsize size = SIZE_NULL);  // not used here
    void      vertical(float x, float y, String content, vtextsize size, vcolor color, vcolor bgColor = COLOR_TRANSPARENT);
    void      copy(float x, float y, int width, int height); // not used here
    void      paste(float x, float y, int width, int height); // not used here
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
    String    _htmlScript = ""; // TODO vasco *** temporaire, essayer en envoyant directment dans _server

    String    _getHtmlSize(vtextsize size);
    String    _getHtmlColor(vcolor color);
    String    _getHtmlColor(vstatus status);

    String    _getPageWrapper(String content);
    String    _getHtmlBlocEnvironment(int reloadDelay);
    String    _getHtmlBlocSvgCartouche(String content);
    String    _getHtmlLink(String href, String text);

};

#endif