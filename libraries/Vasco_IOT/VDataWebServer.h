/*
 * WebServer abstraction
 * Implementation:
 *
 *   #include <VDataWebServer.h>
 *
 *   VDataWebServer server;
 *
 *   void setup() {
 *     server.onPage("/", [arg](){ return getHome(arg); });
 *     server.begin();
 *   }
 *   void loop() {
 *     server.run();
 *   }
 *   String getHome(arg) {
 *     return "Hello world ! " + String(arg);
 *   }
 */

#ifndef VDataWebServer_h
#define VDataWebServer_h

#include "Arduino.h"
#include "VRun.h"
#include "WebServer.h"
#include "FS.h"

static WebServer _server(80); // need to be global because called by lambda functions

class VDataWebServer : public VRun
{
  public:   
    
    // interface
    bool    begin(vmode_code mode);
    bool    run();

    // config before begin server
    void    onHtml(const String &uri, std::function<String()> callHtml);
    void    onHtml(const String &uri, std::function<String(int)> callHtml);
    void    onSvg(const String &uri, std::function<String()> callHtml);
    void    onSvg(const String &uri, std::function<String(int)> callHtml);
    void    onJpg(const String &uri, std::function<File()> callFile);
    void    onJpg(const String &uri, std::function<File(int)> callFile);
    void    onCommand(const String &uri, std::function<void()> callCommand);
    void    onCommand(const String &uri, std::function<void(int)> callCommand);
    
  private:
    
    long _timer = 0;
};

#endif
