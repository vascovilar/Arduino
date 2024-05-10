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
#include "WebServer.h"
#include "FS.h"

static WebServer _server(80); // need to be global because called by lambda functions


class Webserver : public Run
{
  static const int _AWARE_REFRESH_RATE = 10;
  static const int _LAZY_REFRESH_RATE = 1000;

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

    long    _timer = 0;
    long    _timeBuffer = 0;
};

#endif
