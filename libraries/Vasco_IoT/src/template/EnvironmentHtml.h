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

    String    handleHomePage(String title, String subtitle, vsensor* list, int length, int reloadDelay);
    String    handleHistorySvgGraphs(vfield data, Buffer buffer);
    String    handleDataTables(String title, String subtitle, vdatatable* chipsets, int chipsetLength, vfield* sensors, int sensorLength);
    String    handleOsmPoint(float latitude, float longitude, float angle);
    String    handleGpsInfo(int satellites, String quality, float altitude, float speed);
    String    handleNotification(String content);

  private:

    String    _drawEnvironmentGraphs(vsensor* list, int length, int reloadDelay);
    String    _drawSensorDataTable(vfield* sensors, int sensorLength);
};

#endif
