/*
 * WebServer abstraction
 * Implementation:
 *
 *   #include <VToolWebServer.h>
 *
 *   VToolWebServer web;
 *
 *   void setup() {
 *     web.onPage("/", [arg](){ return html.getHome(arg); });
 *     web.begin();
 *   }
 *   void loop() {
 *     web.update(10);
 *   }
 */

#ifndef VToolWebServer_h
#define VToolWebServer_h

#include "Arduino.h"
#include "WebServer.h"
#include "FS.h"

static WebServer _server(80);

class VToolWebServer
{
  public:   
    
    void begin();
    bool update(int delay);
    void sync();

    void onHtml(const String &uri, std::function<String()> callHtml);
    void onHtml(const String &uri, std::function<String(int)> callHtml);
    void onSvg(const String &uri, std::function<String()> callHtml);
    void onSvg(const String &uri, std::function<String(int)> callHtml);
    void onJpg(const String &uri, std::function<File()> callFile);
    void onJpg(const String &uri, std::function<File(int)> callFile);
    void onCommand(const String &uri, std::function<void()> callCommand);
    void onCommand(const String &uri, std::function<void(int)> callCommand);
    
  private:
    
    unsigned int _timer;
};

#endif
