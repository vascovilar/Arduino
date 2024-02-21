/*
 * WebServer abstraction
 * Implementation:
 *
 *   #include <VDeviceWebServer.h>
 *
 *   VDeviceWebServer web;
 *
 *   void setup() {
 *     web.onPage("/", [](){ return html.getHome(); });
 *     web.begin();
 *   }
 *   void loop() {
 *     web.update(10);
 *   }
 */

#ifndef VDeviceWebServer_h
#define VDeviceWebServer_h

#include "Arduino.h"
#include "WebServer.h"
#include "FS.h"

static WebServer _server(80);

class VDeviceWebServer
{
  public:   
    
    void begin();
    bool update(int delay);

    void onHtml(const String &uri, std::function<String()> callHtml);
    void onHtml(const String &uri, std::function<String(int)> callHtml);
    void onSvg(const String &uri, std::function<String()> callHtml);
    void onSvg(const String &uri, std::function<String(int)> callHtml);
    void onJpg(const String &uri, std::function<File()> callFile);
    void onJpg(const String &uri, std::function<File(int)> callFile);
    void onCommand(const String &uri, std::function<void()> callCommand);
    void onCommand(const String &uri, std::function<void(int)> callCommand);
    
  private:
    
    unsigned int _timer = millis();

};

#endif
