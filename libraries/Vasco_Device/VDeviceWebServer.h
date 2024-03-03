/*
 * WebServer abstraction
 * Implementation:
 *
 *   #include <VDeviceWebServer.h>
 *
 *   VDeviceWebServer server;
 *
 *   void setup() {
 *     server.onPage("/", [arg](){ return getHome(arg); });
 *     server.begin();
 *   }
 *   void loop() {
 *     server.update(10);
 *   }
 *   String getHome(arg) {
 *     return "Hello world ! " + String(arg);
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
    
    unsigned int _timer = 0;
};

#endif
