/*
 * Web Server (if wifi connected), return html pages over http on port 80
 *
 * Implementation:
 *
 *   #include "Webserver.h"
 *
 *   Webserver server;
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

#ifndef Webserver_h
#define Webserver_h

#include "Arduino.h"
#include "../interface/Run.h"
#include "../component/Timer.h"
#include "WebServer.h"
#include "WiFi.h"
#include "FS.h"

// need to be global because called by lambda functions
static WebServer _server(80);
//static WiFiServer _server(80);


class Webserver : public Run, public Timer
{
  public:

    // interface
    bool    begin(vrun mode);
    bool    run();

    // api (config before calling begin function)
    void    onHtml(const String &uri, std::function<String()> callHtml);
    void    onHtml(const String &uri, std::function<String(int)> callHtml);
    void    onSvg(const String &uri, std::function<String()> callHtml);
    void    onSvg(const String &uri, std::function<String(int)> callHtml);
    void    onJpg(const String &uri, std::function<File()> callFile);
    void    onJpg(const String &uri, std::function<File(int)> callFile);
    void    onCommand(const String &uri, std::function<void()> callCommand);
    void    onCommand(const String &uri, std::function<void(int)> callCommand);

  private:

    long    _timeBuffer = 0;
};

#endif
