/*
 * This class overload Html class to define specific page for 'esp32-wrover/Environment' project
 * it uses any brosser type below Screen interface
 *
 */

#ifndef EnvironmentHtmlL_h
#define EnvironmentHtml_h

#include "Arduino.h"
#include "../data/Html.h"


class EnvironmentHtml: public Html
{
  public:

    EnvironmentHtml(Webserver &obj) : Html(obj) {};

    String    makeHtmlScript();
    String    handleHomePage(int delay);
    String    handleHistorySvgGraph(vfield data, Buffer buffer);
    String    handleDataTable(vfield* sensors, int length);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String text);

};

#endif
